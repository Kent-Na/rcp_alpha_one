
rcp_extern void rcp_context_manager_init();
rcp_extern rcp_context_ref rcp_context_get(uint32_t id);
rcp_extern rcp_context_ref rcp_context_new(uint32_t id);

rcp_extern void rcp_listen_start(int epfd);
rcp_extern void rcp_listen_end();

