
#ifdef __linux__
typedef struct epoll_event *rcp_event_ref;
#define RCP_USE_EPOLL
#else
typedef struct kevent *rcp_event_ref;
#define RCP_USE_KQUEUE
#endif


struct rcp_event_action{
	void (*action)(int event_queue, rcp_event_ref ev, void *userdata);
	void *userdata;
};

typedef struct rcp_event_action *rcp_event_action_ref;
int rcp_event_new();
void rcp_event_run(int event_queue);
void rcp_event_add_fd(int event_queue, int fd, rcp_event_action_ref action);
