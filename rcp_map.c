#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"
#include "rcp_type.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_tree.h"
#include "rcp_map.h"

typedef struct rcp_map_core *rcp_map_core_ref;
struct rcp_map_core{
	rcp_type_ref key_type;
	rcp_type_ref value_type;
	struct rcp_tree t_core;
};

void rcp_type_map_free(void *val)
{
	rcp_map_core_ref core= val;
	rcp_tree_free(&(core->t_core));
}

struct rcp_type_core rcp_type_map = {
	sizeof(struct rcp_map_core),
	NULL,//init
	rcp_type_map_free,//free
	NULL,//copy
	NULL,//comp
};


void rcp_map_init(rcp_map_core_ref core, 
		rcp_type_ref key_type, rcp_type_ref value_type);

rcp_record_ref rcp_map_new(
		rcp_type_ref key_type, rcp_type_ref value_type)
{
	rcp_record_ref rec = rcp_record_new(&rcp_type_map);
	rcp_map_core_ref core = rcp_record_data(rec);
	rcp_map_init(core, key_type, value_type);
	return rec;
}

void rcp_map_init(rcp_map_core_ref core, 
		rcp_type_ref key_type, rcp_type_ref value_type)
{
	//rcp_map_core_ref core = rcp_record_data(map);
#ifdef RCP_SELF_TEST
	if ( ! key_type)
		rcp_error("map:key type");
	if ( ! value_type)
		rcp_error("map:value type");
#endif
	core->key_type = key_type;
	core->value_type = value_type;
	if (key_type)
		rcp_tree_init(&(core->t_core), key_type->compare);
	else
		rcp_error("key of map must be string or int");
}

rcp_map_node_ref rcp_map_find(rcp_map_ref map,void *key)
{
	rcp_map_core_ref core = rcp_record_data(map);
	return rcp_tree_find(&(core->t_core), key);
}

void rcp_map_set(rcp_map_ref map, rcp_map_node_ref node)
{
	rcp_map_core_ref core = rcp_record_data(map);
	rcp_tree_set(&(core->t_core),node);
}

rcp_extern rcp_map_node_ref rcp_map_root(rcp_map_ref map)
{
	rcp_map_core_ref core = rcp_record_data(map);
	rcp_tree_root(&(core->t_core));
}

rcp_extern rcp_type_ref rcp_map_key_type(rcp_map_ref map)
{
	rcp_map_core_ref core = rcp_record_data(map);
	return core->key_type;
}
rcp_extern rcp_type_ref rcp_map_value_type(rcp_map_ref map)
{
	rcp_map_core_ref core = rcp_record_data(map);
	return core->value_type;
}
//void rcp_map_unset(rcp_map_ref map, rcp_map_node_ref node)
//{
//	rcp_tree_delete(&(map->t_core),node);
//}


rcp_map_node_ref rcp_map_node_new(rcp_map_ref map)
{
	rcp_type_ref key_type = rcp_map_key_type(map);
	rcp_type_ref value_type = rcp_map_value_type(map);

	if (!(key_type)){
		rcp_error("type of key of map");
		return NULL;
	}
	if (!(value_type)){
		rcp_error("type of value of map");
		return NULL;
	}

	size_t s = 0;
	s += key_type->size;
	s += value_type->size;

	rcp_map_node_ref node = rcp_tree_node_new(s);
	
	if (key_type->init)
		key_type->init(rcp_map_node_key(map, node));
	if (value_type->init)
		value_type->init(rcp_map_node_value(map, node));

	return node;
}

void *rcp_map_node_key(
		rcp_map_ref map, rcp_map_node_ref node){
	return rcp_tree_node_data(node);
}
void *rcp_map_node_value(
		rcp_map_ref map, rcp_map_node_ref node){
	rcp_type_ref key_type = rcp_map_key_type(map);
	return rcp_map_node_key(map, node) + key_type->size;
}
rcp_extern rcp_map_node_ref rcp_map_node_next(rcp_map_node_ref node){
	return rcp_tree_node_next(node);
}
