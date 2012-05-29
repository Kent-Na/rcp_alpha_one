#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"
#include "rcp_type.h"
#include "rcp_type_etc.h"
#include "rcp_string.h"
#include "rcp_map.h"

rcp_extern rcp_record_ref rcp_map_find_c_str(
		rcp_map_ref map, const char *key, rcp_type_ref type)
{

#ifdef RCP_SELF_TEST
	if (rcp_map_key_type(map) != &rcp_string_type){
		rcp_error("map value type");
		return;
	}
	if (rcp_map_value_type(map) != &rcp_ref_type){
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

