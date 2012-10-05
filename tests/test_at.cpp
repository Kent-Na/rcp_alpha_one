#include "../rcp_pch.h"
#include "../rcp_utility.h"

#include "../types/rcp_array.h"
#include "../types/rcp_type_utility.h"
#include "../types/rcp_type_list.h"
#include "../types/rcp_array_list.h"
#include "../types/rcp_dict_list.h"
#include "../types/rcp_dict.h"
#include "../types/rcp_string.h"

#include "../rcp_record.h"

#include <random>

int test_at(void){
	rcp_type_ref array_type = rcp_uint32_array;
	rcp_record_ref array_rec = rcp_record_new(array_type);
	rcp_array_ref array = (rcp_array_ref)rcp_record_data(array_rec);

	for (int i = 0; i<100; i++){
		uint32_t val = i;
		rcp_array_append_data(array_type, array, (rcp_data_ref)&val);
	}

	rcp_type_ref dict_type = rcp_str_ref_dict;
	rcp_dict_ref dict = rcp_dict_new(dict_type);

	rcp_string_ref key_str = rcp_string_new("the key");
	rcp_dict_node_ref node = rcp_dict_node_new_with(
			dict_type,
			(rcp_data_ref)key_str, (rcp_data_ref)&array_rec);
	rcp_dict_set_node(dict, node);
	
	rcp_type_ref io_type =	dict_type; 
	rcp_data_ref io_data = (rcp_data_ref)dict;

	rcp_array_ref path = rcp_array_new(rcp_ref_array);

	rcp_record_ref path_seg;
	path_seg = rcp_record_new_with(rcp_string_type, (rcp_data_ref)key_str);
	rcp_array_append_data(rcp_ref_array, path, (rcp_data_ref)&path_seg);

	rcp_data_at(&io_type, &io_data, path);

	if (*(rcp_record_ref*)io_data != array_rec)
		rcp_error("at 1");
	
	uint32_t path_seg_val = 20;	
	path_seg = rcp_record_new_with(rcp_uint32_type,
			(rcp_data_ref)&path_seg_val);
	rcp_array_append_data(rcp_ref_array, path, (rcp_data_ref)&path_seg);

	io_type =	dict_type; 
	io_data = (rcp_data_ref)dict;
	rcp_data_at(&io_type, &io_data, path);

	if (*(uint32_t*)io_data != 20)
		rcp_error("at 2");

	rcp_info("at done");
	return 0;
}
