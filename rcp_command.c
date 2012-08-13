#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_command.h"
#include "cmd_list.h"

extern struct rcp_command_core rcp_command_table[];

rcp_type_ref rcp_command_type(rcp_command_id_t id)
{
	return rcp_command_table[id].cmd_type;
}

rcp_command_ref rcp_command_from_id(rcp_command_id_t id)
{
	return &rcp_command_table[id];
}

rcp_command_ref rcp_command_from_str(const char* str)
{
	//binaly search
	size_t min = 0;
	size_t max = CMD_COUNT-1;
	
	while (max >= min){
		size_t mid = (min + max) >> 1;
		int cmp = strcmp(rcp_command_table[mid].cmd_str,str);

		if (cmp<0)
			min = mid+1;
		else if (cmp>0)
			max = mid-1;
		else
			return &rcp_command_table[mid];
	}

	return NULL;
}

