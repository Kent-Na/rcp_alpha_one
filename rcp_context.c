#include "rcp_pch.h"
#include "rcp_utility.h"

#include "rcp_server.h"

#include "rcp_connection.h"

#include "rcp_command.h"

#include "cmd_list.h"
#include "cmd_types.h"

#include "connections/con_file.h"
#include "rcp_receiver.h"
#include "rcp_sender_classes.h"
#include "rcp_receiver_classes.h"

#include "rcp_tree.h"

#include "rcp_user.h"

#include "rcp_type.h"
#include "types/rcp_type_utility.h"
#include "rcp_record.h"
#include "types/rcp_dict.h"
#include "types/rcp_dict_list.h"
#include "types/rcp_array.h"
#include "types/rcp_struct.h"
#include "types/rcp_string.h"
#include "types/rcp_number.h"
#include "types/rcp_type_list.h"
#include "types/rcp_alias.h"

#include "rcp_caster.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_context.h"


//size_t rcp_context_size = sizeof (struct rcp_context_core);

void rcp_context_send_record(rcp_context_ref ctx, rcp_record_ref cmd);
void rcp_context_send_data(rcp_context_ref ctx, 
		rcp_type_ref type, rcp_data_ref cmd);

void rcp_context_clean_dead(rcp_context_ref ctx);

rcp_extern rcp_context_ref rcp_context_new()
{
	rcp_context_ref ctx = malloc(sizeof *ctx);
	rcp_context_init(ctx);
	return ctx;
}
void rcp_context_init(rcp_context_ref ctx)
{
	ctx->top_level_record = NULL;
	ctx->connections = rcp_tree_new((void*)rcp_pointer_compare,NULL);
	ctx->dead = rcp_array_new(rcp_pointer_type);
	ctx->permissions = rcp_dict_new(rcp_str_uint64_dict);
	ctx->base_permission = RCP_PMS_LOGIN | RCP_PMS_READ | RCP_PMS_WRITE
		|RCP_PMS_PMS|RCP_PMS_CTX;
	ctx->types = rcp_dict_new(rcp_str_ptr_dict);
	ctx->parent_context = NULL;
	ctx->sub_context = rcp_dict_new(rcp_str_ptr_dict);
	ctx->state = 0;
	ctx->id = 0;
}
void rcp_context_uninit(rcp_context_ref ctx)
{
	rcp_record_release(ctx->top_level_record);
	rcp_tree_delete(ctx->connections);
	rcp_array_delete(ctx->dead);
	rcp_dict_delete(rcp_str_uint64_dict, ctx->permissions);
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
		rcp_array_append_data(ctx->dead, (rcp_data_ref)&con);
		rcp_context_clean_dead(ctx);
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
	}
	rcp_sender_cluster_clean_up(cls);
	rcp_context_clean_dead(ctx);
}

rcp_extern
void rcp_connection_set_context(
		rcp_connection_ref con, rcp_context_ref ctx);
rcp_extern
void rcp_connection_unset_context(
		rcp_connection_ref con);

void rcp_context_add_connection(rcp_context_ref ctx, 
		rcp_connection_ref con)
{
	rcp_connection_retain(con);
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
		rcp_array_append_data(ctx->dead, (rcp_data_ref)&con);
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

		rcp_connection_unset_context(con);
		rcp_connection_release(con);

		struct cmd_remove_user cmd;
		rcp_type_ref type_cmd = rcp_command_type(CMD_REMOVE_USER);
		rcp_init(type_cmd, (rcp_data_ref)&cmd);
		cmd.username = rcp_connection_username(con);
		rcp_record_retain(cmd.username);
		cmd.command = rcp_string_new_rec(CMD_STR_REMOVE_USER);

		rcp_context_send_data(ctx,
				type_cmd, (rcp_data_ref)&cmd);
		rcp_deinit(type_cmd, (rcp_data_ref)&cmd);
	}
}

void rcp_context_disconnect_all(
		rcp_context_ref ctx,
		const char* reason)
{
	struct cmd_error cmd;
	rcp_type_ref cmd_type=rcp_command_type(CMD_ERROR);
	rcp_init(cmd_type, (rcp_data_ref)&cmd);
	cmd.command = rcp_string_new_rec(CMD_STR_ERROR);
	cmd.reason = rcp_string_new_rec(reason);
	rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)&cmd);

	rcp_tree_node_ref node = rcp_tree_begin(ctx->connections);
	//delete all connection
	while (node){
		rcp_connection_ref con;
		con = *(rcp_connection_ref*)rcp_tree_node_data(node);
		rcp_connection_release(con);		
		node = rcp_tree_node_next(node);
		rcp_array_append_data(ctx->dead, (rcp_data_ref)&con);
	}
	rcp_context_clean_dead(ctx);
}

