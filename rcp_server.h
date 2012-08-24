#include "def/rcp_context.h"
#include "def/rcp_sender_cluster.h"

///
//server grobal resources
//

rcp_context_ref rcp_root_context();

rcp_extern void rcp_listen_start(int epfd);
rcp_extern void rcp_listen_end();

rcp_extern PGconn* rcp_db_connection();
rcp_extern void rcp_db_connect();

///
//should be thread local resources
//	(but not now...)
//

void rcp_shared_sender_cluster_init();
rcp_sender_cluster_ref rcp_shared_sender_cluster();
