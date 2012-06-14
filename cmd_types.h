
struct cmd_protocol{
	rcp_record_ref version;
	rcp_record_ref client;
	rcp_record_ref command;
};

struct cmd_kill{
	rcp_record_ref command;
};

struct cmd_close{
	rcp_record_ref command;
};

struct cmd_ping{
	rcp_record_ref command;
};

struct cmd_pong{
	rcp_record_ref command;
};

struct cmd_create_user{
	rcp_record_ref username;
	rcp_record_ref password;
	rcp_record_ref command;
};

struct cmd_delete_user{
	rcp_record_ref username;
	rcp_record_ref password;
	rcp_record_ref command;
};

struct cmd_login_user{
	rcp_record_ref username;
	rcp_record_ref password;
	rcp_record_ref command;
};

struct cmd_add_user{
	rcp_record_ref username;
	rcp_record_ref command;
};

struct cmd_remove_user{
	rcp_record_ref username;
	rcp_record_ref command;
};

struct cmd_update_user_permission{
	rcp_record_ref username;
	rcp_record_ref mode;
	rcp_record_ref command;
};

struct cmd_create_context{
	rcp_record_ref command;
};

struct cmd_delete_context{
	uint32_t contextID;
	rcp_record_ref command;
};

struct cmd_login_context{
	uint32_t contextID;
	rcp_record_ref command;
};

struct cmd_update_name{
	rcp_record_ref command;
};

struct cmd_update_user_list{
	rcp_record_ref command;
};

struct cmd_send_value{
	rcp_record_ref value;
	rcp_record_ref type;
	rcp_record_ref command;
};

struct cmd_set_value{
	rcp_record_ref path;
	rcp_record_ref value;
	rcp_record_ref type;
	rcp_record_ref command;
};

struct cmd_unset_value{
	rcp_record_ref path;
	rcp_record_ref command;
};

struct cmd_append_value{
	rcp_record_ref path;
	rcp_record_ref value;
	rcp_record_ref type;
	rcp_record_ref command;
};

struct cmd_reserve_record{
	rcp_record_ref command;
};

struct cmd_release_record{
	rcp_record_ref command;
};

struct cmd_create_struct{
	rcp_record_ref command;
};

struct cmd_add_struct{
	rcp_record_ref command;
};

struct cmd_error{
	rcp_record_ref cause;
	rcp_record_ref reason;
	rcp_record_ref command;
};

struct cmd_caution{
	rcp_record_ref command;
};

struct cmd_note{
	rcp_record_ref command;
};

rcp_type_ref rcp_command_type(rcp_command_type_t command_id);

void rcp_command_type_table_init();