rcp_extern rcp_context_state_t rcp_context_state_flag(
		rcp_context_ref ctx)
{
	return ctx->state;
}
rcp_extern void rcp_context_set_state_flag(
		rcp_context_ref ctx,
		rcp_context_state_t state)
{
	ctx->state |= state;
}
rcp_extern void rcp_context_unset_state_flag(
		rcp_context_ref ctx,
		rcp_context_state_t state)
{
	ctx->state &= ~state;
}

void rcp_page_out_r(rcp_context_ref ctx)
{
	rcp_dict_node_ref node = rcp_dict_begin(ctx->sub_context);
	while (node){
		rcp_data_ref ctx_ref = rcp_dict_node_data(
				rcp_str_ptr_dict, node);
		rcp_context_ref new_ctx = *(rcp_context_ref*)ctx_ref;
		rcp_page_out_r(new_ctx);
		node = rcp_dict_node_next(node);
	}
	rcp_context_store(ctx);
}

void rcp_page_in_r(rcp_context_ref ctx)
{
	rcp_context_load_sub_contexts(ctx);

	rcp_dict_node_ref node = rcp_dict_begin(ctx->sub_context);
	while (node){
		rcp_data_ref ctx_ref = rcp_dict_node_data(
				rcp_str_ptr_dict, node);
		rcp_context_ref new_ctx = *(rcp_context_ref*)ctx_ref;
		rcp_page_in_r(new_ctx);
		node = rcp_dict_node_next(node);
	}
	rcp_context_load(ctx);
}

void rcp_context_store_into_io(rcp_context_ref ctx, rcp_io_ref io)
{
	rcp_connection_ref con = rcp_connection_new();
	rcp_connection_set_io(con, io);
	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_l1_ref sender = rcp_sender_cluster_json_nt(cls);
	rcp_connection_set_sender(con, sender);

	rcp_context_send_all_data(ctx, con);	
	rcp_context_send_all_permission(ctx, con);	

	rcp_connection_release(con);
}

