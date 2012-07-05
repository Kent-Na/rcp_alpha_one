
struct gen_protocol{
	const char* version;
	const char* client;

};

struct gen_create_user{
	const char* username;
	const char* password;

};

struct gen_delete_user{
	const char* username;
	const char* password;
	const char* username;
	const char* password;

};

struct gen_add_user{
	const char* username;

};

struct gen_remove_user{
	const char* username;

};

struct gen_update_user_permission{
	const char* username;
	const char* mode;

};

struct gen_delete_context{
	uint32_t contextID;

};

struct gen_login_context{
	uint32_t contextID;

};

struct gen_set_value{
	ref path;
	ref value;
	string type;

};

struct gen_unset_value{
	ref path;

};

struct gen_append_value{
	ref path;
	ref value;
	string type;

};

struct gen_error{
	const char* cause;
	const char* reason;

};

rcp_sturct_type_ref cmd_protocol_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_protocol);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("version");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_protocol, version);

	param->name = rcp_string_new("client");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_protocol, client);

}

rcp_sturct_type_ref cmd_create_user_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_create_user);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_create_user, username);

	param->name = rcp_string_new("password");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_create_user, password);

}

rcp_sturct_type_ref cmd_delete_user_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(4);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_delete_user);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 4;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_delete_user, username);

	param->name = rcp_string_new("password");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_delete_user, password);

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_delete_user, username);

	param->name = rcp_string_new("password");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_delete_user, password);

}

rcp_sturct_type_ref cmd_add_user_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_add_user);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_add_user, username);

}

rcp_sturct_type_ref cmd_remove_user_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_remove_user);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_remove_user, username);

}

rcp_sturct_type_ref cmd_update_user_permission_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_update_user_permission);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("username");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_update_user_permission, username);

	param->name = rcp_string_new("mode");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_update_user_permission, mode);

}

rcp_sturct_type_ref cmd_delete_context_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_delete_context);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("contextID");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_delete_context, contextID);

}

rcp_sturct_type_ref cmd_login_context_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_login_context);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("contextID");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_login_context, contextID);

}

rcp_sturct_type_ref cmd_set_value_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_set_value);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("path");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_set_value, path);

	param->name = rcp_string_new("value");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_set_value, value);

	param->name = rcp_string_new("type");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_set_value, type);

}

rcp_sturct_type_ref cmd_unset_value_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(1);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_unset_value);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 1;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("path");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_unset_value, path);

}

rcp_sturct_type_ref cmd_append_value_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(3);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_append_value);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 3;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("path");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_append_value, path);

	param->name = rcp_string_new("value");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_append_value, value);

	param->name = rcp_string_new("type");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_append_value, type);

}

rcp_sturct_type_ref cmd_error_type(){
	
	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct gen_error);
	t_core->type_id = 0x100;
	t_core->type_name = "ttt";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("ttt");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);

	param->name = rcp_string_new("cause");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_error, cause);

	param->name = rcp_string_new("reason");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct gen_error, reason);

}
