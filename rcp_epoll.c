#include "rcp_pch.h"
#include "rcp_epoll.h"

void rcp_epoll_run(int epfd)
{
	int err;
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
			struct rcp_epoll_action *unit = events[i].data.ptr;
			unit->action(epfd, &events[i], unit->userdata);
		}
	}

}
