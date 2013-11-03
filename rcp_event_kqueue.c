#ifndef __linux__

#include "rcp_pch.h"
#include "rcp_event.h"

int rcp_event_new()
{
	return kqueue();
}

void rcp_event_run(int kq)
{
	int i;

	while (1){
		int maxEventsCount = 16;
		struct kevent events[maxEventsCount];
		int nfds = kevent(kq, NULL, 0, events, maxEventsCount, NULL);

		if (nfds<0){
			//printf("err : %s\n", strerror(errno));
			return;
		}
		for (i = 0; i<nfds; i++){
			struct kevent *event = events+i;
			rcp_event_action_ref unit = event->udata;
			unit->action(kq, event, unit->userdata);
		}
	}

}

void rcp_event_add_fd(int kq, int fd, rcp_event_action_ref action)
{
	struct kevent ev;
	EV_SET(&ev, fd, EVFILT_READ, EV_ADD, 0, 0, action);
	int err = kevent(kq, &ev, 1, NULL, 0, NULL);
	if (err)
		rcp_error("kevent add");
}

#endif //not __linux__
