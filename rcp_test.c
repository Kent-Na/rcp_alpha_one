#include <stdio.h>

#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_command_list.h"
#include "rcp_command.h"

#include "rcp_epoll.h"
#include "rcp_connection.h"

void rcp_main();

int main (int argc, const char** argv){
	//rcp_command
	{
		if (rcp_command_from_str(RCP_COMMAND_STR_CAUTION) == 
				RCP_COMMAND_CAUTION)
			printf("ok\n");
		else
			printf("fail\n");
	}


	int epfd = epoll_create(10);
	rcp_listener_plain_json_new(epfd);
	rcp_epoll_run(epfd);


}
