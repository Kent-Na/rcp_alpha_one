//This file was generated by program.
#include "rcp_pch.h"
#include "rcp_utility.h"
#include "cmd_impl.h"
#include "cmd_list.h"
#include "cmd_types_func.h"
#include "rcp_command.h"
struct rcp_command_core rcp_command_table[28];

void rcp_command_table_init(){

	rcp_command_table[CMD_OPEN].cmd = 
		CMD_OPEN;
	rcp_command_table[CMD_OPEN].cmd_str = 
		"open";
	rcp_command_table[CMD_OPEN].cmd_pms = 
		0;
	rcp_command_table[CMD_OPEN].cmd_type= 
		cmd_open_type();
	rcp_command_table[CMD_OPEN].cmd_impl= 
		cmd_impl_open;

	rcp_command_table[CMD_KILL].cmd = 
		CMD_KILL;
	rcp_command_table[CMD_KILL].cmd_str = 
		"kill";
	rcp_command_table[CMD_KILL].cmd_pms = 
		0;
	rcp_command_table[CMD_KILL].cmd_type= 
		cmd_kill_type();
	rcp_command_table[CMD_KILL].cmd_impl= 
		cmd_impl_kill;

	rcp_command_table[CMD_DUMP].cmd = 
		CMD_DUMP;
	rcp_command_table[CMD_DUMP].cmd_str = 
		"dump";
	rcp_command_table[CMD_DUMP].cmd_pms = 
		0;
	rcp_command_table[CMD_DUMP].cmd_type= 
		cmd_dump_type();
	rcp_command_table[CMD_DUMP].cmd_impl= 
		cmd_impl_dump;

	rcp_command_table[CMD_LOAD].cmd = 
		CMD_LOAD;
	rcp_command_table[CMD_LOAD].cmd_str = 
		"load";
	rcp_command_table[CMD_LOAD].cmd_pms = 
		0;
	rcp_command_table[CMD_LOAD].cmd_type= 
		cmd_load_type();
	rcp_command_table[CMD_LOAD].cmd_impl= 
		cmd_impl_load;

	rcp_command_table[CMD_CLOSE].cmd = 
		CMD_CLOSE;
	rcp_command_table[CMD_CLOSE].cmd_str = 
		"close";
	rcp_command_table[CMD_CLOSE].cmd_pms = 
		0;
	rcp_command_table[CMD_CLOSE].cmd_type= 
		cmd_close_type();
	rcp_command_table[CMD_CLOSE].cmd_impl= 
		cmd_impl_close;

	rcp_command_table[CMD_PING].cmd = 
		CMD_PING;
	rcp_command_table[CMD_PING].cmd_str = 
		"ping";
	rcp_command_table[CMD_PING].cmd_pms = 
		0;
	rcp_command_table[CMD_PING].cmd_type= 
		cmd_ping_type();
	rcp_command_table[CMD_PING].cmd_impl= 
		cmd_impl_ping;

	rcp_command_table[CMD_PONG].cmd = 
		CMD_PONG;
	rcp_command_table[CMD_PONG].cmd_str = 
		"pong";
	rcp_command_table[CMD_PONG].cmd_pms = 
		0;
	rcp_command_table[CMD_PONG].cmd_type= 
		cmd_pong_type();
	rcp_command_table[CMD_PONG].cmd_impl= 
		cmd_impl_pong;

	rcp_command_table[CMD_CREATE_USER].cmd = 
		CMD_CREATE_USER;
	rcp_command_table[CMD_CREATE_USER].cmd_str = 
		"createUser";
	rcp_command_table[CMD_CREATE_USER].cmd_pms = 
		0;
	rcp_command_table[CMD_CREATE_USER].cmd_type= 
		cmd_create_user_type();
	rcp_command_table[CMD_CREATE_USER].cmd_impl= 
		cmd_impl_create_user;

	rcp_command_table[CMD_DELETE_USER].cmd = 
		CMD_DELETE_USER;
	rcp_command_table[CMD_DELETE_USER].cmd_str = 
		"deleteUser";
	rcp_command_table[CMD_DELETE_USER].cmd_pms = 
		0;
	rcp_command_table[CMD_DELETE_USER].cmd_type= 
		cmd_delete_user_type();
	rcp_command_table[CMD_DELETE_USER].cmd_impl= 
		cmd_impl_delete_user;

	rcp_command_table[CMD_LOGIN_USER].cmd = 
		CMD_LOGIN_USER;
	rcp_command_table[CMD_LOGIN_USER].cmd_str = 
		"loginUser";
	rcp_command_table[CMD_LOGIN_USER].cmd_pms = 
		0;
	rcp_command_table[CMD_LOGIN_USER].cmd_type= 
		cmd_login_user_type();
	rcp_command_table[CMD_LOGIN_USER].cmd_impl= 
		cmd_impl_login_user;

	rcp_command_table[CMD_ADD_USER].cmd = 
		CMD_ADD_USER;
	rcp_command_table[CMD_ADD_USER].cmd_str = 
		"addUser";
	rcp_command_table[CMD_ADD_USER].cmd_pms = 
		0;
	rcp_command_table[CMD_ADD_USER].cmd_type= 
		cmd_add_user_type();
	rcp_command_table[CMD_ADD_USER].cmd_impl= 
		cmd_impl_add_user;

	rcp_command_table[CMD_REMOVE_USER].cmd = 
		CMD_REMOVE_USER;
	rcp_command_table[CMD_REMOVE_USER].cmd_str = 
		"removeUser";
	rcp_command_table[CMD_REMOVE_USER].cmd_pms = 
		0;
	rcp_command_table[CMD_REMOVE_USER].cmd_type= 
		cmd_remove_user_type();
	rcp_command_table[CMD_REMOVE_USER].cmd_impl= 
		cmd_impl_remove_user;

	rcp_command_table[CMD_SET_PERMISSION].cmd = 
		CMD_SET_PERMISSION;
	rcp_command_table[CMD_SET_PERMISSION].cmd_str = 
		"setPermission";
	rcp_command_table[CMD_SET_PERMISSION].cmd_pms = 
		8;
	rcp_command_table[CMD_SET_PERMISSION].cmd_type= 
		cmd_set_permission_type();
	rcp_command_table[CMD_SET_PERMISSION].cmd_impl= 
		cmd_impl_set_permission;

	rcp_command_table[CMD_UNSET_PERMISSION].cmd = 
		CMD_UNSET_PERMISSION;
	rcp_command_table[CMD_UNSET_PERMISSION].cmd_str = 
		"unsetPermission";
	rcp_command_table[CMD_UNSET_PERMISSION].cmd_pms = 
		8;
	rcp_command_table[CMD_UNSET_PERMISSION].cmd_type= 
		cmd_unset_permission_type();
	rcp_command_table[CMD_UNSET_PERMISSION].cmd_impl= 
		cmd_impl_unset_permission;

	rcp_command_table[CMD_ADD_CONTEXT].cmd = 
		CMD_ADD_CONTEXT;
	rcp_command_table[CMD_ADD_CONTEXT].cmd_str = 
		"addContext";
	rcp_command_table[CMD_ADD_CONTEXT].cmd_pms = 
		16;
	rcp_command_table[CMD_ADD_CONTEXT].cmd_type= 
		cmd_add_context_type();
	rcp_command_table[CMD_ADD_CONTEXT].cmd_impl= 
		cmd_impl_add_context;

	rcp_command_table[CMD_REMOVE_CONTEXT].cmd = 
		CMD_REMOVE_CONTEXT;
	rcp_command_table[CMD_REMOVE_CONTEXT].cmd_str = 
		"removeContext";
	rcp_command_table[CMD_REMOVE_CONTEXT].cmd_pms = 
		16;
	rcp_command_table[CMD_REMOVE_CONTEXT].cmd_type= 
		cmd_remove_context_type();
	rcp_command_table[CMD_REMOVE_CONTEXT].cmd_impl= 
		cmd_impl_remove_context;

	rcp_command_table[CMD_LOGIN_CONTEXT].cmd = 
		CMD_LOGIN_CONTEXT;
	rcp_command_table[CMD_LOGIN_CONTEXT].cmd_str = 
		"loginContext";
	rcp_command_table[CMD_LOGIN_CONTEXT].cmd_pms = 
		0;
	rcp_command_table[CMD_LOGIN_CONTEXT].cmd_type= 
		cmd_login_context_type();
	rcp_command_table[CMD_LOGIN_CONTEXT].cmd_impl= 
		cmd_impl_login_context;

	rcp_command_table[CMD_LOGOUT_CONTEXT].cmd = 
		CMD_LOGOUT_CONTEXT;
	rcp_command_table[CMD_LOGOUT_CONTEXT].cmd_str = 
		"logoutContext";
	rcp_command_table[CMD_LOGOUT_CONTEXT].cmd_pms = 
		0;
	rcp_command_table[CMD_LOGOUT_CONTEXT].cmd_type= 
		cmd_logout_context_type();
	rcp_command_table[CMD_LOGOUT_CONTEXT].cmd_impl= 
		cmd_impl_logout_context;

	rcp_command_table[CMD_SEND_VALUE].cmd = 
		CMD_SEND_VALUE;
	rcp_command_table[CMD_SEND_VALUE].cmd_str = 
		"sendValue";
	rcp_command_table[CMD_SEND_VALUE].cmd_pms = 
		0;
	rcp_command_table[CMD_SEND_VALUE].cmd_type= 
		cmd_send_value_type();
	rcp_command_table[CMD_SEND_VALUE].cmd_impl= 
		cmd_impl_send_value;

	rcp_command_table[CMD_SET_VALUE].cmd = 
		CMD_SET_VALUE;
	rcp_command_table[CMD_SET_VALUE].cmd_str = 
		"setValue";
	rcp_command_table[CMD_SET_VALUE].cmd_pms = 
		4;
	rcp_command_table[CMD_SET_VALUE].cmd_type= 
		cmd_set_value_type();
	rcp_command_table[CMD_SET_VALUE].cmd_impl= 
		cmd_impl_set_value;

	rcp_command_table[CMD_UNSET_VALUE].cmd = 
		CMD_UNSET_VALUE;
	rcp_command_table[CMD_UNSET_VALUE].cmd_str = 
		"unsetValue";
	rcp_command_table[CMD_UNSET_VALUE].cmd_pms = 
		4;
	rcp_command_table[CMD_UNSET_VALUE].cmd_type= 
		cmd_unset_value_type();
	rcp_command_table[CMD_UNSET_VALUE].cmd_impl= 
		cmd_impl_unset_value;

	rcp_command_table[CMD_APPEND_VALUE].cmd = 
		CMD_APPEND_VALUE;
	rcp_command_table[CMD_APPEND_VALUE].cmd_str = 
		"appendValue";
	rcp_command_table[CMD_APPEND_VALUE].cmd_pms = 
		4;
	rcp_command_table[CMD_APPEND_VALUE].cmd_type= 
		cmd_append_value_type();
	rcp_command_table[CMD_APPEND_VALUE].cmd_impl= 
		cmd_impl_append_value;

	rcp_command_table[CMD_CREATE_STRUCT].cmd = 
		CMD_CREATE_STRUCT;
	rcp_command_table[CMD_CREATE_STRUCT].cmd_str = 
		"createStruct";
	rcp_command_table[CMD_CREATE_STRUCT].cmd_pms = 
		0;
	rcp_command_table[CMD_CREATE_STRUCT].cmd_type= 
		cmd_create_struct_type();
	rcp_command_table[CMD_CREATE_STRUCT].cmd_impl= 
		cmd_impl_create_struct;

	rcp_command_table[CMD_ADD_STRUCT].cmd = 
		CMD_ADD_STRUCT;
	rcp_command_table[CMD_ADD_STRUCT].cmd_str = 
		"addStruct";
	rcp_command_table[CMD_ADD_STRUCT].cmd_pms = 
		0;
	rcp_command_table[CMD_ADD_STRUCT].cmd_type= 
		cmd_add_struct_type();
	rcp_command_table[CMD_ADD_STRUCT].cmd_impl= 
		cmd_impl_add_struct;

	rcp_command_table[CMD_ADD_TYPE].cmd = 
		CMD_ADD_TYPE;
	rcp_command_table[CMD_ADD_TYPE].cmd_str = 
		"addType";
	rcp_command_table[CMD_ADD_TYPE].cmd_pms = 
		0;
	rcp_command_table[CMD_ADD_TYPE].cmd_type= 
		cmd_add_type_type();
	rcp_command_table[CMD_ADD_TYPE].cmd_impl= 
		cmd_impl_add_type;

	rcp_command_table[CMD_ERROR].cmd = 
		CMD_ERROR;
	rcp_command_table[CMD_ERROR].cmd_str = 
		"error";
	rcp_command_table[CMD_ERROR].cmd_pms = 
		0;
	rcp_command_table[CMD_ERROR].cmd_type= 
		cmd_error_type();
	rcp_command_table[CMD_ERROR].cmd_impl= 
		cmd_impl_error;

	rcp_command_table[CMD_CAUTION].cmd = 
		CMD_CAUTION;
	rcp_command_table[CMD_CAUTION].cmd_str = 
		"caution";
	rcp_command_table[CMD_CAUTION].cmd_pms = 
		0;
	rcp_command_table[CMD_CAUTION].cmd_type= 
		cmd_caution_type();
	rcp_command_table[CMD_CAUTION].cmd_impl= 
		cmd_impl_caution;

	rcp_command_table[CMD_INFO].cmd = 
		CMD_INFO;
	rcp_command_table[CMD_INFO].cmd_str = 
		"info";
	rcp_command_table[CMD_INFO].cmd_pms = 
		0;
	rcp_command_table[CMD_INFO].cmd_type= 
		cmd_info_type();
	rcp_command_table[CMD_INFO].cmd_impl= 
		cmd_impl_info;

}
