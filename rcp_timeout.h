#include "def/rcp_connection.h"
#include "def/rcp_open_timeout.h"

rcp_open_timeout_ref rcp_open_timeout_new();
rcp_open_timeout_ref rcp_open_timeout_delete();
void rcp_open_timeout_add(
		rcp_open_timeout_ref timeout, rcp_connection_ref con);
void rcp_open_timeout_exec(rcp_open_timeout_ref timeout);
