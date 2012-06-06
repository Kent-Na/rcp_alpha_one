
//epoll
void rcp_connection_epoll_action(int epfd, struct epoll_event *ev,
		epoll_data_t data);

rcp_epoll_action_ref rcp_listener_plain_json_new(int epfd);
rcp_epoll_action_ref rcp_listener_plain_ws_json_new(int epfd);

void rcp_listener_release(rcp_epoll_action_ref unit);
