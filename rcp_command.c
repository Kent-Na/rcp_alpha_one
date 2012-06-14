#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_command_list.h"

rcp_command_type_t rcp_command_from_str(const char* str)
{
	//binaly search
	size_t min = 0;
	size_t max = CMD_COUNT-1;
	
	while (max >= min){
		size_t mid = (min + max) >> 1;
		int cmp = strcmp(rcp_command_str_table[mid],str);

		if (cmp<0)
			min = mid+1;
		else if (cmp>0)
			max = mid-1;
		else
			return (rcp_command_type_t)mid;
	}

	return CMD_INVALID;
}

