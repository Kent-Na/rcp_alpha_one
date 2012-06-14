#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_tree.h"
#include "rcp_types.h"
#include "rcp_epoll.h"
#include "rcp_connection.h"
#include "rcp_context.h"
#include "rcp_server.h"

#include "rcp_command_list.h"
#include "rcp_command.h"

#include "cmd_types.h"

struct rcp_context_core{
	rcp_record_ref top_level_record;
	rcp_tree_ref connections;

	//dead connections. Must send "removeUser" command
	//to the entire context.
	rcp_array_ref dead;
};

size_t rcp_context_size = sizeof (struct rcp_context_core);

void rcp_context_send_record(rcp_context_ref ctx, rcp_record_ref cmd);
void rcp_context_send_struct(rcp_context_ref ctx, 
		rcp_struct_type_ref type, rcp_data_ref cmd);
void rcp_context_clean_dead(rcp_context_ref ctx);

void rcp_context_init(rcp_context_ref ctx){
	ctx->top_level_record = 
		rcp_map_new_rec(rcp_uint32_type, rcp_ref_type);
	ctx->connections = rcp_tree_new(rcp_pointer_type_compare,NULL);
	ctx->dead = rcp_array_new(rcp_pointer_type);
}
void rcp_context_uninit(rcp_context_ref ctx){
	//ctx->top_level_record
	rcp_tree_delete(ctx->connections);
}

rcp_record_ref rcp_context_top_level_record(rcp_context_ref ctx){
	return ctx->top_level_record;
}

void rcp_context_add_connection(rcp_context_ref ctx, 
		rcp_connection_ref con)
{
	rcp_tree_node_ref node= rcp_tree_node_new(sizeof con);
	*(rcp_connection_ref*)rcp_tree_node_data(node) = con;
	rcp_tree_add(ctx->connections, node);

	struct cmd_add_user cmd;
	rcp_type_ref type_cmd= rcp_command_type(CMD_ADD_USER);
	rcp_init(type_cmd, (rcp_data_ref)&cmd);
	cmd.username = rcp_string_new_rec("anyone");
	cmd.command = rcp_string_new_rec(CMD_STR_ADD_USER);
	rcp_context_send_struct(ctx,
			type_cmd, (rcp_data_ref)&cmd);
	rcp_deinit(type_cmd, (rcp_data_ref)&cmd);
	rcp_info("ctx:add connection");
}

void rcp_context_remove_connection(rcp_context_ref ctx,
		rcp_connection_ref con){
	rcp_error("ctx:remove connection");
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
		rcp_tree_remove(ctx->connections, node);
		rcp_tree_node_delete(node);

		//rcp_connection_free(con);
		
		struct cmd_remove_user cmd;
		rcp_type_ref type_cmd = rcp_command_type(CMD_REMOVE_USER);
		rcp_init(type_cmd, (rcp_data_ref)&cmd);
		cmd.username = rcp_string_new_rec("anyone");
		cmd.command = rcp_string_new_rec(CMD_STR_REMOVE_USER);

		rcp_context_send_struct(ctx,
				type_cmd, (rcp_data_ref)&cmd);
	}
}

void rcp_context_send_struct(rcp_context_ref ctx, 
		rcp_struct_type_ref type, rcp_data_ref cmd)
{
	rcp_record_ref map_rec = rcp_map_new_rec(rcp_string_type, rcp_ref_type);
	rcp_map_ref map = rcp_record_data(map_rec);
	rcp_struct_to_map(type, cmd, map);
	rcp_context_send_record(ctx, map_rec);
	rcp_record_release(map_rec);
}
void rcp_context_send_record(rcp_context_ref ctx, rcp_record_ref cmd)
{
	rcp_tree_node_ref node = rcp_tree_begin(ctx->connections);
	while (node){
		rcp_connection_ref con = 
			*(rcp_connection_ref*)rcp_tree_node_data(node);
		con_json_send_record(con, cmd);
		node = rcp_tree_node_next(node);
		if (!rcp_connection_alive(con)){
			rcp_info("dead");
			rcp_context_remove_connection(ctx, con);
		}
	}
}

