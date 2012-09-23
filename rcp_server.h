#include "def/rcp_context.h"
#include "def/rcp_sender_cluster.h"
#include "def/rcp_open_timeout.h"

///
//server grobal resources
//

rcp_context_ref rcp_root_context();

rcp_extern void rcp_listen_start(int epfd);
rcp_extern void rcp_listen_end();

rcp_extern PGconn* rcp_db_connection();
rcp_extern void rcp_db_connect();

rcp_extern rcp_open_timeout_ref rcp_open_timeout();
rcp_extern void rcp_open_timeout_setup(int epfd);


///
//should be thread local resources
//	(but not now...)
//

void rcp_shared_sender_cluster_init();
rcp_sender_cluster_ref rcp_shared_sender_cluster();
