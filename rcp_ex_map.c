#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_tree.h"
#include "rcp_dict.h"

typedef rcp_data_ref rcp_dict_ref;

#ifdef RCP_INTERNAL_STRUCTURE
typedef rcp_tree_node_ref rcp_dict_node_ref;
#else
typedef void *rcp_dict_node_ref;
#endif

///
//type map
//
struct rcp_type_map_ext{
	rcp_type_ref key_type;
	rcp_type_ref data_type;
};

struct rcp_type_core rcp_dict_type_def = {
	sizeof(struct rcp_tree),
	RCP_TYPE_MAP,
	"map",
	rcp_dict_init,//init
	rcp_dict_deinit,//free
	NULL,//copy
	NULL,//comp
};

rcp_dict_type_ref rcp_dict_type_new(
		rcp_type_ref key_type, rcp_type_ref data_type)
{
	rcp_type_ref type = malloc(sizeof *type +
			(struct rcp_type_map_ext))	
	struct rcp_type_map_ext* ext = 
		(struct rcp_type_map_ext*)(type + 1);
	memcpy(type, rcp_dict_type_def, sizeof *type);
	ext->key_type = key_type;
	ext->data_type = data_type;
}

rcp_dict_type_ref rcp_dict_type_delete(
		rcp_dict_type_ref type)
{
	free(type);
}

rcp_type_ref rcp_dict_type_key_type(rcp_dict_type_ref type)
{
	struct rcp_type_map_ext* ext = 
		(struct rcp_type_map_ext*)(type + 1);
	return ext->key_type;
}
rcp_type_ref rcp_dict_type_value_type(rcp_dict_type_ref type)
{
	struct rcp_type_map_ext* ext = 
		(struct rcp_type_map_ext*)(type + 1);
	return ext->data_type;
}

///
//map itself
//

void rcp_dict_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_tree_init(data, type->compare, 
			rcp_dict_type_key_type(type));
}
void rcp_dict_deinit(rcp_type_ref type, rcp_data_ref data)
{
	rcp_dict_node_ref node = rcp_dict_begin(data);
	while(node){
		rcp_dict_node_deinit(type, node);
		node = rcp_dict_node_next(node);
	}
	rcp_tree_deinit(data);
}

rcp_data_ref rcp_dict_at(rcp_dict_type_ref type, rcp_data_ref data,
		rcp_data_ref key)
{
	rcp_dict_node_ref node = rcp_dict_find(data, key);
	return rcp_dict_node_data(type, node);
}

rcp_data_ref rcp_dict_set(rcp_dict_type_ref type, rcp_data_ref data,
		rcp_record_ref rec);
rcp_data_ref rcp_dict_unset(rcp_dict_type_ref type, rcp_data_ref data,
		rcp_record_ref rec);

rcp_extern 
rcp_dict_node_ref rcp_dict_find(
		rcp_dict_ref map, rcp_data_ref key)
{
	return rcp_tree_find(map, key);
}

rcp_extern 
rcp_dict_node_ref rcp_dict_set_node(
		rcp_dict_ref map, rcp_dict_node_ref node)
{
	return rcp_tree_set(map, node);
}

rcp_extern 
void rcp_dict_unset_node(rcp_dict_ref map, rcp_dict_node_ref node)
{
	return rcp_tree_remove(map, node);
}

///
//map node
//

rcp_dict_node_ref rcp_dict_node_alloc(rcp_dict_type_ref type)
{
	rcp_type_ref key_type = rcp_dict_type_key_type(type);
	rcp_type_ref data_type = rcp_dict_type_data_type(type);
	return rcp_tree_node_new(key_type->size + data_type->size);
}

void rcp_dict_node_dealloc(rcp_dict_nede_ref node)
{
	rcp_tree_node_delete(node);
}

rcp_extern 
void rcp_dict_node_init(rcp_dict_type_ref type, rcp_dict_node_ref node)
{
	rcp_type_ref key_type = rcp_dict_type_key_type(type);
	rcp_type_ref data_type = rcp_dict_type_data_type(type);
	rcp_data_ref key_data = rcp_dict_node_key(type, node);
	rcp_data_ref data_data = rcp_dict_node_data(type, node);

	rcp_init(key_type, key_data);
	rcp_init(data_type, data_data);
}

rcp_extern 
void rcp_dict_node_deinit(rcp_dict_type_ref type, rcp_dict_node_ref node)
{
	rcp_type_ref key_type = rcp_dict_type_key_type(type);
	rcp_type_ref data_type = rcp_dict_type_data_type(type);
	rcp_data_ref key_data = rcp_dict_node_key(type, node);
	rcp_data_ref data_data = rcp_dict_node_data(type, node);

	rcp_deinit(key_type, key_data);
	rcp_deinit(data_type, data_data);
}

rcp_extern 
rcp_dict_node_ref rcp_dict_node_new(rcp_dict_type_ref type)
{
	rcp_dict_node_ref node = rcp_dict_node_alloc(type);
	rcp_dict_node_init(type, node);
	return node;
}

rcp_extern 
void rcp_dict_node_delete(rcp_dict_type_ref type, rcp_dict_node_ref node)
{
	rcp_dict_node_deinit(type, node);
	rcp_dict_node_dealloc(node);
}

rcp_extern 
rcp_data_ref rcp_dict_node_key(
		rcp_dict_type_ref type, rcp_dict_node_ref node)
{
	return rcp_tree_node_data(node);
}

rcp_extern 
rcp_data_ref rcp_dict_node_value(
		rcp_dict_type_ref type, rcp_dict_node_ref node)
{
	rcp_type_ref key_type = rcp_dict_type_key_type(type);
	return rcp_tree_node_data(node) + type->size; 
}

///
//iterate
rcp_extern 
rcp_dict_node_ref rcp_dict_begin(rcp_dict_ref map)
{
	return rcp_tree_begin(map);
}

rcp_extern 
rcp_dict_node_ref rcp_dict_node_next(rcp_dict_node_ref node)
{
	return rcp_tree_node_next(node);
}

