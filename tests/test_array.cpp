#include "../rcp_pch.h"
#include "../rcp_utility.h"

#include "../types/rcp_type_list.h"
#include "../types/rcp_old_array.h"
#include <random>

int test_array(void){

	std::mt19937 eng(time(NULL));
	std::uniform_int_distribution<int> dist(0,500);
	
	rcp_old_array_ref array = rcp_old_array_new(rcp_uint32_type);

	if (rcp_old_array_data_type(array) != rcp_uint32_type)
		rcp_error("array type");

	const uint32_t num = 1;

	for (unsigned int i= 0; i<num; i++){
		uint32_t val = i;
		rcp_old_array_append_data(array, &val);
	}

	if (rcp_old_array_count(array) != num)
		rcp_error("array count");

	for (unsigned int i= 0; i<num; i++){
		uint32_t *val = (uint32_t *)rcp_old_array_data(array, i);
		if (i != *val)
			rcp_error("array val");
	}

	rcp_old_array_iterater_ref itr = rcp_old_array_begin(array);
	for (unsigned int i= 0; i<num; i++){
		uint32_t *val = (uint32_t *)rcp_old_array_iterater_data(array, itr);
		if (i != *val)
			rcp_error("array itr val");
		itr = rcp_old_array_iterater_next(array,itr);
	}
	if (itr)
		rcp_error("array itr end");

	rcp_info("array done");

	return 0;
}
