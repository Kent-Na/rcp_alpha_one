
#include "json/json.h"
#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_command_list.h"
#include "rcp_command.h"
#include "rcp_connection.h"
rcp_extern void con_json_cpp_execute(
		rcp_connection_ref con, const char *begin, const char *end)
{
	Json::Value command;
	bool success = Json::Reader().parse(begin, end, command, false);
	
	if (not success || not command.isObject()){
		rcp_error("json parse err");
		return;	
	}

	std::string command_type_str;
	if (not command.getMemberAsString("command",command_type_str)){
		rcp_error("json missing element\"command\"");
		return;
	}

	rcp_command_type_t command_type = rcp_command_from_str(
			command_type_str.c_str());
	
	if (command_type == RCP_COMMAND_INVALID){
		rcp_error("invalid command");
		return;
	}

	if (command_type == RCP_COMMAND_LOGIN_USER){
		rcp_error("login?");
	}
}
