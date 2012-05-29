//record structure
//type_ref[map] (record core)
//type_ref[key]
//type_ref[value]
//tree_core

//node structure
//node_core
//type_ref[key] if key type is any
//key
//type_ref[value] if value type is any
//value

typedef rcp_record_ref rcp_map_ref;
//rcp_map_ref safely cast to rcp_record_ref

#ifdef RCP_INTERNAL_STRUCTURE
typedef rcp_tree_node_ref rcp_map_node_ref;
#else
typedef void *rcp_map_node_ref;
#endif

rcp_extern struct rcp_type_core rcp_type_map;

rcp_extern rcp_record_ref rcp_map_new(
		rcp_type_ref key_type, rcp_type_ref value_type);
rcp_extern rcp_type_ref rcp_map_key_type(rcp_map_ref map);
rcp_extern rcp_type_ref rcp_map_value_type(rcp_map_ref map);
//rcp_extern void rcp_map_init(rcp_map_ref map, 
//		rcp_type_ref key_type, rcp_type_ref value_type);
rcp_extern rcp_map_node_ref rcp_map_find(rcp_map_ref map,void *key);
rcp_extern void rcp_map_set(rcp_map_ref map, rcp_map_node_ref node);
rcp_extern void rcp_map_unset(rcp_map_ref map, rcp_map_node_ref node);
rcp_extern rcp_map_node_ref rcp_map_root(rcp_map_ref map);

rcp_extern rcp_map_node_ref rcp_map_node_new(rcp_map_ref map);
rcp_extern void *rcp_map_node_key(
		rcp_map_ref map, rcp_map_node_ref node);
rcp_extern void *rcp_map_node_value(
		rcp_map_ref map, rcp_map_node_ref node);
rcp_extern rcp_map_node_ref rcp_map_node_next(rcp_map_node_ref node);
