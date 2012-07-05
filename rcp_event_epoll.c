#ifdef __linux__

#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_event.h"

int rcp_event_new()
{
	return epoll_create(10);
}

void rcp_event_run(int epfd)
{
	int i;

	while (1){
		int maxEventsCount = 16;
		struct epoll_event events[maxEventsCount];
		int nfds = epoll_wait(epfd,events,
						maxEventsCount,
						-1);
		if (nfds<0){
			//printf("err : %s\n", strerror(errno));
			return;
		}
		for (i = 0; i<nfds; i++){
			rcp_event_action_ref unit = events[i].data.ptr;
			unit->action(epfd, &events[i], unit->userdata);
		}
	}

}


void rcp_event_add_fd(int epfd, int fd, rcp_event_action_ref action)
{
	struct epoll_event ev;
	ev.events = EPOLLIN|EPOLLPRI|EPOLLRDHUP|EPOLLERR|EPOLLHUP;
	ev.data.ptr = action;
	int err = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
	if (err)
		rcp_error("epoll ctl");
}

#endif //__linux__
