#include <stdio.h>

#include "rcp_pch.h"
#include "rcp_utility.h"

#include "rcp_ssl.h"

#include "cmd_table.h"

#include "rcp_event.h"
#include "rcp_server.h"

void rcp_main();

int main (int argc, const char** argv)
{
	signal(SIGPIPE,SIG_IGN);

	SSL_library_init();
	rcp_ssl_ctx_init();
	
	rcp_db_connect();

	rcp_shared_sender_cluster_init();

	rcp_command_table_init();
	rcp_root_context();

	int e_queue= rcp_event_new();
	rcp_open_timeout_setup(e_queue);
	rcp_listen_start(e_queue);
	rcp_event_run(e_queue);

	return 0;
}
