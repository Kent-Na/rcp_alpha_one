#include "def/rcp_type.h"
#include "def/rcp_record.h"

struct cmd_open{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref protocol;
	rcp_record_ref client;
};

struct cmd_kill{
	rcp_record_ref command;
	uint16_t loginID;
};

struct cmd_dump{
	rcp_record_ref command;
	uint16_t loginID;
};

struct cmd_load{
	rcp_record_ref command;
	uint16_t loginID;
};

struct cmd_close{
	rcp_record_ref command;
	uint16_t loginID;
};

struct cmd_ping{
	rcp_record_ref command;
	uint16_t loginID;
};

struct cmd_pong{
	rcp_record_ref command;
	uint16_t loginID;
};

struct cmd_create_user{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref username;
	rcp_record_ref password;
};

struct cmd_delete_user{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref username;
	rcp_record_ref password;
};

struct cmd_login_user{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref username;
	rcp_record_ref password;
};

struct cmd_add_user{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref username;
};

struct cmd_remove_user{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref username;
};

struct cmd_set_permission{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref username;
	rcp_record_ref mode;
};

struct cmd_unset_permission{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref username;
};

struct cmd_add_context{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref name;
	rcp_record_ref timestamp;
	uint16_t connectionCount;
};

struct cmd_update_context{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref name;
	rcp_record_ref timestamp;
	uint16_t connectionCount;
};

struct cmd_remove_context{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref name;
};

struct cmd_login_context{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref name;
};

struct cmd_logout_context{
	rcp_record_ref command;
	uint16_t loginID;
};

struct cmd_reset_context{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref name;
};

struct cmd_send_value{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref value;
};

struct cmd_set_value{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref path;
	rcp_record_ref value;
};

struct cmd_unset_value{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref path;
};

struct cmd_replace_value{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref path;
	int64_t begin;
	int64_t end;
	rcp_record_ref value;
};

struct cmd_fatal{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref cause;
	rcp_record_ref description;
};

struct cmd_error{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref cause;
	rcp_record_ref description;
};

struct cmd_caution{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref cause;
	rcp_record_ref description;
};

struct cmd_info{
	rcp_record_ref command;
	uint16_t loginID;
	rcp_record_ref cause;
	rcp_record_ref description;
};
