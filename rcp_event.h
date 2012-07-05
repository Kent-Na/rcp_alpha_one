#include "def/rcp_event.h"
#include "def/rcp_event_action.h"

struct rcp_event_action_core{
	void (*action)(int event_queue, rcp_event_ref ev, void *userdata);
	void *userdata;
};

int rcp_event_new();
void rcp_event_run(int event_queue);
void rcp_event_add_fd(int event_queue, int fd, rcp_event_action_ref action);
