///
//server grobal resources
//

typedef uint32_t rcp_context_id_t;

rcp_extern void rcp_context_manager_init();
rcp_extern rcp_context_ref rcp_context_get(rcp_context_id_t id);
rcp_extern rcp_context_ref rcp_context_new(rcp_context_id_t id);

rcp_extern void rcp_listen_start(int epfd);
rcp_extern void rcp_listen_end();

typedef uint32_t rcp_user_id_t;
//id 0 is public id

rcp_extern rcp_user_id_t rcp_create_user(
		const char *user_name, const char *password);
rcp_extern rcp_user_id_t rcp_login_user(
		const char *user_name, const char *password);

void rcp_ssl_ctx_init();
SSL_CTX* rcp_ssl_ctx();

///
//should be thread local resources
//

typedef struct rcp_sender_cluster *rcp_sender_cluster_ref;
void rcp_shared_sender_cluster_init();
rcp_sender_cluster_ref rcp_shared_sender_cluster();
