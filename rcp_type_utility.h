#include "def/rcp_map.h"
#include "def/rcp_type.h"
#include "def/rcp_struct.h"
#include "def/rcp_record.h"
rcp_extern rcp_record_ref rcp_map_find_c_str(
		rcp_map_ref map, const char *key, rcp_type_ref type);

rcp_extern void rcp_map_to_struct(rcp_map_ref in,
		rcp_type_ref out_type, rcp_struct_ref out);
rcp_extern void rcp_struct_to_map(rcp_type_ref in_type, 
		rcp_struct_ref in, rcp_map_ref out);
