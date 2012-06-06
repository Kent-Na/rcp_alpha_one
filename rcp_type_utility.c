#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"
#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_string.h"
#include "rcp_map.h"
#include "rcp_struct.h"

rcp_extern rcp_record_ref rcp_map_find_c_str(
		rcp_map_ref map, const char *key, rcp_type_ref type)
{

#ifdef RCP_SELF_TEST
	if (rcp_map_key_type(map) != rcp_string_type){
		rcp_error("map value type");
		return;
	}
	if (rcp_map_value_type(map) != rcp_ref_type){
		rcp_error("map value type");
		return;
	}
#endif

	rcp_string_ref str = rcp_string_new(key);
	rcp_map_node_ref node = rcp_map_find(map, rcp_record_data(str));
	rcp_record_delete(str);
	if (!node){
		rcp_error("not found");
		return NULL;
	}
	rcp_record_ref rec = *(rcp_record_ref*)rcp_map_node_value(map, node);
	if (type && rcp_record_type(rec) != type)
		return NULL;
	return rec;
}
/*
rcp_extern rcp_struct_ref rcp_map_to_struct(
		rcp_type_ref type, rcp_map_ref map){
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

	rcp_map_node_ref node = rcp_map_begin(map);
	rcp_struct_parameter_ref param;
	rcp_struct_parameter_ref end_param;
	while (1){
		int comp = rcp_compair(rcp_string_type, 
				rcp_map_node_key(map, node), 
				rcp_struct_parameter_name(param));

		if (comp < 0){
			node = rcp_map_node_next(node);
			if (node)
				continue;
			break;
		}
		else if (comp > 0){
			param = rcp_struct_parameter_next(param);
			if (param != end_param)
				continue;
			break;
		}
		else{
			rcp_record_ref node_data = 
				(rcp_record_ref)rcp_map_node_value(map, node);
			rcp_type_ref type = rcp_record_type(node_data);
			if (type  == rcp_struct_parameter_type(param)){
				rcp_data_ref src = rcp_record_data(node_data);
				rcp_data_ref dst = rcp_struct_data(param);
				rcp_move(type, src, dst);
			}
			node = rcp_map_node_next(node);
			param = rcp_struct_parameter_next(param);
			if (node && param != end_param)
				continue;
			break;
		}
	}
}

rcp_extern rcp_struct_ref rcp_struct_to_map(rcp_struct_ref st)
{
	rcp_type_ref s_type = rcp_record_type(st);
	rcp_map_ref map = rcp_map_new(rcp_string_type, rcp_ref_type);

	rcp_struct_parameter_ref param;
	while (param){
		rcp_map_node_ref node = rcp_map_node_new(map);
		rcp_data_ref p_name = rcp_struct_parameter_name(param);
		rcp_copy(rcp_string_type, p_name, rcp_map_node_key(map, node));
		rcp_copy(rcp_struct_parameter_type(param),
				rcp_struct_parameter_data(st, param)
				rcp_map_node_value(map, node));
		rcp_map_set(node);
		param = rcp_struct_parameter_next(s_type, param);
	}
}
*/
