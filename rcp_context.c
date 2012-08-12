#include "rcp_pch.h"
#include "rcp_utility.h"

#include "rcp_server.h"

#include "rcp_connection.h"

#include "rcp_command_list.h"
#include "rcp_command.h"

#include "cmd_types.h"

#include "connections/con_file.h"
#include "rcp_receiver.h"
#include "rcp_sender_classes.h"
#include "rcp_receiver_classes.h"

#include "rcp_tree.h"

#include "rcp_user.h"

#include "rcp_type.h"
#include "rcp_type_utility.h"
#include "rcp_record.h"
#include "types/rcp_dict.h"
#include "types/rcp_dict_list.h"
#include "types/rcp_array.h"
#include "types/rcp_struct.h"
#include "types/rcp_string.h"
#include "types/rcp_number.h"
#include "types/rcp_type_list.h"
#include "types/rcp_alias.h"

#include "rcp_context.h"

#include "rcp_caster.h"


struct rcp_context_core{
	rcp_record_ref top_level_record;
	rcp_tree_ref connections;

	//string - uint64
	rcp_dict_ref permissions;

	uint64_t base_permission;

	//string - ptr
	rcp_dict_ref types;

	//connections that closed but not send "removeUser" command. 
	rcp_array_ref dead;

	//string - ptr
	rcp_dict_ref sub_context;
};

size_t rcp_context_size = sizeof (struct rcp_context_core);

void rcp_context_send_record(rcp_context_ref ctx, rcp_record_ref cmd);
void rcp_context_send_data(rcp_context_ref ctx, 
		rcp_type_ref type, rcp_data_ref cmd);

void rcp_context_clean_dead(rcp_context_ref ctx);
void rcp_context_send_all_data(
		rcp_context_ref ctx, rcp_connection_ref con);


void rcp_context_page_in(rcp_context_ref ctx);
void rcp_context_page_out(rcp_context_ref ctx);

rcp_extern rcp_context_ref rcp_context_new()
{
	rcp_context_ref ctx = malloc(sizeof *ctx);
	rcp_context_init(ctx);
	return ctx;
}
void rcp_context_init(rcp_context_ref ctx)
{
	//ctx->top_level_record = 
	//	rcp_map_new_rec(rcp_int64_type, rcp_ref_type);
	ctx->top_level_record = 
		rcp_array_new_rec(rcp_ref_type);
	ctx->connections = rcp_tree_new((void*)rcp_pointer_compare,NULL);
	ctx->dead = rcp_array_new(rcp_pointer_type);
	ctx->permissions = rcp_dict_new(rcp_str_uint64_dict);
	ctx->base_permission = RCP_PMS_LOGIN | RCP_PMS_READ | RCP_PMS_WRITE;
	ctx->types = rcp_dict_new(rcp_str_ptr_dict);
	ctx->sub_context = rcp_dict_new(rcp_str_ptr_dict);
}
void rcp_context_uninit(rcp_context_ref ctx)
{
	//ctx->top_level_record
	rcp_tree_delete(ctx->connections);
	rcp_array_delete(ctx->dead);
	rcp_dict_delete(rcp_str_uint64_dict, ctx->permissions);
	rcp_dict_delete(rcp_str_ptr_dict, ctx->permissions);
	rcp_dict_delete(rcp_str_ptr_dict, ctx->sub_context);
}

rcp_record_ref rcp_context_top_level_record(rcp_context_ref ctx)
{
	return ctx->top_level_record;
}

void rcp_context_test_and_kill(
		rcp_context_ref ctx, rcp_connection_ref con)
{
	rcp_assert(ctx, "test and kill, null ctx");
	if (!rcp_connection_alive(con)){
		rcp_info("dead");
		rcp_array_append(ctx->dead, (rcp_data_ref)&con);
	}
}

