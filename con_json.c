#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_command_list.h"
#include "rcp_command.h"
#include "rcp_epoll.h"
#include "rcp_connection.h"

#include "rcp_tree.h"
#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_map.h"
#include "rcp_array.h"
#include "rcp_string.h"
#include "rcp_struct.h"
#include "rcp_type_utility.h"
#include "rcp_json.h"
#include "rcp_context.h"
#include "rcp_server.h"

void con_json_send_string(
		rcp_connection_ref con,rcp_string_ref str)
{
	const char *cmd = rcp_string_c_str(str);
	size_t s = rcp_string_c_str_len(str);
	rcp_connection_send(con,(void*)cmd,s);
}

void con_json_send_record(rcp_connection_ref con, rcp_record_ref rec)
{
	rcp_string_ref cmd_str = rcp_string_new(NULL);
	rcp_json_write_record(rec, cmd_str);
	rcp_info(rcp_string_c_str(cmd_str));
	con_json_send_string(con, cmd_str);
	rcp_delete(rcp_string_type, cmd_str);
}

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
	
	rcp_context_execute_command_rec(con, cmd);
}
