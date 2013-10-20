#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#include "types/rcp_type.h"
#include "types/rcp_record.h"

#include "rcp_connection.h"

#include "rcp_command.h"
#include "cmd_list.h"
#include "cmd_types.h"

#include "types/rcp_string.h"
#include "types/rcp_struct.h"
#include "types/rcp_dict.h"
#include "types/rcp_type_list.h"
#include "types/rcp_array.h"
#include "types/rcp_array_list.h"
#include "types/rcp_dict_list.h"

#include "rcp_send_as_command.h"

void rcp_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con)
{
    if (type == rcp_ref_array)
        rcp_array_send_as_command(type, data, con);
    else if (type == rcp_str_ref_dict)
        rcp_dict_send_as_command(type, data, con);
    else
        rcp_std_send_as_command(type, data, con);
}

void rcp_std_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con)
{
	struct cmd_set_value cmd_ini;
	rcp_type_ref cmd_type=rcp_command_type(CMD_SET_VALUE);
	rcp_init(cmd_type, (rcp_data_ref)&cmd_ini);
	cmd_ini.command = rcp_string_new_rec(CMD_STR_SET_VALUE);
	cmd_ini.loginID = 0;
	cmd_ini.value = rcp_record_new_with(type, data);
	rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd_ini);
	rcp_deinit(cmd_type, (rcp_data_ref)&cmd_ini);
}

void rcp_array_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con)
{
	if (rcp_array_type_data_type(type) != rcp_ref_type){
		rcp_error("Unsupported array type.");
		return;
	}

	{
		struct cmd_set_value cmd_ini;
		rcp_type_ref cmd_type=rcp_command_type(CMD_SET_VALUE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd_ini);
		cmd_ini.command = rcp_string_new_rec(CMD_STR_SET_VALUE);
		cmd_ini.loginID = 0;
		cmd_ini.value = rcp_record_new(type);
		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd_ini);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd_ini);
	}

	rcp_record_ref tmp_record = rcp_record_new(type);
	rcp_array_ref tmp_array = (rcp_array_ref)rcp_record_data(tmp_record);
	rcp_record_ref *tmp_val;

	{
		rcp_record_ref null_rec = NULL;
		rcp_array_append_data(type, tmp_array, (rcp_data_ref)&null_rec);
		tmp_val = rcp_array_raw_data(tmp_array);
	}

	rcp_array_ref array = (rcp_array_ref)data;
	rcp_array_iterater_ref node = rcp_array_begin(array);
	
	struct cmd_replace_value cmd;
	rcp_type_ref cmd_type=rcp_command_type(CMD_REPLACE_VALUE);
	rcp_init(cmd_type, (rcp_data_ref)&cmd);
	cmd.command = rcp_string_new_rec(CMD_STR_REPLACE_VALUE);
	cmd.begin = -1;
	cmd.end = -1;
	cmd.loginID = 0;
	cmd.value = tmp_record;

	while (node){
		*tmp_val = *(rcp_record_ref*)rcp_array_iterater_data(node);
		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		node = rcp_array_iterater_next(type, array, node);
	}
	
	*tmp_val = NULL;
	rcp_deinit(cmd_type, (rcp_data_ref)&cmd);
}
void rcp_dict_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con)
{
	{
		struct cmd_set_value cmd;
		rcp_type_ref cmd_type=rcp_command_type(CMD_SET_VALUE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd);
		cmd.command = rcp_string_new_rec(CMD_STR_SET_VALUE);
		cmd.loginID = 0;
		cmd.value = rcp_record_new(type);
		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd);
	}
	rcp_dict_ref dict = (rcp_dict_ref)data;
	rcp_dict_node_ref node = rcp_dict_begin(dict);
	rcp_type_ref key_type = rcp_dict_type_key_type(type);
	rcp_type_ref data_type = rcp_dict_type_data_type(type);

	while (node){
		struct cmd_set_value cmd;
		rcp_type_ref cmd_type=rcp_command_type(CMD_SET_VALUE);
		rcp_init(cmd_type, (rcp_data_ref)&cmd);
		cmd.command = rcp_string_new_rec(CMD_STR_SET_VALUE);
		cmd.loginID = 0;

		if (key_type == rcp_ref_type){
			rcp_copy(rcp_dict_type_key_type(type), 
					rcp_dict_node_key(type, node),
					(rcp_data_ref)&cmd.path);
		}
		else{
			cmd.path = rcp_record_new_with(key_type,
					rcp_dict_node_key(type, node));
		}

		if (data_type == rcp_ref_type){
			rcp_copy(rcp_dict_type_data_type(type), 
					rcp_dict_node_data(type, node),
					(rcp_data_ref)&cmd.value);
		}
		else{
			cmd.value = rcp_record_new_with(data_type,
					rcp_dict_node_data(type, node));
		}

		rcp_connection_send_data(con, cmd_type, (rcp_data_ref)&cmd);
		rcp_deinit(cmd_type, (rcp_data_ref)&cmd);

		node = rcp_dict_node_next(node);
	}
}
