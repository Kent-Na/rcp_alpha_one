#include <stdio.h>

#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_command_list.h"
#include "rcp_command.h"

#include "rcp_epoll.h"
#include "rcp_connection.h"

int test_json(void);
int test_tree(void);
int test_map(void);

int main (int argc, const char** argv){
	//rcp_command
	{
		if (rcp_command_from_str(RCP_COMMAND_STR_CAUTION) == 
				RCP_COMMAND_CAUTION)
			printf("ok\n");
		else
			printf("fail\n");
		rcp_info("command done");
	}
	test_json();
	test_tree();
	test_map();
}
