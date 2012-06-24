//record structure
//type_ref[map] (record core)
//type_ref[key]
//type_ref[value]
//tree_core

//node structure
//node_core
//key
//value

typedef rcp_data_ref rcp_map_ref;

#ifdef RCP_INTERNAL_STRUCTURE
typedef rcp_tree_node_ref rcp_map_node_ref;
#else
typedef void *rcp_map_node_ref;
#endif

///
//type map
//
struct rcp_type_map_ext{
	rcp_type_ref key_type;
	rcp_type_ref data_type;
};

rcp_map_type_ref rcp_map_type_new(
		rcp_type_ref key_type, rcp_type_ref data_type);
rcp_map_type_ref rcp_map_type_delete(
		rcp_map_type_ref type)

rcp_type_ref rcp_map_type_key_type(rcp_map_type_ref type);
rcp_type_ref rcp_map_type_value_type(rcp_map_type_ref type);

///
//map itself
//
void rcp_map_init(rcp_type_ref type, rcp_data_ref data);
void rcp_map_deinit(rcp_type_ref type, rcp_data_ref data);

rcp_type_ref rcp_map_key_type(rcp_map_type_ref type);
rcp_type_ref rcp_map_data_type(rcp_map_type_ref type);

rcp_map_node_ref rcp_map_find(rcp_map_type_ref type, rcp_data_ref data);
rcp_data_ref rcp_map_at(rcp_map_type_ref type, rcp_data_ref data,
		rcp_data_ref key);

rcp_data_ref rcp_map_set(rcp_map_type_ref type, rcp_data_ref data,
		rcp_record_ref rec);
rcp_data_ref rcp_map_unset(rcp_map_type_ref type, rcp_data_ref data,
		rcp_record_ref rec);


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

rcp_record_ref rcp_map_new_rec(
		rcp_type_ref key_type, rcp_type_ref value_type);
