#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "../rcp_defines.h"
#include "../rcp_caster.h"

#include "../rcp_json_write.h"
#include "../rcp_send_as_command.h"

#define RCP_INTERNAL_STRUCTURE

#include "../rcp_type.h"
#include "../rcp_record.h"
#include "rcp_type_list.h"
#include "../rcp_tree.h"
#include "rcp_dict.h"

///
//type dict
//

//unused
struct rcp_type_core rcp_dict_type_def = {
	sizeof(struct rcp_tree_core),
	0,
	NULL,
	rcp_dict_init,//init
	rcp_dict_deinit,//free
	NULL,//copy
	NULL,//comp
	rcp_dict_write_json,
	rcp_dict_send_as_command,
	rcp_dict_set,
	NULL,
	rcp_dict_unset
};

rcp_type_ref rcp_dict_type_new(
		rcp_type_ref key_type, rcp_type_ref data_type)
{
	rcp_type_ref type = malloc(sizeof *type +
			sizeof (struct rcp_type_dict_ext));
	struct rcp_type_dict_ext* ext = 
		(struct rcp_type_dict_ext*)(type + 1);
	memcpy((void*)type, &rcp_dict_type_def, sizeof *type);
	ext->key_type = key_type;
	ext->data_type = data_type;
	return type;
}

void rcp_dict_type_delete(
		rcp_type_ref type)
{
	free((void*)type);
}

rcp_type_ref rcp_dict_type_key_type(rcp_type_ref type)
{
	struct rcp_type_dict_ext* ext = 
		(struct rcp_type_dict_ext*)(type + 1);
	return ext->key_type;
}
rcp_type_ref rcp_dict_type_data_type(rcp_type_ref type)
{
	struct rcp_type_dict_ext* ext = 
		(struct rcp_type_dict_ext*)(type + 1);
	return ext->data_type;
}

///
//dict itself
//

rcp_dict_ref rcp_dict_new(rcp_type_ref type)
{
	return (rcp_dict_ref)rcp_new(type);
}
void rcp_dict_delete(rcp_type_ref type, rcp_dict_ref data)
{
	rcp_delete(type,(rcp_data_ref)data);
}


///
// type class
void rcp_dict_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_type_ref key_type = rcp_dict_type_key_type(type);
	rcp_tree_init((rcp_tree_ref)data, (void*)key_type->compare, 
			(void*)key_type);
}
void rcp_dict_deinit(rcp_type_ref type, rcp_data_ref data)
{
	rcp_dict_node_ref node = rcp_dict_begin((rcp_dict_ref)data);
	while(node){
		rcp_dict_node_deinit(type, node);
		node = rcp_dict_node_next(node);
	}
	rcp_tree_deinit((rcp_tree_ref)data);
}
void rcp_dict_at(
		rcp_type_ref *io_type, rcp_data_ref *io_data,
		rcp_type_ref key_type, rcp_data_ref key_data)
{
	rcp_type_ref dict_type = *io_type;
	rcp_dict_ref dict = (rcp_dict_ref)*io_data;

	*io_type = NULL;
	*io_data = NULL;

	rcp_assert(key_type == rcp_dict_type_key_type(dict_type),
			"bad key type");

	rcp_dict_node_ref node;
	node = rcp_dict_find(dict, key_data);

	rcp_assert(node, "key is missing");

	*io_type = rcp_dict_type_data_type(dict_type);
	*io_data = rcp_dict_node_data(dict_type, node);
}

void rcp_dict_set(rcp_type_ref type, rcp_data_ref data,
		rcp_type_ref key_type, rcp_data_ref key_data,
		rcp_type_ref data_type, rcp_data_ref data_data)
{
	if (key_type != rcp_dict_type_key_type(type)){
		rcp_type_ref tgt_key_type = rcp_dict_type_key_type(type);
		rcp_data_ref tgt_key_data = rcp_cast_l1(
				key_type, key_data, tgt_key_type);

		rcp_dict_set(type, data, tgt_key_type, tgt_key_data,
				data_type, data_data);
		return;
	}
	if (data_type != rcp_dict_type_data_type(type)){
		rcp_type_ref tgt_data_type = rcp_dict_type_data_type(type);
		rcp_data_ref tgt_data_data = rcp_cast_l1(
				data_type, data_data, tgt_data_type);

		rcp_dict_set(type, data, key_type, key_data,
				tgt_data_type, tgt_data_data);
		return;
	}

	rcp_dict_ref dict = (rcp_dict_ref)data;

	rcp_dict_node_ref node = rcp_dict_node_new(type);
	rcp_copy(key_type,
			key_data,
			rcp_dict_node_key(type, node));

	rcp_copy(data_type,
			data_data,
			rcp_dict_node_data(type, node));

	rcp_dict_node_ref old_node = rcp_dict_set_node(dict, node);
	if (old_node)
		rcp_dict_node_delete(type, old_node);
}

void rcp_dict_unset(rcp_type_ref type, rcp_data_ref data,
		rcp_type_ref key_type, rcp_data_ref key_data)
{
	if (key_type != rcp_dict_type_key_type(type))
		return;

	rcp_dict_ref dict = (rcp_dict_ref)data;

	rcp_dict_node_ref node = rcp_dict_find(dict, key_data);
	if (node)
		rcp_dict_unset_node(dict, node);
}

