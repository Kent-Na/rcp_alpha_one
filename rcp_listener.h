#include "def/rcp_event.h"
#include "def/rcp_event_action.h"
//event
void rcp_connection_event_action(int epfd, rcp_event_ref ev,
		void *userdata);

rcp_event_action_ref rcp_listener_plain_json_new(int epfd);
rcp_event_action_ref rcp_listener_ws_json_new(int epfd);
rcp_event_action_ref rcp_listener_wss_json_new(int epfd);

void rcp_listener_delete(rcp_event_action_ref unit);
