#include <stdio.h>

#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_command_list.h"
#include "rcp_command.h"

#include "rcp_epoll.h"
#include "rcp_connection.h"

#include "rcp_type.h"
#include "rcp_context.h"
#include "rcp_server.h"

#include "cmd_types.h"

void rcp_main();

int main (int argc, const char** argv)
{
	SSL_library_init();

	rcp_command_type_table_init();
	rcp_context_manager_init();
	rcp_context_new(0);

	int epfd = epoll_create(10);
	rcp_listen_start(epfd);
	rcp_epoll_run(epfd);

	return 0;
}
