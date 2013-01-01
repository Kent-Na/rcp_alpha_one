#include "../rcp_pch.h"
#include "../rcp_utility.h"

#include "../types/rcp_type_list.h"
#include "../types/rcp_array_list.h"

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

	{
		int32_t initial_data[] = 
			{0,1,2,3,4,5,6,7};
		rcp_array_ref initial_array = rcp_array_new(rcp_int32_array);
		for (int i = 0; i<8; i++){
			rcp_array_append_data(rcp_int32_array, 
				initial_array, (rcp_data_ref)(initial_data+i));
		}

		rcp_array_ref array = (rcp_array_ref)rcp_alloc(rcp_int32_array);
		rcp_copy(rcp_int32_array, 
			(rcp_data_ref)initial_array, (rcp_data_ref)array);

		int32_t extra_data[] = 
			{10,11,12};
		rcp_array_ref extra_array = rcp_array_new(rcp_int32_array);
		for (int i = 0; i<3; i++){
			rcp_array_append_data(rcp_int32_array, 
				extra_array, (rcp_data_ref)(extra_data+i));
		}
		rcp_array_replace(
			rcp_int32_array, (rcp_data_ref)array,
			0, -1, (rcp_data_ref)extra_array);

		for (int i = 0; i<3; i++){
			int32_t *ptr = (int32_t*)rcp_array_data_at(
			rcp_int32_array, array, i);
			if (*ptr != extra_data[i])
				rcp_error("array replace");
		}
		rcp_deinit(rcp_int32_array, (rcp_data_ref)array);
		rcp_copy(rcp_int32_array, 
			(rcp_data_ref)initial_array, (rcp_data_ref)array);

		int32_t ans[] = {
			0,1,10,11,12,4,5,6,7
		};
		rcp_array_replace(
			rcp_int32_array, (rcp_data_ref)array,
			2, 4, (rcp_data_ref)extra_array);

		for (int i = 0; i<9; i++){
			int32_t *ptr = (int32_t*)rcp_array_data_at(
			rcp_int32_array, array, i);
			if (*ptr != ans[i])
				rcp_error("array replace");
		}

	}

	rcp_info("array done");

	return 0;
}