void rcp_context_send_data(rcp_context_ref ctx, 
		rcp_type_ref type, rcp_data_ref data)
{
	rcp_tree_node_ref node = rcp_tree_begin(ctx->connections);
	if (!node)
		return;

	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_cluster_set_data(cls, type, data);

	while (node){
		rcp_connection_ref con = 
			*(rcp_connection_ref*)rcp_tree_node_data(node);
		rcp_connection_send(con);
		node = rcp_tree_node_next(node);
		rcp_context_test_and_kill(ctx, con);
	}
	rcp_sender_cluster_clean_up(cls);
	rcp_context_clean_dead(ctx);
}
/*
void rcp_connection_send_data(
		rcp_context_ref ctx, rcp_connection_ref con,
		rcp_type_ref type, rcp_data_ref data)
{
	if (!con)
		return;

	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_cluster_set_data(cls, type, data);
	rcp_connection_send(con);
	if (ctx)
		rcp_context_test_and_kill(ctx, con);
	rcp_sender_cluster_clean_up(cls);
}
*/
void rcp_context_add_connection(rcp_context_ref ctx, 
		rcp_connection_ref con)
{
	rcp_connection_set_context(con, ctx);

	//add connection to context
	rcp_tree_node_ref node= rcp_tree_node_new(sizeof con);
	*(rcp_connection_ref*)rcp_tree_node_data(node) = con;
	rcp_tree_add(ctx->connections, node);

	//send add user command to context
	struct cmd_add_user cmd;
	rcp_type_ref type_cmd= rcp_command_type(CMD_ADD_USER);
	rcp_init(type_cmd, (rcp_data_ref)&cmd);
	cmd.command = rcp_string_new_rec(CMD_STR_ADD_USER);
	cmd.username = rcp_connection_username(con);
	rcp_record_retain(cmd.username);
	rcp_context_send_data(ctx,
			type_cmd, (rcp_data_ref)&cmd);
	rcp_deinit(type_cmd, (rcp_data_ref)&cmd);
	rcp_info("ctx:add connection");
}

void rcp_context_remove_connection(rcp_context_ref ctx,
		rcp_connection_ref con){
	rcp_info("ctx:remove connection");
	if (con)
		rcp_array_append(ctx->dead, (rcp_data_ref)&con);
	rcp_context_clean_dead(ctx);
}

void rcp_context_clean_dead(rcp_context_ref ctx)
{
	while (rcp_array_count(ctx->dead)){
		rcp_connection_ref con = 0;
		rcp_array_pop(ctx->dead, &con);
		rcp_tree_node_ref node = rcp_tree_find(ctx->connections, &con);
		if (!node){
			rcp_error("con to rm from ctx is missing");
			return;
		}
		rcp_tree_verify(ctx->connections);
		rcp_tree_remove(ctx->connections, node);
		rcp_tree_node_delete(node);

		
		struct cmd_remove_user cmd;
		rcp_type_ref type_cmd = rcp_command_type(CMD_REMOVE_USER);
		rcp_init(type_cmd, (rcp_data_ref)&cmd);
		cmd.username = rcp_connection_username(con);
		rcp_record_retain(cmd.username);
		cmd.command = rcp_string_new_rec(CMD_STR_REMOVE_USER);

		rcp_context_send_data(ctx,
				type_cmd, (rcp_data_ref)&cmd);

		//rcp_connection_free(con);
	}
}

void rcp_context_page_out(rcp_context_ref ctx)
{
	rcp_connection_ref con = rcp_connection_new();
	rcp_io_ref io = con_file_io_new_wr("contexts/file");
	rcp_connection_set_io(con, io);
	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_l1_ref sender = rcp_sender_cluster_json_nt(cls);
	rcp_connection_set_sender(con, sender);

	rcp_context_send_all_data(ctx, con);	

	rcp_connection_delete(con);
}

void rcp_context_page_in(rcp_context_ref ctx)
{
	rcp_connection_ref con = rcp_connection_new();
	rcp_io_ref io = con_file_io_new_rd("contexts/file");
	rcp_connection_set_io(con, io);
	rcp_receiver_ref receiver = rcp_receiver_new(&con_nt_json_class);
	rcp_connection_set_receiver(con, receiver);

	while (rcp_connection_alive(con)){
		rcp_connection_on_receive(con);
	}

	rcp_connection_delete(con);
}

