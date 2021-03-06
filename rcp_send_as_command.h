#include "def/rcp_connection.h"

void rcp_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con);

void rcp_std_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con);

void rcp_array_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con);

void rcp_dict_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con);
