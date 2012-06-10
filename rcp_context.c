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
};

size_t rcp_context_size = sizeof (struct rcp_context_core);

void rcp_context_init(rcp_context_ref ctx){
	ctx->top_level_record = 
		rcp_map_new_rec(rcp_uint32_type, rcp_ref_type);
	ctx->connections = rcp_tree_new(rcp_pointer_type_compare,NULL);
}
void rcp_context_uninit(rcp_context_ref ctx){
	//ctx->top_level_record
	rcp_tree_delete(ctx->connections);
}

rcp_record_ref rcp_context_top_level_record(rcp_context_ref ctx){
	return ctx->top_level_record;
}

void rcp_context_add_connection(rcp_context_ref ctx, 
		rcp_connection_ref con){
	rcp_tree_node_ref node= rcp_tree_node_new(sizeof con);
	*(rcp_connection_ref*)rcp_tree_node_data(node) = con;
	rcp_tree_add(ctx->connections, node);
}

void rcp_context_remove_connection(rcp_context_ref ctx,
		rcp_connection_ref con){
	rcp_tree_node_ref node = rcp_tree_find(ctx->connections, con);
	if (node)
		rcp_tree_remove(ctx->connections, node);
	else
		rcp_error("missing connection");
}

rcp_record_ref rcp_command_error_new(
		rcp_record_ref cause, const char* reason)
{
	struct cmd_error cmd;
	rcp_type_ref cmd_type=rcp_command_type(RCP_COMMAND_ERROR);
	cmd.command = rcp_string_new_rec(RCP_COMMAND_STR_ERROR); 
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
	
	if (command_type == RCP_COMMAND_INVALID){
		rcp_error("invalid command");
		return;
	}

	if (command_type == RCP_COMMAND_KILL){
		rcp_listen_end();
	}
	if (command_type == RCP_COMMAND_LOGIN_USER){
		rcp_info("login?");
	}
	if (command_type == RCP_COMMAND_LOGIN_CONTEXT || 
			command_type == RCP_COMMAND_PROTOCOL){
		rcp_info("login ctx");

		rcp_context_ref ctx = rcp_context_get(0);
		rcp_context_add_connection(ctx, con);
		con_json_send_error(con);
		rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
		rcp_map_ref tlo = rcp_record_data(tlo_rec);
		rcp_map_node_ref node = rcp_map_begin(tlo);
	
		while (node){
			rcp_record_ref r_cmd_rec = 
				rcp_map_new_rec(rcp_string_type, rcp_ref_type);
			rcp_map_ref r_cmd = rcp_record_data(r_cmd_rec);
			struct cmd_set_value cmd;
			cmd.command = rcp_string_new_rec("")
			{
				rcp_map_node_ref r_node = rcp_map_node_new(r_cmd);
				rcp_string_init_with_c_str(
						rcp_map_node_key(r_cmd, r_node),"command");
				rcp_record_ref val = rcp_string_new_rec(
						RCP_COMMAND_STR_SET_VALUE);
				rcp_copy(rcp_ref_type, 
						(rcp_data_ref)&val, 
						rcp_map_node_value(r_cmd, r_node));
				rcp_map_set(r_cmd, r_node);
				rcp_record_release(val);
			}
			{
				rcp_map_node_ref r_node= rcp_map_node_new(r_cmd);
				rcp_string_init_with_c_str(
						rcp_map_node_key(r_cmd, r_node),"value");
				rcp_copy(rcp_ref_type, 
						rcp_map_node_value(tlo, node),
						rcp_map_node_value(r_cmd, r_node));
				rcp_map_set(r_cmd, r_node);
			}
			{
				rcp_map_node_ref r_node = rcp_map_node_new(r_cmd);
				rcp_string_init_with_c_str(
						rcp_map_node_key(r_cmd, r_node),"path");
				rcp_record_ref val = rcp_record_new(rcp_uint16_type);
				rcp_copy(rcp_ref_type, 
						(rcp_data_ref)&val,
						rcp_map_node_value(r_cmd, r_node));
				rcp_map_set(r_cmd, r_node);
				rcp_record_release(val);
			}

			con_json_send_record(con, r_cmd_rec);
			rcp_record_release(r_cmd_rec);

			node = rcp_map_node_next(node);
		}
	}
	if (command_type == RCP_COMMAND_SET_VALUE){
		rcp_record_ref value = rcp_map_find_c_str(
				cmd, "value", NULL);
		if (! value)
			return;

		rcp_record_ref path = 
			rcp_map_find_c_str(cmd, "path", rcp_int64_type);

		int64_t key = *(int64_t*)rcp_record_data(path);

		rcp_context_ref ctx = rcp_context_get(0);
		if (! ctx)
			rcp_error("missing context");

		rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
		rcp_map_ref tlo = rcp_record_data(tlo_rec);
		rcp_map_node_ref node = rcp_map_node_new(tlo);
		*(uint32_t*)rcp_map_node_key(tlo, node) = key; 
		rcp_copy(rcp_ref_type, (rcp_data_ref)&value,
				rcp_map_node_value(tlo, node));
		rcp_map_set(tlo, node);
	}
	if (command_type == RCP_COMMAND_ADD_RECORD){
		rcp_info("add?");

		rcp_record_ref value = rcp_map_find_c_str(
				cmd, "value", NULL);
		if (! value)
			return;

		rcp_record_ref rec_id_rec = 
			rcp_map_find_c_str(cmd, "recordID", rcp_int64_type);
		if (! rec_id_rec)
			return;
		int64_t recID = *(int64_t*)rcp_record_data(rec_id_rec);

		rcp_context_ref ctx = rcp_context_get(0);
		if (! ctx)
			rcp_error("missing context");

		rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);
		rcp_map_ref tlo = rcp_record_data(tlo_rec);
		rcp_map_node_ref node = rcp_map_node_new(tlo);
		*(uint32_t*)rcp_map_node_key(tlo, node) = recID; 
		rcp_copy(rcp_ref_type, (rcp_data_ref)&value,
				rcp_map_node_value(tlo, node));
		rcp_map_set(tlo, node);
		//rcp_context_add_record(ctx, rec);
	}
	if (command_type == RCP_COMMAND_INVALID){
		rcp_caution("invalid command");
	}
}