int8_t rcp_dict_merge(
		rcp_type_ref type, rcp_data_ref target_data,
		rcp_data_ref input_data)
{
	rcp_dict_ref target_dict = (rcp_dict_ref) target_data;
	rcp_dict_ref input_dict = (rcp_dict_ref) input_data;

	rcp_dict_node_ref input_node = rcp_dict_begin(input_dict);
	
	while (input_node){
		rcp_dict_node_ref c_node = rcp_dict_node_clone(type, input_node);
		rcp_dict_node_ref old_node = rcp_dict_set_node(target_dict, c_node);
		rcp_dict_node_delete(type, old_node);
		input_node = rcp_dict_node_next(input_node);
	}
	
	return 0;
}

//type class end
///

rcp_extern 
rcp_dict_node_ref rcp_dict_find(
		rcp_dict_ref dict, rcp_data_ref key)
{
	return (rcp_dict_node_ref)rcp_tree_find((rcp_tree_ref)dict, key);
}

rcp_extern 
rcp_dict_node_ref rcp_dict_set_node(
		rcp_dict_ref dict, rcp_dict_node_ref node)
{
	return (rcp_dict_node_ref)rcp_tree_set(
			(rcp_tree_ref)dict, (rcp_tree_node_ref)node);
}

rcp_extern 
void rcp_dict_unset_node(rcp_dict_ref dict, rcp_dict_node_ref node)
{
	rcp_tree_remove((rcp_tree_ref)dict, (rcp_tree_node_ref)node);
}

///
//dict node
//

rcp_dict_node_ref rcp_dict_node_alloc(rcp_type_ref type)
{
	rcp_type_ref key_type = rcp_dict_type_key_type(type);
	rcp_type_ref data_type = rcp_dict_type_data_type(type);
	return (rcp_dict_node_ref)rcp_tree_node_new(
			key_type->size + data_type->size);
}

void rcp_dict_node_dealloc(rcp_dict_node_ref node)
{
	rcp_tree_node_delete((rcp_tree_node_ref)node);
}

rcp_extern 
void rcp_dict_node_init(rcp_type_ref type, rcp_dict_node_ref node)
{
	rcp_type_ref key_type = rcp_dict_type_key_type(type);
	rcp_type_ref data_type = rcp_dict_type_data_type(type);
	rcp_data_ref key_data = rcp_dict_node_key(type, node);
	rcp_data_ref data_data = rcp_dict_node_data(type, node);

	rcp_init(key_type, key_data);
	rcp_init(data_type, data_data);
}

rcp_extern 
void rcp_dict_node_deinit(rcp_type_ref type, rcp_dict_node_ref node)
{
	rcp_type_ref key_type = rcp_dict_type_key_type(type);
	rcp_type_ref data_type = rcp_dict_type_data_type(type);
	rcp_data_ref key_data = rcp_dict_node_key(type, node);
	rcp_data_ref data_data = rcp_dict_node_data(type, node);

	rcp_deinit(key_type, key_data);
	rcp_deinit(data_type, data_data);
}

rcp_extern 
rcp_dict_node_ref rcp_dict_node_new(rcp_type_ref type)
{
	rcp_dict_node_ref node = rcp_dict_node_alloc(type);
	rcp_dict_node_init(type, node);
	return node;
}

rcp_dict_node_ref rcp_dict_node_new_with(rcp_type_ref type,
		rcp_data_ref key, rcp_data_ref data)
{
	rcp_dict_node_ref node = rcp_dict_node_new(type);
	rcp_copy(rcp_dict_type_key_type(type),
			key,
			rcp_dict_node_key(type, node));
	rcp_copy(rcp_dict_type_data_type(type),
			data,
			rcp_dict_node_data(type, node));
	return node;
}

rcp_dict_node_ref rcp_dict_node_clone(
	rcp_type_ref type, rcp_dict_node_ref master_node)
{
	rcp_dict_node_ref node = rcp_dict_node_alloc(type);
	rcp_copy(rcp_dict_type_key_type(type),
			rcp_dict_node_key(type, master_node),
			rcp_dict_node_key(type, node));
	rcp_copy(rcp_dict_type_data_type(type),
			rcp_dict_node_data(type, master_node),
			rcp_dict_node_data(type, node));
	return node;
}

rcp_extern 
void rcp_dict_node_delete(rcp_type_ref type, rcp_dict_node_ref node)
{
	if (!node)
		return;
	rcp_dict_node_deinit(type, node);
	rcp_dict_node_dealloc(node);
}

rcp_extern 
rcp_data_ref rcp_dict_node_key(
		rcp_type_ref type, rcp_dict_node_ref node)
{
	return (rcp_data_ref)rcp_tree_node_data((rcp_tree_node_ref)node);
}

rcp_extern 
rcp_data_ref rcp_dict_node_data(
		rcp_type_ref type, rcp_dict_node_ref node)
{
	rcp_type_ref key_type = rcp_dict_type_key_type(type);
	return rcp_tree_node_data((rcp_tree_node_ref)node) + key_type->size; 
}

///
//iterate
rcp_extern 
rcp_dict_node_ref rcp_dict_begin(rcp_dict_ref dict)
{
	return (rcp_dict_node_ref)rcp_tree_begin((rcp_tree_ref)dict);
}

rcp_extern 
rcp_dict_node_ref rcp_dict_node_next(rcp_dict_node_ref node)
{
	return (rcp_dict_node_ref)rcp_tree_node_next((rcp_tree_node_ref)node);
}

