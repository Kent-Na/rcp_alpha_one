#include "def/rcp_context.h"
#include "def/rcp_connection.h"
#include "def/rcp_command.h"
#include "def/rcp_permission.h"

typedef uint8_t rcp_command_id_t;

struct rcp_command_core{
	rcp_command_id_t cmd;
	const char* cmd_str;
	rcp_type_ref cmd_type;
	rcp_permission_t cmd_pms;

	void(*cmd_impl)(
			rcp_context_ref ctx,
			rcp_connection_ref con,
			rcp_record_ref cmd_rec,
			rcp_type_ref cmd_type,
			void* cmd);
};

rcp_extern rcp_type_ref rcp_command_type(rcp_command_id_t id);
rcp_extern rcp_command_ref rcp_command_from_str(const char* str);
rcp_extern rcp_command_ref rcp_command_from_id(rcp_command_id_t id);