rcp_record_ref rcp_command_error_new(
		rcp_record_ref cause, const char* reason)
{
	struct cmd_error cmd;
	rcp_type_ref cmd_type=rcp_command_type(CMD_ERROR);
	cmd.command = rcp_string_new_rec(CMD_STR_ERROR); 
	cmd.cause = cause; 
	rcp_record_retain(cause);
	cmd.reason = rcp_string_new_rec(reason);

	rcp_record_ref map_rec = rcp_map_new_rec(
			rcp_string_type, rcp_ref_type);
	rcp_map_ref map = rcp_record_data(map_rec);
	rcp_struct_to_map(cmd_type, (rcp_data_ref)&cmd, map);
	rcp_deinit(cmd_type, (rcp_data_ref)&cmd);
	return map_rec;
}

rcp_extern void rcp_context_execute_command_rec(
		rcp_connection_ref con, rcp_record_ref cmd_rec)
{
	rcp_context_ref ctx = rcp_context_get(0);
	if (! ctx)
		rcp_error("missing context");

	if (!cmd_rec || rcp_record_type(cmd_rec) != rcp_map_type){
		rcp_error("json parse err");
		return;	
	}

	rcp_map_ref cmd = rcp_record_data(cmd_rec);

	rcp_record_ref cmd_name_rec = rcp_map_find_c_str(
			cmd, "command", rcp_string_type);
	if (!cmd_name_rec){
		rcp_error("missing command element");
		return;
	}
	rcp_map_ref cmd_name = rcp_record_data(cmd_name_rec);

	rcp_command_type_t command_type = rcp_command_from_str(
			rcp_string_c_str(cmd_name));
	
	if (command_type == CMD_INVALID){
		rcp_error("invalid command");
		return;
	}

	if (command_type == CMD_KILL){
		rcp_listen_end();
	}
	if (command_type == CMD_LOGIN_USER){
		rcp_info("login?");
	}
	if (command_type == CMD_LOGIN_CONTEXT){
		rcp_info("login ctx");

		rcp_context_add_connection(ctx, con);

		rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
		rcp_map_ref tlo = rcp_record_data(tlo_rec);
		rcp_map_node_ref node = rcp_map_begin(tlo);
	
		while (node){
			struct cmd_set_value cmd;
			rcp_type_ref cmd_type=rcp_command_type(CMD_SET_VALUE);
			rcp_init(cmd_type, (rcp_data_ref)&cmd);
			cmd.command = rcp_string_new_rec(CMD_STR_SET_VALUE);
			rcp_copy(rcp_ref_type, 
					rcp_map_node_value(tlo, node),
					(rcp_data_ref)&cmd.value);
			cmd.path = rcp_record_new(rcp_uint16_type);

			rcp_record_ref map_rec = rcp_map_new_rec(
					rcp_string_type, rcp_ref_type);
			rcp_map_ref map = rcp_record_data(map_rec);
			rcp_struct_to_map(cmd_type, (rcp_data_ref)&cmd, map);
			rcp_deinit(cmd_type, (rcp_data_ref)&cmd);

			con_json_send_record(con, map_rec);
			rcp_record_release(map_rec);

			node = rcp_map_node_next(node);
		}
	}
	if (command_type == CMD_SEND_VALUE){
		struct cmd_set_value cmd_st;
		rcp_type_ref cmd_type = rcp_command_type(CMD_SEND_VALUE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_st);
		rcp_map_to_struct(cmd, cmd_type, (rcp_data_ref)&cmd_st);
		rcp_context_send_struct(ctx, cmd_type, (rcp_data_ref)&cmd_st);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd_st);
	}
	if (command_type == CMD_SET_VALUE){
		struct cmd_set_value cmd_st;
		rcp_type_ref cmd_type = rcp_command_type(CMD_SET_VALUE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_st);
		rcp_map_to_struct(cmd, cmd_type, (rcp_data_ref)&cmd_st);

		if (! cmd_st.value)
			return;

		int64_t key = *(int64_t*)rcp_record_data(cmd_st.path);

		rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
		rcp_map_ref tlo = rcp_record_data(tlo_rec);
		rcp_map_node_ref node = rcp_map_node_new(tlo);
		*(uint32_t*)rcp_map_node_key(tlo, node) = key; 
		rcp_copy(rcp_ref_type, (rcp_data_ref)&cmd_st.value,
				rcp_map_node_value(tlo, node));
		rcp_map_node_ref old = rcp_map_set(tlo, node);
		rcp_map_node_delete(tlo, old);
	}
	if (command_type == CMD_INVALID){
		rcp_caution("invalid command");
	}
}
