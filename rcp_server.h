///
//server grobal resources
//

typedef uint32_t rcp_context_id_t;

rcp_extern void rcp_context_manager_init();
rcp_extern rcp_context_ref rcp_context_get(rcp_context_id_t id);
rcp_extern rcp_context_ref rcp_context_new(rcp_context_id_t id);

rcp_extern void rcp_listen_start(int epfd);
rcp_extern void rcp_listen_end();


///
//should be thread local resources
//

void rcp_shared_sender_cluster_init();
rcp_sender_cluster_ref rcp_shared_sender_cluster();
