#include "def/rcp_type.h"
#include "def/rcp_data.h"
#include "def/rcp_connection.h"

void rcp_array_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con);

void rcp_dict_send_as_command(
		rcp_type_ref type, rcp_data_ref data,
		rcp_connection_ref con);