void rcp_context_load_from_io(rcp_context_ref ctx, rcp_io_ref io)
{
	rcp_connection_ref con = rcp_connection_new();
	rcp_connection_set_io(con, io);

	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_l1_ref sender = rcp_sender_cluster_dummy(cls);
	rcp_connection_set_sender(con, sender);

	rcp_receiver_ref receiver = rcp_receiver_new(&con_nt_json_class);
	rcp_connection_set_receiver(con, receiver);

	rcp_record_ref protocol_ver = rcp_string_new_rec("alpha1");
	rcp_connection_open(con, protocol_ver, NULL);
	rcp_record_release(protocol_ver);

	rcp_connection_set_permission(con, 
			RCP_PMS_WRITE|RCP_PMS_PMS|RCP_PMS_CTX);

	rcp_context_add_connection(ctx, con);

	while (rcp_connection_alive(con)){
		rcp_connection_on_receive(con);
	}
	rcp_connection_release(con);
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
		cmd.name = rcp_record_new_with(
				rcp_string_type,
				rcp_dict_node_key(rcp_str_ptr_dict, node));

		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd);

		node = rcp_dict_node_next(node);
	}
}
void rcp_context_send_all_permission(rcp_context_ref ctx, 
		rcp_connection_ref con)
{
	rcp_dict_node_ref node = rcp_dict_begin(ctx->permissions);

	{
		struct cmd_set_permission cmd;
		rcp_type_ref cmd_type=rcp_command_type(CMD_SET_PERMISSION);
		rcp_init(cmd_type, (rcp_data_ref)&cmd);

		cmd.command = rcp_string_new_rec(CMD_STR_SET_PERMISSION);
		rcp_permission_t pms = ctx->base_permission;
		cmd.mode = rcp_permission_to_array(pms); 

		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd);
	}

	while (node){
		struct cmd_set_permission cmd;
		rcp_type_ref cmd_type=rcp_command_type(CMD_SET_PERMISSION);
		rcp_init(cmd_type, (rcp_data_ref)&cmd);

		cmd.command = rcp_string_new_rec(CMD_STR_SET_PERMISSION);
		cmd.username = rcp_record_new_with(
				rcp_string_type,
				rcp_dict_node_key(rcp_str_uint64_dict, node));
		rcp_permission_t pms;
		pms = *(rcp_permission_t*)rcp_dict_node_data(
				rcp_str_uint64_dict, node);
		cmd.mode = rcp_permission_to_array(pms); 

		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd);

		node = rcp_dict_node_next(node);
	}
}
void rcp_context_send_all_data(rcp_context_ref ctx, rcp_connection_ref con)
{
	rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
	if (!tlo_rec)
		return;

	rcp_type_ref type = rcp_record_type(tlo_rec);
	rcp_data_ref data = rcp_record_data(tlo_rec);
	rcp_send_as_command(type, data, con);
}
void rcp_context_send_error(rcp_connection_ref con, 
		rcp_record_ref cause, const char* reason)
{
	struct cmd_error cmd;
	rcp_type_ref cmd_type=rcp_command_type(CMD_ERROR);
	rcp_init(cmd_type, (rcp_data_ref)&cmd);
	cmd.command = rcp_string_new_rec(CMD_STR_ERROR);

	cmd.cause = rcp_record_retain(cause);
	cmd.reason = rcp_string_new_rec(reason);
	
	rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
	rcp_deinit(cmd_type, (rcp_data_ref)&cmd);

	rcp_connection_close(con);
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

void rcp_context_set_permission(
		rcp_context_ref ctx, rcp_string_ref username, int64_t pms)
{
	if (!username){
		ctx->base_permission = pms;
		return;
	}
	if (!rcp_user_exist(rcp_string_c_str(username))){
		return;
	}

	rcp_dict_node_ref node;
	node = rcp_dict_find(ctx->permissions, (rcp_data_ref)username);
	if (node){
		rcp_copy(rcp_uint64_type,
				(rcp_data_ref)&pms,
				rcp_dict_node_data(rcp_str_uint64_dict, node));
	}
	else{
		node = rcp_dict_node_new_with(rcp_str_uint64_dict,
				(rcp_data_ref)username,
				(rcp_data_ref)&pms);
		rcp_dict_set_node(ctx->permissions, node);
		node = rcp_dict_find(ctx->permissions, (rcp_data_ref)username);
		if (!node)
			rcp_error("omg.");
	}
}

void rcp_context_unset_permission(
		rcp_context_ref ctx, rcp_string_ref username)
{
	if (!username)
		return;
	rcp_dict_node_ref node;
	node = rcp_dict_find(ctx->permissions, (rcp_data_ref)username);
	if (!node)
		return;
	rcp_dict_unset_node(ctx->permissions, node);
}

rcp_extern void rcp_login_root_context(
		rcp_connection_ref con)
{
	rcp_context_ref ctx = rcp_root_context();

	rcp_context_send_all_con(ctx, con);
	rcp_context_send_all_data(ctx, con);	
	rcp_context_send_all_sub_ctx(ctx, con);	

	rcp_context_add_connection(ctx, con);
}

void rcp_context_add_context(rcp_context_ref ctx, 
		rcp_string_ref name, rcp_context_ref new_ctx)
{
	new_ctx->parent_context = ctx;
	rcp_dict_node_ref node;
	node = rcp_dict_node_new_with(rcp_str_ptr_dict,
			(rcp_data_ref)name,
			(rcp_data_ref)&new_ctx);
	node = rcp_dict_set_node(ctx->sub_context, node);
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

	rcp_command_ref cmd_info = rcp_command_from_str(
			rcp_string_c_str(cmd_name));
	
	if (! cmd_info){
		rcp_context_send_caution(con, cmd_rec, 
				"invalid command");
		rcp_error(rcp_string_c_str(cmd_name));
		rcp_error("^ invalid command");
		return;
	}

	if (cmd_info->cmd != CMD_OPEN && ! rcp_connection_is_open(con)){
		rcp_context_send_error(con, cmd_rec, 
				"must send open command.");
		return;
	}

	rcp_permission_t con_pms = rcp_connection_permission(con);
	if ((cmd_info->cmd_pms & con_pms) != cmd_info->cmd_pms){
		rcp_context_send_caution(con, cmd_rec, 
				"not enough permission.");
		return;
	}

	rcp_type_ref cmd_type = cmd_info->cmd_type;
	rcp_data_ref cmd_recv = rcp_new(cmd_type);
	rcp_dict_to_struct(rcp_str_ref_dict, cmd,
			cmd_type, (rcp_struct_ref)cmd_recv);

	cmd_info->cmd_impl(ctx, con, cmd_rec, cmd_type, cmd_recv);

	rcp_delete(cmd_type, cmd_recv);
}
