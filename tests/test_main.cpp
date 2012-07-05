#include <stdio.h>

#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "../rcp_command_list.h"
#include "../rcp_command.h"

int test_json(void);
int test_tree(void);
int test_map(void);
int test_array(void);
int test_struct(void);

int main (int argc, const char** argv){

	//rcp_command
	{
		if (rcp_command_from_str(CMD_STR_CAUTION) == 
				CMD_CAUTION)
			printf("ok\n");
		else
			printf("fail\n");
		rcp_info("command done");
	}
	test_json();
	test_tree();
	test_map();
	test_array();
	test_struct();
}