void rcp_context_send_all_con(rcp_context_ref ctx, rcp_connection_ref con)
{
	rcp_tree_node_ref node = rcp_tree_begin(ctx->connections);
	while (node){
		rcp_connection_ref con_t = 
			*(rcp_connection_ref*)rcp_tree_node_data(node);

		struct cmd_add_user cmd;
		rcp_type_ref cmd_type=rcp_command_type(CMD_ADD_USER);
		rcp_init(cmd_type, (rcp_data_ref)&cmd);

		cmd.command = rcp_string_new_rec(CMD_STR_ADD_USER);
		cmd.username = rcp_connection_username(con_t);
		rcp_record_retain(cmd.username);

		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd);

		node = rcp_tree_node_next(node);
	}
}
void rcp_context_send_all_sub_ctx(rcp_context_ref ctx, 
		rcp_connection_ref con)
{
	rcp_dict_node_ref node = rcp_dict_begin(ctx->sub_context);
	while (node){
		struct cmd_add_context cmd;
		rcp_type_ref cmd_type=rcp_command_type(CMD_ADD_CONTEXT);
		rcp_init(cmd_type, (rcp_data_ref)&cmd);

		cmd.command = rcp_string_new_rec(CMD_STR_ADD_CONTEXT);
		cmd.name = (rcp_string_ref)rcp_dict_node_key(
				rcp_str_ptr_dict, node);
		rcp_record_retain(cmd.name);

		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd);

		node = rcp_dict_node_next(node);
	}
}
void rcp_context_send_all_array_data(
		rcp_context_ref ctx, rcp_connection_ref con)
{
	rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
	rcp_array_ref tlo = (rcp_array_ref)rcp_record_data(tlo_rec);
	rcp_array_iterater_ref node = rcp_array_begin(tlo);

	struct cmd_append_value cmd;
	rcp_type_ref cmd_type=rcp_command_type(CMD_APPEND_VALUE);
	rcp_init(cmd_type, (rcp_data_ref)&cmd);
	cmd.command = rcp_string_new_rec(CMD_STR_APPEND_VALUE);

	while (node){
		rcp_record_ref rec = 
			*(rcp_record_ref*)rcp_array_iterater_data(tlo, node);
		
		rcp_string_ref type_name = rcp_type_name(rcp_record_type(rec));
		if (type_name){	
			cmd.type = rcp_record_new(rcp_string_type);
			rcp_copy(rcp_string_type,
					(rcp_data_ref)type_name,
					rcp_record_data(cmd.type));
		}
		else{
			cmd.type = NULL;
		}

		cmd.value = rec;

		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		rcp_record_release(cmd.type);	
		node = rcp_array_iterater_next(tlo, node);
	}

	cmd.value = NULL;
	rcp_deinit(cmd_type, (rcp_data_ref)&cmd);
}

/*
void rcp_context_send_all_map_data(
		rcp_context_ref ctx, rcp_connection_ref con)
{
	rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
	rcp_map_ref tlo = (rcp_map_ref)rcp_record_data(tlo_rec);
	rcp_map_node_ref node = rcp_map_begin(tlo);

	while (node){
		struct cmd_set_value cmd;
		rcp_type_ref cmd_type=rcp_command_type(CMD_SET_VALUE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd);
		cmd.command = rcp_string_new_rec(CMD_STR_SET_VALUE);
		rcp_copy(rcp_ref_type, 
				rcp_map_node_value(tlo, node),
				(rcp_data_ref)&cmd.value);
		cmd.path = rcp_record_new(rcp_int64_type);
		rcp_copy(rcp_int64_type, 
				rcp_map_node_key(tlo, node),
				rcp_record_data(cmd.path));

		rcp_context_send_struct_to(con, cmd_type, (rcp_struct_ref)&cmd);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd);

		node = rcp_map_node_next(node);
	}
}
*/
void rcp_context_send_all_data(rcp_context_ref ctx, rcp_connection_ref con)
{
	rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
	rcp_type_ref type = rcp_record_type(tlo_rec);
	//if (type == rcp_map_type)
	//	rcp_context_send_all_map_data(ctx, con);
	if (type == rcp_array_type)
		rcp_context_send_all_array_data(ctx, con);
}
void rcp_context_send_caution(rcp_connection_ref con, 
		rcp_record_ref cause, const char* reason)
{
	struct cmd_caution cmd;
	rcp_type_ref cmd_type=rcp_command_type(CMD_CAUTION);
	rcp_init(cmd_type, (rcp_data_ref)&cmd);
	cmd.command = rcp_string_new_rec(CMD_STR_CAUTION);

	cmd.cause = rcp_record_retain(cause);
	cmd.reason = rcp_string_new_rec(reason);
	
	rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
	rcp_deinit(cmd_type, (rcp_data_ref)&cmd);
}

