
//epoll
void rcp_connection_epoll_action(int epfd, struct epoll_event *ev,
		epoll_data_t data);

typedef struct rcp_sender_cluster *rcp_sender_cluster_ref;
rcp_sender_cluster_ref rcp_sender_cluster_new();
void rcp_sender_cluster_set_rec(rcp_sender_cluster_ref cluster,
		rcp_record_ref rec);
void rcp_sender_cluster_clean_up(rcp_sender_cluster_ref cluster);

rcp_epoll_action_ref rcp_listener_plain_json_new(int epfd);
rcp_epoll_action_ref rcp_listener_ws_json_new(int epfd);
rcp_epoll_action_ref rcp_listener_wss_json_new(int epfd);

void rcp_listener_delete(rcp_epoll_action_ref unit);
