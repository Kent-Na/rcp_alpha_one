#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#include "rcp_type.h"
#include "rcp_record.h"
#include "types/rcp_map.h"
#include "types/rcp_struct.h"
#include "types/rcp_string.h"
#include "types/rcp_type_list.h"

rcp_extern rcp_record_ref rcp_map_find_c_str(
		rcp_map_ref map, const char *key, rcp_type_ref type)
{

#ifdef RCP_SELF_TEST
	if (rcp_map_key_type(map) != rcp_string_type){
		rcp_error("map value type");
		return NULL;
	}
	if (rcp_map_value_type(map) != rcp_ref_type){
		rcp_error("map value type");
		return NULL;
	}
#endif

	rcp_string_ref str = rcp_string_new(key);
	rcp_map_node_ref node = rcp_map_find(map, str);
	rcp_string_delete(str);
	if (!node){
		rcp_caution("not found");
		return NULL;
	}
	rcp_record_ref rec = *(rcp_record_ref*)rcp_map_node_value(map, node);
	if (type && rcp_record_type(rec) != type){
		rcp_caution("not this type");
		return NULL;
	}
	return rec;
}

rcp_extern void rcp_map_to_struct(rcp_map_ref in,
		rcp_type_ref out_type, rcp_struct_ref out)
{
#ifdef RCP_SELF_TEST
	if (rcp_map_key_type(in) != rcp_string_type){
		rcp_error("map value type");
		return;
	}
	if (rcp_map_value_type(in) != rcp_ref_type){
		rcp_error("map value type");
		return;
	}
#endif

	rcp_map_node_ref node = rcp_map_begin(in);
	rcp_struct_param_ref param = rcp_struct_type_begin(out_type);
	while (1){
		int comp = rcp_compare(rcp_string_type, 
				rcp_map_node_key(in, node), 
				(rcp_data_ref)rcp_struct_param_name(param));

		if (comp < 0){
			node = rcp_map_node_next(node);
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
				*(rcp_record_ref*)rcp_map_node_value(in, node);
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
			node = rcp_map_node_next(node);
			param = rcp_struct_param_next(out_type, param);
			if (!node || !param)
				break;
		}
	}
}

void rcp_struct_to_map(rcp_type_ref in_type, 
		rcp_struct_ref in, rcp_map_ref out)
{
	rcp_struct_param_ref param = rcp_struct_type_begin(in_type);
	while (param){
		rcp_map_node_ref node = rcp_map_node_new(out);
		//key
		rcp_string_ref p_name = rcp_struct_param_name(param);
		rcp_copy(rcp_string_type, 
				(rcp_data_ref)p_name, rcp_map_node_key(out, node));
		//value
		rcp_type_ref type = rcp_struct_param_type(param);
		if (type == rcp_ref_type){
			rcp_copy(type,
					rcp_struct_data(in, param),
					rcp_map_node_value(out, node));
		}
		else{
			rcp_record_ref rec = rcp_record_new(type);
			rcp_copy(type,
					rcp_struct_data(in, param),
					rcp_record_data(rec));
			rcp_copy(rcp_ref_type,
					(rcp_data_ref)&rec,
					rcp_map_node_value(out, node));
			rcp_record_release(rec);
		}
		rcp_map_set(out, node);
		param = rcp_struct_param_next(in_type, param);
	}
}
