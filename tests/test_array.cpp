#include "../rcp_pch.h"
#include "../rcp_utility.h"

#include "../types/rcp_type_list.h"

#define RCP_INTERNAL_STRUCTURE
#include "../rcp_type.h"
#include "../types/rcp_array.h"
#include <random>

int test_array(void){

	std::mt19937 eng(time(NULL));
	std::uniform_int_distribution<int> dist(0,500);
	
	struct {
		struct rcp_type_core core;
		struct rcp_type_array_ext ext;
	} array_type_val = {
		{
			1024,
			0,
			NULL,
			rcp_array_init,
			rcp_array_deinit,
			rcp_array_copy,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
		},
		{
			rcp_uint32_type
		}
	};

	auto array_type = &array_type_val.core;

	rcp_array_ref array = rcp_array_new(array_type);

	if (rcp_array_type_data_type(array_type) != rcp_uint32_type)
		rcp_error("array type");

	const uint32_t num = 100;

	for (unsigned int i= 0; i<num; i++){
		uint32_t val = i;
		rcp_array_append_data(array_type, array, (rcp_data_ref)&val);
	}

	if (rcp_array_len(array) != num)
		rcp_error("array count");

	for (unsigned int i= 0; i<num; i++){
		uint32_t *val = (uint32_t *)rcp_array_raw_data(array);
		if (i != val[i])
			rcp_error("array val");
	}

	rcp_array_iterater_ref itr = rcp_array_begin(array);
	for (unsigned int i= 0; i<num; i++){
		uint32_t *val = (uint32_t *)rcp_array_iterater_data(itr);
		if (i != *val)
			rcp_error("array itr val");
		itr = rcp_array_iterater_next(array_type, array,itr);
	}
	if (itr)
		rcp_error("array itr end");

	rcp_array_clear_data(array_type, array);
	if (rcp_array_begin(array))
		rcp_error("array clear");

	rcp_info("array done");

	return 0;
}
