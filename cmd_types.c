//This file is generated by "gen_command.py"
#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_tree.h"
#include "rcp_json_write.h"
#define RCP_INTERNAL_STRUCTURE
#include "types/rcp_type_list.h"
#include "types/rcp_string.h"
#include "types/rcp_struct.h"
#include "rcp_type.h"
#include "cmd_list.h"
#include "cmd_types.h"

rcp_type_ref cmd_open_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_open);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("client");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_open, client);
	param ++;

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_open, command);
	param ++;

	param->name = rcp_string_new("protocol");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_open, protocol);
	param ++;

	return s_type;
}

rcp_type_ref cmd_kill_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_kill);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_kill, command);
	param ++;

	return s_type;
}

rcp_type_ref cmd_dump_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_dump);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_dump, command);
	param ++;

	return s_type;
}

rcp_type_ref cmd_load_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_load);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_load, command);
	param ++;

	return s_type;
}

rcp_type_ref cmd_close_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_close);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_close, command);
	param ++;

	return s_type;
}

rcp_type_ref cmd_ping_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_ping);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_ping, command);
	param ++;

	return s_type;
}

rcp_type_ref cmd_pong_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_pong);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_pong, command);
	param ++;

	return s_type;
}

rcp_type_ref cmd_create_user_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_create_user);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_create_user, command);
	param ++;

	param->name = rcp_string_new("password");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_create_user, password);
	param ++;

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_create_user, username);
	param ++;

	return s_type;
}

rcp_type_ref cmd_delete_user_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_delete_user);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_delete_user, command);
	param ++;

	param->name = rcp_string_new("password");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_delete_user, password);
	param ++;

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_delete_user, username);
	param ++;

	return s_type;
}

rcp_type_ref cmd_login_user_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_login_user);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_login_user, command);
	param ++;

	param->name = rcp_string_new("password");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_login_user, password);
	param ++;

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_login_user, username);
	param ++;

	return s_type;
}

rcp_type_ref cmd_add_user_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_add_user);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_add_user, command);
	param ++;

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_add_user, username);
	param ++;

	return s_type;
}

rcp_type_ref cmd_remove_user_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_remove_user);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_remove_user, command);
	param ++;

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_remove_user, username);
	param ++;

	return s_type;
}

rcp_type_ref cmd_set_permission_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_set_permission);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_set_permission, command);
	param ++;

	param->name = rcp_string_new("mode");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_set_permission, mode);
	param ++;

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_set_permission, username);
	param ++;

	return s_type;
}

rcp_type_ref cmd_unset_permission_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_unset_permission);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_unset_permission, command);
	param ++;

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_unset_permission, username);
	param ++;

	return s_type;
}

rcp_type_ref cmd_add_context_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_add_context);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_add_context, command);
	param ++;

	param->name = rcp_string_new("name");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_add_context, name);
	param ++;

	return s_type;
}

rcp_type_ref cmd_remove_context_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_remove_context);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_remove_context, command);
	param ++;

	param->name = rcp_string_new("name");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_remove_context, name);
	param ++;

	return s_type;
}

rcp_type_ref cmd_login_context_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_login_context);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_login_context, command);
	param ++;

	param->name = rcp_string_new("name");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_login_context, name);
	param ++;

	return s_type;
}

rcp_type_ref cmd_logout_context_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_logout_context);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_logout_context, command);
	param ++;

	return s_type;
}

rcp_type_ref cmd_send_value_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_send_value);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_send_value, command);
	param ++;

	param->name = rcp_string_new("type");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_send_value, type);
	param ++;

	param->name = rcp_string_new("value");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_send_value, value);
	param ++;

	return s_type;
}

rcp_type_ref cmd_set_value_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(4);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_set_value);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 4;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_set_value, command);
	param ++;

	param->name = rcp_string_new("path");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_set_value, path);
	param ++;

	param->name = rcp_string_new("type");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_set_value, type);
	param ++;

	param->name = rcp_string_new("value");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_set_value, value);
	param ++;

	return s_type;
}

rcp_type_ref cmd_unset_value_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_unset_value);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_unset_value, command);
	param ++;

	param->name = rcp_string_new("path");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_unset_value, path);
	param ++;

	return s_type;
}

rcp_type_ref cmd_append_value_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(4);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_append_value);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 4;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_append_value, command);
	param ++;

	param->name = rcp_string_new("path");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_append_value, path);
	param ++;

	param->name = rcp_string_new("type");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_append_value, type);
	param ++;

	param->name = rcp_string_new("value");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_append_value, value);
	param ++;

	return s_type;
}

rcp_type_ref cmd_create_struct_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_create_struct);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_create_struct, command);
	param ++;

	return s_type;
}

rcp_type_ref cmd_add_struct_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_add_struct);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_add_struct, command);
	param ++;

	return s_type;
}

rcp_type_ref cmd_add_type_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_add_type);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_add_type, command);
	param ++;

	param->name = rcp_string_new("name");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_add_type, name);
	param ++;

	return s_type;
}

rcp_type_ref cmd_error_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_error);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("cause");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_error, cause);
	param ++;

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_error, command);
	param ++;

	param->name = rcp_string_new("reason");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_error, reason);
	param ++;

	return s_type;
}

rcp_type_ref cmd_caution_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_caution);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("cause");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_caution, cause);
	param ++;

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_caution, command);
	param ++;

	param->name = rcp_string_new("reason");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_caution, reason);
	param ++;

	return s_type;
}

rcp_type_ref cmd_info_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct cmd_info);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("cause");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_info, cause);
	param ++;

	param->name = rcp_string_new("command");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_info, command);
	param ++;

	param->name = rcp_string_new("info");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct cmd_info, info);
	param ++;

	return s_type;
}
