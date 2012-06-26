#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_tree.h"
#include "rcp_map.h"

typedef struct rcp_map_core *rcp_map_core_ref;
struct rcp_map_core{
	rcp_type_ref key_type;
	rcp_type_ref value_type;
	struct rcp_tree t_core;
};

rcp_map_ref rcp_map_new(
		rcp_type_ref key_type, rcp_type_ref value_type)
{
	rcp_data_ref map = rcp_alloc(rcp_map_type);
	rcp_map_init_with_type(map, key_type, value_type);
	return map;
}

void rcp_map_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_map_core_ref core = data;
	core->key_type = NULL;
	core->value_type = NULL;
	core->t_core.root = NULL;
}

void rcp_map_deinit(rcp_type_ref type, rcp_data_ref data)
{
	rcp_map_core_ref core = data;
	rcp_map_node_ref node = rcp_map_begin(data);
	while (node){
		rcp_type_ref key_type = rcp_map_key_type(core);
		rcp_type_ref value_type = rcp_map_value_type(core);
		rcp_deinit(key_type, rcp_map_node_key(core, node));
		rcp_deinit(value_type, rcp_map_node_value(core, node));
		node = rcp_map_node_next(node);
	}
	rcp_tree_deinit(&(core->t_core));
}

struct rcp_type_core rcp_map_type_def = {
	sizeof(struct rcp_map_core),
	RCP_TYPE_MAP,
	"map",
	rcp_map_init,//init
	rcp_map_deinit,//free
	NULL,//copy
	NULL,//comp
};


void rcp_map_init_with_type(rcp_map_ref map, 
		rcp_type_ref key_type, rcp_type_ref value_type)
{
	rcp_map_core_ref core = map;
#ifdef RCP_SELF_TEST
	if ( ! key_type)
		rcp_error("map:key type");
	if ( ! value_type)
		rcp_error("map:value type");
#endif
	core->key_type = key_type;
	core->value_type = value_type;
	if (key_type)
		rcp_tree_init(&(core->t_core), 
				(void*)key_type->compare, (void*)key_type);
	else
		rcp_error("key of map must be string or int");
}

rcp_map_node_ref rcp_map_find(rcp_map_ref map,void *key)
{
	rcp_map_core_ref core = map;
	return rcp_tree_find(&(core->t_core), key);
}

rcp_map_node_ref rcp_map_set(rcp_map_ref map, rcp_map_node_ref node)
{
	rcp_map_core_ref core = map;
	return rcp_tree_set(&(core->t_core),node);
}

rcp_extern rcp_map_node_ref rcp_map_begin(rcp_map_ref map)
{
	rcp_map_core_ref core = map;
	return rcp_tree_begin(&(core->t_core));
}
rcp_extern rcp_type_ref rcp_map_key_type(rcp_map_ref map)
{
	rcp_map_core_ref core = map;
	return core->key_type;
}
rcp_extern rcp_type_ref rcp_map_value_type(rcp_map_ref map)
{
	rcp_map_core_ref core = map;
	return core->value_type;
}

void rcp_map_at(rcp_type_ref type, rcp_map_ref map, 
		rcp_data_ref *io_data)
{
	rcp_assert(type == rcp_map_type, "type err");

	rcp_data_ref in_data = *io_data;

	rcp_map_node_ref out = rcp_map_find(map, in_data);
	if (!out){
		*io_data = NULL;
		return;
	}

	*io_data = rcp_map_node_value(map, out);
	return;
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

	rcp_init(key_type, rcp_map_node_key(map, node));
	rcp_init(value_type, rcp_map_node_value(map, node));

	return node;
}

rcp_extern void rcp_map_node_delete(rcp_map_ref map, rcp_map_node_ref node)
{
	if (!node)
		return;

	rcp_type_ref key_type = rcp_map_key_type(map);
	rcp_type_ref value_type = rcp_map_value_type(map);

	if (!(key_type)){
		rcp_error("type of key of map");
		return;
	}
	if (!(value_type)){
		rcp_error("type of value of map");
		return;
	}

	rcp_deinit(key_type, rcp_map_node_key(map, node));
	rcp_deinit(value_type, rcp_map_node_value(map, node));

	rcp_tree_node_delete(node);
}
rcp_data_ref rcp_map_node_key(
		rcp_map_ref map, rcp_map_node_ref node){
	return rcp_tree_node_data(node);
}
rcp_data_ref rcp_map_node_value(
		rcp_map_ref map, rcp_map_node_ref node){
	rcp_type_ref key_type = rcp_map_key_type(map);
	return rcp_map_node_key(map, node) + key_type->size;
}
rcp_extern rcp_map_node_ref rcp_map_node_next(rcp_map_node_ref node){
	return rcp_tree_node_next(node);
}

rcp_record_ref rcp_map_new_rec(
		rcp_type_ref key_type, rcp_type_ref value_type)
{
	rcp_record_ref rec = rcp_record_new(rcp_map_type);
	rcp_map_core_ref core = rcp_record_data(rec);
	rcp_map_deinit(rcp_map_type, core);
	rcp_map_init_with_type(core, key_type, value_type);
	return rec;
}
