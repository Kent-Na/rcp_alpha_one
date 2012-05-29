#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_command_list.h"
#include "rcp_command.h"
#include "rcp_epoll.h"
#include "rcp_connection.h"


#include "rcp_tree.h"
#include "rcp_type.h"
#include "rcp_type_number.h"
#include "rcp_type_etc.h"
#include "rcp_map.h"
#include "rcp_string.h"
#include "rcp_string_map.h"
#include "rcp_struct.h"
#include "rcp_json.h"
#include "rcp_context.h"
#include "rcp_server.h"

void con_json_send_error(
		rcp_connection_ref con)
{
	const char *cmd = "{\"command\":\"error\"}";
	rcp_connection_send(con,(void*)cmd,strlen(cmd));
}

rcp_extern void con_json_execute(
		rcp_connection_ref con, const char *begin, const char *end)
{
	const char *cmd_begin = (const char *)begin;
	rcp_record_ref cmd = rcp_json_parse(&cmd_begin, end);
	
	if (!cmd || rcp_record_type(cmd) != &rcp_type_map){
		rcp_error("json parse err");
		return;	
	}

	rcp_record_ref cmd_name = rcp_map_find_c_str(
			cmd, "command", &rcp_string_type);
	if (!cmd_name){
		rcp_error("missing command element");
		return;
	}

	rcp_command_type_t command_type = rcp_command_from_str(
			rcp_string_type_c_str(cmd_name));
	
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
	if (command_type == RCP_COMMAND_LOGIN_CONTEXT){
		rcp_info("login ctx");
		rcp_context_ref ctx = rcp_context_get(0);
		if (! ctx)
			rcp_error("missing context");
		rcp_context_add_connection(ctx, con);
		con_json_send_error(con);
	/*	
		rcp_map_ref tlo = (rcp_map_ref)rcp_context_top_level_record(ctx);
		rcp_map_node_ref node = rcp_map_root(tlo);
		while (node){
			Json::Value value;
			value["command"] = RCP_COMMAND_STR_ADD_RECORD;
			value["recordID"] = *(uint32_t*)rcp_map_node_key(tlo, node);
			value["value"] = con_json_convert_to_json(
					rcp_map_node_value_type(tlo, node),
					rcp_map_node_value(tlo, node));

			con_json_send_json(con, value);

			node = rcp_map_node_next(node);
		}
		*/
	}
	if (command_type == RCP_COMMAND_ADD_RECORD){
		rcp_info("add?");

		rcp_record_ref value = rcp_map_find_c_str(
				cmd, "value", NULL);
		if (! value)
			return;

		rcp_record_ref rec_id_rec = 
			rcp_map_find_c_str(cmd, "recordID", &rcp_int64_type);
		if (! rec_id_rec)
			return;
		int64_t recID = *(int64_t*)rcp_record_data(rec_id_rec);

		rcp_context_ref ctx = rcp_context_get(0);
		if (! ctx)
			rcp_error("missing context");

		rcp_map_ref tlo = (rcp_map_ref)rcp_context_top_level_record(ctx);
		rcp_map_node_ref node = rcp_map_node_new(tlo);
		*(uint32_t*)rcp_map_node_key(tlo, node) = recID; 
		rcp_copy(&rcp_ref_type, &value, rcp_map_node_value(tlo, node));
		rcp_map_set(tlo, node);
		//rcp_context_add_record(ctx, rec);
	}
	if (command_type == RCP_COMMAND_INVALID){
		rcp_caution("invalid command");
	}
}