void rcp_context_send_info(rcp_connection_ref con, 
		rcp_record_ref cause, const char* info)
{
	struct cmd_info cmd;
	rcp_type_ref cmd_type=rcp_command_type(CMD_INFO);
	rcp_init(cmd_type, (rcp_data_ref)&cmd);
	cmd.command = rcp_string_new_rec(CMD_STR_INFO);

	cmd.cause = rcp_record_retain(cause);
	cmd.info= rcp_string_new_rec(info);
	
	rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
	rcp_deinit(cmd_type, (rcp_data_ref)&cmd);
}

int64_t rcp_context_permission(rcp_context_ref ctx, rcp_string_ref username)
{
	if (!username){
		return ctx->base_permission;
	}
	if (!rcp_user_exist(rcp_string_c_str(username))){
		return ctx->base_permission;
	}

	rcp_dict_node_ref node;
	node = rcp_dict_find(ctx->permissions, (rcp_data_ref)username);
	if (!node){
		return ctx->base_permission;
	}

	int64_t pms;
	rcp_copy(rcp_uint64_type,
			rcp_dict_node_data(rcp_str_uint64_dict, node),
			(rcp_data_ref)&pms);
	return pms;
}

rcp_extern void rcp_login_root_context(
		rcp_connection_ref con)
{
	rcp_context_ref ctx = rcp_context_get(0);

	rcp_context_send_all_con(ctx, con);
	rcp_context_send_all_data(ctx, con);	
	rcp_context_send_all_sub_ctx(ctx, con);	

	rcp_context_add_connection(ctx, con);
}

