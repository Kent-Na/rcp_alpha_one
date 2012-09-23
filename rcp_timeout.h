#include "def/rcp_connection.h"
#include "def/rcp_open_timeout.h"

rcp_open_timeout_ref rcp_timeout_setup_timer(int epfd);

void rcp_open_timeout_add(
		rcp_open_timeout_ref timeout, rcp_connection_ref con);
void rcp_open_timeout_exec(rcp_open_timeout_ref timeout);
