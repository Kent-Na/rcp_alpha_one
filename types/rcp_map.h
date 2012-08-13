#include "../def/rcp_type.h"
#include "../def/rcp_map.h"
#include "../def/rcp_map_node.h"
#include "../rcp_record.h"

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_map_core{
	rcp_type_ref key_type;
	rcp_type_ref value_type;
	struct rcp_tree_core t_core;
};
#endif

rcp_record_ref rcp_map_new_rec(
		rcp_type_ref key_type, rcp_type_ref value_type);

rcp_extern rcp_map_ref rcp_map_new(
		rcp_type_ref key_type, rcp_type_ref value_type);
rcp_extern void rcp_map_delete(
		rcp_map_ref map);

void rcp_map_init(rcp_type_ref type, rcp_data_ref data);
void rcp_map_deinit(rcp_type_ref type, rcp_data_ref data);

void rcp_map_init_with_type(rcp_map_ref core, 
		rcp_type_ref key_type, rcp_type_ref value_type);
rcp_extern rcp_type_ref rcp_map_key_type(rcp_map_ref map);
rcp_extern rcp_type_ref rcp_map_value_type(rcp_map_ref map);
rcp_extern rcp_map_node_ref rcp_map_find(rcp_map_ref map,void *key);

//return value: old value
rcp_extern rcp_map_node_ref rcp_map_set(
		rcp_map_ref map, rcp_map_node_ref node);
rcp_extern void rcp_map_unset(rcp_map_ref map, rcp_map_node_ref node);
rcp_extern rcp_map_node_ref rcp_map_begin(rcp_map_ref map);

rcp_extern rcp_map_node_ref rcp_map_node_new(rcp_map_ref map);
rcp_extern void rcp_map_node_delete(rcp_map_ref map, rcp_map_node_ref node);
rcp_extern rcp_data_ref rcp_map_node_key(
		rcp_map_ref map, rcp_map_node_ref node);
rcp_extern rcp_data_ref rcp_map_node_value(
		rcp_map_ref map, rcp_map_node_ref node);
rcp_extern rcp_map_node_ref rcp_map_node_next(rcp_map_node_ref node);