rcp_extern void rcp_context_execute_command_rec(
		rcp_context_ref ctx,
		rcp_connection_ref con, rcp_record_ref cmd_rec)
{
	if (!cmd_rec || rcp_record_type(cmd_rec) != rcp_str_ref_dict){
		rcp_error("json parse err");
		return;	
	}

	rcp_dict_ref cmd = (rcp_dict_ref)rcp_record_data(cmd_rec);

	rcp_record_ref cmd_name_rec = rcp_dict_find_c_str(
			rcp_str_ref_dict, cmd, "command", rcp_string_type);
	if (!cmd_name_rec){
		rcp_context_send_caution(con, cmd_rec, 
				"missing command element");
		return;
	}
	if (rcp_record_type(cmd_name_rec) != rcp_string_type){
		rcp_context_send_caution(con, cmd_rec, 
				"type : command element");
		return;
	}
	rcp_string_ref cmd_name = (rcp_string_ref)rcp_record_data(cmd_name_rec);

	rcp_command_type_t command_type = rcp_command_from_str(
			rcp_string_c_str(cmd_name));
	
	if (command_type == CMD_INVALID){
		rcp_error(rcp_string_c_str(cmd_name));
		rcp_error("^ invalid command");
		return;
	}

	if (command_type == CMD_KILL){
		rcp_listen_end();
	}
	if (0 && command_type == CMD_LOGIN_CONTEXT){
		rcp_info("login ctx");
		rcp_context_ref ctx = rcp_context_get(0);
		if (!ctx){
			rcp_context_send_caution(con, cmd_rec, 
					"context not found");
			return;
		}
		uint64_t permission = ctx->base_permission;
		rcp_string_ref username = (rcp_string_ref)rcp_record_data(
				rcp_connection_username(con));
		if (username){
			rcp_dict_node_ref node = 
				rcp_dict_find(ctx->permissions, (rcp_data_ref)username);
			if (node){
				uint64_t user_permission;
				rcp_copy(rcp_uint64_type,
						rcp_dict_node_data(rcp_str_uint64_dict, node),
						(rcp_data_ref)&user_permission);
				permission |= user_permission;
			}
		}
		if (! (permission & RCP_PMS_LOGIN)){
			rcp_context_send_caution(con, cmd_rec, 
					"not enough permission");
			return;
		}
		
		rcp_context_send_all_con(ctx, con);
		rcp_context_send_all_data(ctx, con);	
		rcp_context_add_connection(ctx, con);
	}

	if (command_type == CMD_CREATE_USER){
		struct cmd_create_user cmd_recv;
		rcp_type_ref cmd_type = rcp_command_type(CMD_CREATE_USER);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_recv);
		rcp_dict_to_struct(rcp_str_ref_dict, cmd,
				cmd_type, (rcp_struct_ref)&cmd_recv);

		rcp_string_ref username = 
			(rcp_string_ref)rcp_record_data(cmd_recv.username);
		rcp_string_ref password = 
			(rcp_string_ref)rcp_record_data(cmd_recv.password);

		int r = rcp_user_create(rcp_string_c_str(username),
								rcp_string_c_str(password));

		rcp_deinit(cmd_type, (rcp_data_ref)&cmd_recv);
	}
	if (command_type == CMD_LOGIN_USER){
		if (rcp_connection_username(con)){
			rcp_context_send_caution(con, cmd_rec, 
					"you are already user logined");
			return;
		}
		struct cmd_login_user cmd_recv;
		rcp_type_ref cmd_type = rcp_command_type(CMD_LOGIN_USER);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_recv);
		rcp_dict_to_struct(rcp_str_ref_dict, cmd,
				cmd_type, (rcp_struct_ref)&cmd_recv);

		if (!(cmd_recv.username && cmd_recv.password)){
			rcp_context_send_caution(con, cmd_rec, 
					"Not enough parameters.");
			return;
		}

		rcp_string_ref username = 
			(rcp_string_ref)rcp_record_data(cmd_recv.username);
		rcp_string_ref password = 
			(rcp_string_ref)rcp_record_data(cmd_recv.password);

		int r = rcp_user_autenticate(rcp_string_c_str(username),
									rcp_string_c_str(password));
		if (r != 1){
			rcp_context_send_caution(con, cmd_rec, 
					"Inncorrect username and password pair.");
		}
		else{
			rcp_connection_set_username(con, cmd_recv.username);
			rcp_context_send_info(con, 
					cmd_rec, "Loggin succeed.");
		}

		rcp_deinit(cmd_type, (rcp_data_ref)&cmd_recv);
	}
	if ( 0 && command_type == CMD_LOGIN_USER){
		if (rcp_connection_username(con)){
			rcp_context_send_caution(con, cmd_rec, 
					"you are already user logined");
			return;
		}
		struct cmd_login_user cmd_recv;
		rcp_type_ref cmd_type = rcp_command_type(CMD_LOGIN_USER);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_recv);
		rcp_dict_to_struct(rcp_str_ref_dict, cmd,
				cmd_type, (rcp_struct_ref)&cmd_recv);

		if (rcp_record_type(cmd_recv.username) != rcp_string_type){
			rcp_context_send_caution(con, cmd_rec, 
					"type of username must be string");
			return;
		}

		if (rcp_record_type(cmd_recv.password) != rcp_string_type){
			rcp_context_send_caution(con, cmd_rec, 
					"type of username must be string");
			return;
		}

		rcp_string_ref username = 
			(rcp_string_ref)rcp_record_data(cmd_recv.username);
		rcp_string_ref password = 
			(rcp_string_ref)rcp_record_data(cmd_recv.password);

		int result = rcp_user_autenticate(
				rcp_string_c_str(username),
				rcp_string_c_str(password));
		if (result > 0){
			rcp_connection_set_username(con, cmd_recv.username);
		}
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd_recv);
	}

	////////////////////////////////
	// wall of context login
	if (! ctx){
		rcp_caution("missing context");
		return;	
	}
	////////////////////////////////

	if (command_type == CMD_ADD_CONTEXT){
		struct cmd_add_context cmd_recv;
		rcp_type_ref cmd_type = rcp_command_type(CMD_ADD_CONTEXT);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_recv);
		rcp_dict_to_struct(rcp_str_ref_dict, cmd,
				cmd_type, (rcp_struct_ref)&cmd_recv);

		if (!cmd_recv.name){
			rcp_context_send_caution(con, cmd_rec, 
					"Not enough parameters.");
		}

		rcp_context_ref new_ctx = rcp_context_new();

		rcp_dict_node_ref node;
		node = rcp_dict_node_new(rcp_str_ptr_dict);
		rcp_copy(rcp_string_type,
				(rcp_data_ref)rcp_record_data(cmd_recv.name),
				rcp_dict_node_key(rcp_str_ptr_dict, node));
		rcp_copy(rcp_pointer_type,
				(rcp_data_ref)&new_ctx,
				rcp_dict_node_data(rcp_str_ptr_dict, node));

		rcp_dict_set_node(ctx->sub_context, node);

		rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)&cmd_recv);

		rcp_deinit(cmd_type, (rcp_data_ref)&cmd_recv);
	}

	if (command_type == CMD_LOGIN_CONTEXT){
		struct cmd_login_context cmd_recv;
		rcp_type_ref cmd_type = rcp_command_type(CMD_LOGIN_CONTEXT);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_recv);
		rcp_dict_to_struct(rcp_str_ref_dict, cmd,
			cmd_type, (rcp_struct_ref)&cmd_recv);

		if (!cmd_recv.name){
			rcp_context_send_caution(con, cmd_rec, 
					"Not enough parameters.");
		}

		rcp_dict_node_ref node;
		node = rcp_dict_find(ctx->sub_context, 
				rcp_record_data(cmd_recv.name));

		if (!node){
			rcp_context_send_caution(con, cmd_rec, 
					"context not found");
			return;
		}

		rcp_data_ref ctx_ref = rcp_dict_node_data(
				rcp_str_ptr_dict, node);
		rcp_context_ref ctx = *(rcp_context_ref*)ctx_ref;

		rcp_string_ref username = (rcp_string_ref)rcp_record_data(
				rcp_connection_username(con));
		uint64_t pms = rcp_context_permission(ctx, username);

		if (! (pms & RCP_PMS_LOGIN)){
			rcp_context_send_caution(con, cmd_rec, 
					"not enough permission");
			return;
		}
		
		rcp_context_send_all_con(ctx, con);
		rcp_context_send_all_data(ctx, con);	
		rcp_context_send_all_sub_ctx(ctx, con);	
		rcp_context_remove_connection(rcp_connection_context(con), con);
		rcp_context_add_connection(ctx, con);
	}

	if (command_type == CMD_DUMP){
		rcp_context_page_out(ctx);
	}
	if (command_type == CMD_ADD_PERMISSION ||
			command_type == CMD_REMOVE_PERMISSION){
		struct cmd_add_permission cmd_recv;
		rcp_type_ref cmd_type = rcp_command_type(CMD_ADD_PERMISSION);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_recv);
		rcp_dict_to_struct(rcp_str_ref_dict, cmd,
			cmd_type, (rcp_struct_ref)&cmd_recv);

		uint64_t old;
		rcp_string_ref username = NULL;
		if (cmd_recv.username)
			username = (rcp_string_ref)rcp_record_data(cmd_recv.username);
		rcp_dict_node_ref node = NULL;

		if (username){
			if (!rcp_user_exist(rcp_string_c_str(username))){
				rcp_context_send_caution(con, cmd_rec, 
						"a specified user is not existed");
				return;
			}
		
			node = rcp_dict_find(ctx->permissions, (rcp_data_ref)username);
			if (!node){
				node = rcp_dict_node_new(rcp_str_uint64_dict);
				rcp_copy(rcp_string_type,
						(rcp_data_ref)username,
						rcp_dict_node_key(rcp_str_uint64_dict, node));
				uint64_t initial = 0;
				rcp_copy(rcp_uint64_type,
						(rcp_data_ref)&initial,
						rcp_dict_node_data(rcp_str_uint64_dict, node));
				rcp_dict_set_node(ctx->permissions, node);
			}

			rcp_copy(rcp_uint64_type,
					rcp_dict_node_data(rcp_str_uint64_dict, node),
					(rcp_data_ref)&old);

		}
		else{
			old = ctx->base_permission;
		}

		uint64_t new = old;

		if (command_type == CMD_ADD_PERMISSION){
			new |= RCP_PMS_LOGIN;
			new |= RCP_PMS_READ;
			new |= RCP_PMS_WRITE;
		}
		else if (command_type == CMD_REMOVE_PERMISSION){
			new &= ~RCP_PMS_LOGIN;
			new &= ~RCP_PMS_READ;
			new &= ~RCP_PMS_WRITE;
		}
		else{
			rcp_error("never reach this point");
		}

		if (username){
			rcp_assert(node, "omg");
			rcp_copy(rcp_uint64_type,
					(rcp_data_ref)&new,
					rcp_dict_node_data(rcp_str_uint64_dict, node));
		}
		else{
			ctx->base_permission = new;
		}
	}
	if (command_type == CMD_SEND_VALUE){
		struct cmd_set_value cmd_st;
		rcp_type_ref cmd_type = rcp_command_type(CMD_SEND_VALUE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_st);
		rcp_dict_to_struct(rcp_str_ref_dict, cmd,
			cmd_type, (rcp_struct_ref)&cmd_st);
		rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)&cmd_st);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd_st);
	}
	if (command_type == CMD_UNSET_VALUE){
		rcp_record_release(ctx->top_level_record);
		ctx->top_level_record = 
			rcp_array_new_rec(rcp_ref_type);
	}
	/*
	if (command_type == CMD_SET_VALUE){
		struct cmd_set_value cmd_st;
		rcp_type_ref cmd_type = rcp_command_type(CMD_SET_VALUE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_st);
		rcp_map_to_struct(cmd, cmd_type, (rcp_struct_ref)&cmd_st);

		if (! cmd_st.value)
			return;

		int64_t key = *(int64_t*)rcp_record_data(cmd_st.path);

		rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
		rcp_map_ref tlo = (rcp_map_ref)rcp_record_data(tlo_rec);
		rcp_map_node_ref node = rcp_map_node_new(tlo);
		*(int64_t*)rcp_map_node_key(tlo, node) = key; 
		rcp_copy(rcp_ref_type, (rcp_data_ref)&cmd_st.value,
				rcp_map_node_value(tlo, node));
		rcp_map_node_ref old = rcp_map_set(tlo, node);
		rcp_map_node_delete(tlo, old);

		rcp_context_send_struct(ctx, cmd_type, (rcp_struct_ref)&cmd_st);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd_st);
	}
	*/
	if (command_type == CMD_APPEND_VALUE){
		struct cmd_set_value cmd_st;
		rcp_type_ref cmd_type = rcp_command_type(CMD_APPEND_VALUE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_st);
		rcp_dict_to_struct(rcp_str_ref_dict, cmd,
			cmd_type, (rcp_struct_ref)&cmd_st);

		if (! cmd_st.value)
			return;

		if (cmd_st.type && rcp_record_type(cmd_st.type) == rcp_string_type){
			rcp_dict_node_ref node = rcp_dict_find(ctx->types,
					rcp_record_data(cmd_st.type));

			if (!node){
				rcp_context_send_caution(con, cmd_rec, 
						"No such type.");
				return;
			}

			rcp_type_ref dst_type = *(rcp_type_ref*)
				rcp_dict_node_data(rcp_str_ptr_dict, node);
			
			rcp_record_change_type(cmd_st.value, dst_type);
			if (rcp_record_type(cmd_st.value) != dst_type){
				rcp_context_send_caution(con, cmd_rec, 
						"Can not cast.");
				return;
			}
		}

		rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
		rcp_array_ref tlo = (rcp_array_ref)rcp_record_data(tlo_rec);
		rcp_array_append(tlo, &cmd_st.value);

		rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)&cmd_st);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd_st);
	}
	if (command_type == CMD_ADD_TYPE){
		struct cmd_add_type cmd_st;
		rcp_type_ref cmd_type = rcp_command_type(CMD_ADD_TYPE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_st);
		rcp_dict_to_struct(rcp_str_ref_dict, cmd,
			cmd_type, (rcp_struct_ref)&cmd_st);

		if (cmd_st.name && rcp_record_type(cmd_st.name) != rcp_string_type)
			return;

		rcp_type_ref new_type = rcp_alias_type_new(rcp_str_ref_dict);
		rcp_dict_node_ref node = rcp_dict_node_new(rcp_str_ptr_dict);
		rcp_copy(rcp_string_type,
				rcp_record_data(cmd_st.name),
				rcp_dict_node_key(rcp_str_ptr_dict, node));
		rcp_copy(rcp_pointer_type,
				(rcp_data_ref)&new_type,
				rcp_dict_node_data(rcp_str_ptr_dict, node));
		rcp_dict_set_node(ctx->types, node);

		rcp_data_ref name = rcp_new(rcp_string_type);
		rcp_copy(rcp_string_type,
				rcp_record_data(cmd_st.name),
				name);
		rcp_type_set_name(new_type, (rcp_string_ref)name);

		rcp_deinit(cmd_type, (rcp_data_ref)&cmd_st);
	}
}
