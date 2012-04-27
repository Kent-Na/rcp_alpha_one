
struct rcp_epoll_action{
	void (*action)(int epfd, struct epoll_event *ev, epoll_data_t userdata);
	epoll_data_t userdata;
};

void rcp_epoll_run(int epfd);

