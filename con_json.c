#include "rcp_command.h"
#include "con_json.h"


struct con_json_command{
	const char *json_str;
	const char *json_str_end;

	rcp_command_t cmd;
	
}
