#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"
#include "rcp_type.h"

#include "rcp_connection.h"

#include "rcp_command.h"
#include "cmd_list.h"
#include "cmd_types.h"

#include "types/rcp_string.h"
#include "types/rcp_map.h"
#include "types/rcp_array.h"
#include "types/rcp_struct.h"
#include "types/rcp_dict.h"
#include "types/rcp_type_list.h"


void rcp_array_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con)
{
	rcp_array_ref array = (rcp_array_ref)data;
	rcp_array_iterater_ref node = rcp_array_begin(array);

	struct cmd_append_value cmd;
	rcp_type_ref cmd_type=rcp_command_type(CMD_APPEND_VALUE);
	rcp_init(cmd_type, (rcp_data_ref)&cmd);
	cmd.command = rcp_string_new_rec(CMD_STR_APPEND_VALUE);

	while (node){
		cmd.value = *(rcp_record_ref*)rcp_array_iterater_data(array, node);
		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		node = rcp_array_iterater_next(array, node);
	}

	cmd.value = NULL;
	rcp_deinit(cmd_type, (rcp_data_ref)&cmd);
}

void rcp_dict_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con)
{
	rcp_dict_ref dict = (rcp_dict_ref)dict;
	rcp_dict_node_ref node = rcp_dict_begin(dict);

	while (node){
		struct cmd_set_value cmd;
		rcp_type_ref cmd_type=rcp_command_type(CMD_SET_VALUE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd);
		cmd.command = rcp_string_new_rec(CMD_STR_SET_VALUE);

		rcp_copy(rcp_dict_type_data_type(type), 
				rcp_dict_node_data(type, node),
				(rcp_data_ref)&cmd.value);
		rcp_copy(rcp_dict_type_key_type(type), 
				rcp_dict_node_key(type, node),
				(rcp_data_ref)&cmd.value);

		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd);

		node = rcp_dict_node_next(node);
	}
}
