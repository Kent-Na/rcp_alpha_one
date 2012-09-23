#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"

#include "rcp_connection.h"
#include "rcp_event.h"

#include "types/rcp_array.h"
#include "types/rcp_array_list.h"

#include "rcp_timeout.h"

//must run on single thread
struct rcp_open_timeout_core{
	struct rcp_event_action_core act_core;

	///
	//array of rcp_connection_ref as pointer

	//less than "timeout" at exec
	rcp_array_ref buket0;
	//"timeout" < t < 2 x "timeout" 
	rcp_array_ref buket1;
};



void rcp_open_timeout_event_action(
		int epfd, rcp_event_ref ev, void* userdata);

rcp_open_timeout_ref rcp_open_timeout_new()
{
	rcp_open_timeout_ref timeout = malloc(sizeof(*timeout));
	timeout->act_core.action = rcp_open_timeout_event_action;
	timeout->act_core.userdata = timeout;
	timeout->buket0 = rcp_array_new(rcp_pointer_array);
	timeout->buket1 = rcp_array_new(rcp_pointer_array);
	return timeout;
}

void rcp_open_timeout_event_action(
		int epfd, rcp_event_ref ev, void* userdata)
{
	rcp_open_timeout_exec(userdata);
}

rcp_open_timeout_ref rcp_open_timeout_delete();
void rcp_open_timeout_add(
		rcp_open_timeout_ref timeout, rcp_connection_ref con)
{
	rcp_array_append_data(rcp_pointer_array, 
			timeout->buket0, (rcp_data_ref)&con);
}

void rcp_open_timeout_exec(rcp_open_timeout_ref timeout)
{
	rcp_array_iterater_ref itr = rcp_array_begin(timeout->buket1);
	while (itr){
		rcp_connection_ref con = 
			*(rcp_connection_ref*)rcp_array_iterater_data(itr);
		rcp_connection_release(con);
		itr = rcp_array_iterater_next(
				rcp_pointer_array, timeout->buket1, itr);
	}
	rcp_array_clear_data(rcp_pointer_array, timeout->buket1);

	//swap
	rcp_array_ref tmp = timeout->buket0;
	timeout->buket0 = timeout->buket1;
	timeout->buket1 = tmp;
}
