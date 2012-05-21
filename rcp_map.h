
struct rcp_map_core{
	struct rcp_record_core r_core;
	rcp_type_ref key_type;
	rcp_type_ref value_type;
	struct rcp_tree_core t_core;
};

void rcp_map_init(rcp_map_ref map, 
		rcp_type_ref key_type, rcp_type_ref value_type)
{
	map->r_core->type = &rcp_type_map;
	map->key_type = key_type;
	map->value_type = value_type;
	if (key_type)
		rcp_tree_init(&(map->t_core), key_type->compare);
	else
		rcp_error("key of map must be string or int");
}

void rcp_map_set(rcp_map_ref map,);

void rcp_map_add(rcp_tree_ref tree,){
	rcp_type_ref key_type;
	rcp_type_ref value_type;
}

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

struct rcp_type_core rcp_type_map = {

};

