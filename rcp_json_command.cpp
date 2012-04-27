#include <rcp_pch.h>

void rcp_execute_json_command(unsigned char* cmd){

}

static rcp_execute_command(const Json::Value &json){
	std::string command_name;
	if (not command.getMemberAsString("command", command_name)){
		printf("err : command name not found\n");
	}

	rcp_command_t command = rcp_command_from_str(command_name.c_str());	

}
