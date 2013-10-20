#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "../rcp_defines.h"

#include "rcp_type.h"
#include "rcp_record.h"
#include "rcp_struct.h"
#include "rcp_string.h"
#include "rcp_type_list.h"
#include "rcp_dict.h"
#include "rcp_dict_list.h"
#include "rcp_array.h"
#include "rcp_array_list.h"


rcp_extern rcp_record_ref rcp_dict_find_c_str(
		rcp_type_ref dict_type, rcp_dict_ref dict, 
		const char *key, rcp_type_ref type)
{

#ifdef RCP_SELF_TEST
	if (rcp_dict_type_key_type(dict_type) != rcp_string_type){
		rcp_error("dict value type");
		return NULL;
	}
	if (rcp_dict_type_data_type(dict_type) != rcp_ref_type){
		rcp_error("dict value type");
		return NULL;
	}
#endif

	rcp_string_ref str = rcp_string_new(key);
	rcp_dict_node_ref node = rcp_dict_find(dict, (rcp_data_ref)str);
	rcp_string_delete(str);
	if (!node){
		rcp_caution("not found");
		return NULL;
	}
	rcp_record_ref rec = *(rcp_record_ref*)
		rcp_dict_node_data(dict_type, node);
	if (type && rcp_record_type(rec) != type){
		rcp_caution("not this type");
		return NULL;
	}
	return rec;
}
rcp_extern void rcp_dict_to_struct(
		rcp_type_ref in_type, rcp_dict_ref in,
		rcp_type_ref out_type, rcp_struct_ref out)
{
#ifdef RCP_SELF_TEST
	if (rcp_dict_type_key_type(in_type) != rcp_string_type){
		rcp_error("dict value type");
		return;
	}
	if (rcp_dict_type_data_type(in_type) != rcp_ref_type){
		rcp_error("dict value type");
		return;
	}
#endif

	rcp_dict_node_ref node = rcp_dict_begin(in);
	rcp_struct_param_ref param = rcp_struct_type_begin(out_type);
	while (1){
		int comp = rcp_compare(rcp_string_type, 
				rcp_dict_node_key(in_type, node), 
				(rcp_data_ref)rcp_struct_param_name(param));

		if (comp < 0){
			node = rcp_dict_node_next(node);
			if (!node)
				break;
		}
		else if (comp > 0){
			param = rcp_struct_param_next(out_type, param);
			if (!param)
				break;
		}
		else{
			rcp_record_ref node_data = 
				*(rcp_record_ref*)rcp_dict_node_data(in_type, node);
			rcp_type_ref rec_type = rcp_record_type(node_data);
			rcp_type_ref param_type = rcp_struct_param_type(param);
			if (param_type == rcp_ref_type){
				rcp_data_ref dst = rcp_struct_data(out, param);
				rcp_copy(rcp_ref_type, (rcp_data_ref)&node_data, dst);
			}
			else if (rec_type == param_type){
				rcp_data_ref src = rcp_record_data(node_data);
				rcp_data_ref dst = rcp_struct_data(out, param);
				rcp_copy(param_type, src, dst);
			}
			else{
				rcp_caution("type missmatch");
			}
			node = rcp_dict_node_next(node);
			param = rcp_struct_param_next(out_type, param);
			if (!node || !param)
				break;
		}
	}
}

rcp_extern int rcp_type_is_int(rcp_type_ref type)
{
	if (type == rcp_int8_type)
		return 1;
	if (type == rcp_int16_type)
		return 1;
	if (type == rcp_int32_type)
		return 1;
	if (type == rcp_int64_type)
		return 1;
	return 0;
}

rcp_extern int rcp_type_is_uint(rcp_type_ref type)
{
	if (type == rcp_uint8_type)
		return 1;
	if (type == rcp_uint16_type)
		return 1;
	if (type == rcp_uint32_type)
		return 1;
	if (type == rcp_uint64_type)
		return 1;
	return 0;
}

rcp_extern int64_t rcp_int_as_int(rcp_type_ref type, rcp_data_ref data)
{
	if (type == rcp_int8_type)
		return *(int8_t*)data;
	if (type == rcp_int16_type)
		return *(int16_t*)data;
	if (type == rcp_int32_type)
		return *(int32_t*)data;
	if (type == rcp_int64_type)
		return *(int64_t*)data;
	return 0;
}

rcp_extern uint64_t rcp_uint_as_uint(rcp_type_ref type, rcp_data_ref data)
{
	if (type == rcp_uint8_type)
		return *(uint8_t*)data;
	if (type == rcp_uint16_type)
		return *(uint16_t*)data;
	if (type == rcp_uint32_type)
		return *(uint32_t*)data;
	if (type == rcp_uint64_type)
		return *(uint64_t*)data;
	return 0;
}

rcp_extern int rcp_record_is_null(rcp_record_ref rec)
{
	if (! rec)
		return 1;
	if (rcp_record_type(rec) == rcp_null_type)
		return 1;
	return 0;
}

void rcp_data_at(rcp_type_ref *io_type, rcp_data_ref *io_data, 
		rcp_array_ref path)
{
	rcp_type_ref path_type = rcp_ref_array;
	rcp_array_iterater_ref itr = rcp_array_begin(path);
	while (itr){
        if (! *io_data) 
            return;
		rcp_record_ref path_seg = 
			*(rcp_record_ref*)rcp_array_iterater_data(itr);
		rcp_at(io_type, io_data, 
				rcp_record_type(path_seg), rcp_record_data(path_seg));
		itr = rcp_array_iterater_next(path_type, path, itr);
	}
}
