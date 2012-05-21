
#include "json/json.h"
#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_command_list.h"
#include "rcp_command.h"
#include "rcp_epoll.h"
#include "rcp_connection.h"

#include "rcp_tree.h"
#include "rcp_string.h"
#include "rcp_type.h"
#include "rcp_struct.h"
#include "rcp_type_list.h"
#include "rcp_context.h"
#include "rcp_server.h"

rcp_extern void con_json_cpp_send_error(
		rcp_connection_ref con, const char *error)
{

}

void con_json_cpp_convert_to_record(Json::Value &value, rcp_record_ref rec){
	if (rec == NULL)
		return;
	rcp_type_ref type = rcp_record_type(rec);
	if (type == &rcp_type_uint32){
		if (not value.isUInt64()){
			rcp_error("type error");
			return;
		}
		uint32_t *val = (uint32_t*)rcp_record_data(rec);
		*val = value.asUInt64();	
	}
	if (type == &rcp_type_string){
		if (not value.isString()){
			rcp_error("type error");
			return;
		}
		rcp_string_ref *val = (rcp_string_ref*)rcp_record_data(rec);
		*val = rcp_string_new(value.asCString());	
	}
}

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

	if (command_type == RCP_COMMAND_KILL){
		rcp_listen_end();
	}
	if (command_type == RCP_COMMAND_LOGIN_USER){
		rcp_info("login?");
	}
	if (command_type == RCP_COMMAND_LOGIN_CONTEXT){
		rcp_info("login ctx");
		rcp_context_ref ctx = rcp_context_get(0);
		if (not ctx)
			rcp_error("missing context");
		rcp_context_add_connection(ctx, con);
	}
	if (command_type == RCP_COMMAND_ADD_RECORD){
		rcp_info("add?");
		std::string type_str;
		if (not command.getMemberAsString("type",type_str))
			return;
		rcp_type_ref type = &rcp_type_uint32;
			//; = rcp_type_from_string(type_str.c_str());
		if (not type)
			return;
		rcp_record_ref rec = rcp_record_new(type);
		Json::Value value = command["value"];
		con_json_cpp_convert_to_record(value, rec);
		rcp_context_ref ctx = rcp_context_get(0);
		if (not ctx)
			rcp_error("missing context");
		//rcp_context_add_record(ctx, rec);
	}
}
/*
loid f(Json::Value json){
	//dict
	rcp_structure_ref st;
	rcp_record_ref rec = rcp_record_new(st);

	auto itr = json.begin();

	for (auto itr = json.begin(); itr<json.end(); itr++){
		auto key = itr.key();
		if (not key.isString()){
			rcp_error("key is not string");
			continue;
		}
		auto key_str = key.asCString();
		auto json_param = rcp_parameter_from_str(st, key_str);
		if (param == NULL){
			rcp_caution("undefined key");
			continue;
		}
		auto rcp_parameter_ptr(rec, param);
		f(json_param, rec, param);
	}
}

void f(Json::Value json, rcp_record_ref rec, rcp_parameter_ref param){
	if (param.type == string){

	}

}

//string
void f(Json::Value json, rcp_record_ref rec, rcp_parameter_ref param){
	if (not json.isString()){
		rcp_error("parameter type error");
		continue;
	}
	rcp_string_ref *dst = rec + rcp_parameter_offset(param);
	*dst = rcp_string_new(json.asCString);
}
*/
