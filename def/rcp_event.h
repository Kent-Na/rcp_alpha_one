
#ifndef RCP_DEF_EVENT
#define RCP_DEF_EVENT
#ifdef __linux__
typedef struct epoll_event *rcp_event_ref;
#define RCP_USE_EPOLL
#else
typedef struct kevent *rcp_event_ref;
#define RCP_USE_KQUEUE
#endif
#endif
