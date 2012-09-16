#include "../rcp_pch.h"
#include "../rcp_defines.h"
#include "../rcp_utility.h"
#include "../rcp_record.h"
#include "rcp_type_list.h"

#define RCP_INTERNAL_STRUCTURE

#include "../rcp_type.h"
#include "rcp_old_array.h"
#include "rcp_type_utility.h"


rcp_extern rcp_record_ref rcp_old_array_new_rec(rcp_type_ref type)
{
	rcp_record_ref array = rcp_record_new(rcp_old_array_type);
	rcp_old_array_ref core = (rcp_old_array_ref)rcp_record_data(array);
	rcp_old_array_init_with_type(core, type);
	return array;
}

void rcp_old_array_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_old_array_ref core = (rcp_old_array_ref)data;
	core->data = NULL;
	core->size = 0;
	core->count = 0;
	core->type = NULL;
}
void rcp_old_array_init_with_type(void *data, rcp_type_ref type)
{
	rcp_old_array_ref core = data;
	core->data = NULL;
	core->size = 0;
	core->count = 0;
	core->type = type;
}

void rcp_old_array_deinit(rcp_type_ref type, rcp_data_ref data)
{
	int i;
	rcp_old_array_ref core = (rcp_old_array_ref)data;
	if (core->type && core->type->deinit){
		for (i = 0; i< core->count; i++){
			core->type->deinit(
					core->type,core->data + core->type->size * i);
		}
	}

	free(core->data);
}

void rcp_old_array_append(rcp_type_ref type, rcp_data_ref data,
		rcp_type_ref data_type, rcp_data_ref data_data)
{
	rcp_old_array_ref array = (rcp_old_array_ref)data;

	if (data_type != rcp_old_array_data_type(array))
		return;

	rcp_old_array_append_data(array, data_data);
}

void rcp_old_array_set(rcp_type_ref type, rcp_data_ref data,
		rcp_type_ref key_type, rcp_data_ref key_data,
		rcp_type_ref data_type, rcp_data_ref data_data)
{
	rcp_old_array_ref array = (rcp_old_array_ref)data;
	if (rcp_old_array_data_type(array) != data_type)
		return;

	if (key_type == rcp_ref_type){
		rcp_record_ref key_rec = *(rcp_record_ref*)key_data;
		key_type = rcp_record_type(key_rec);
		key_data = rcp_record_data(key_rec);
	}

	if (!rcp_type_is_uint(key_type))
		return;

	uint64_t idx = rcp_uint_as_uint(key_type, key_data);
	if (idx >= array->count)
		return;

	rcp_data_ref out = array->data + data_type->size*idx;
		
	rcp_deinit(data_type, out);
	rcp_copy(data_type, data_data, out);
}

rcp_extern rcp_old_array_ref rcp_old_array_new(rcp_type_ref type)
{
	rcp_old_array_ref ar = (rcp_old_array_ref)rcp_alloc(rcp_old_array_type);
	rcp_old_array_init_with_type(ar, type);
	return ar;
}
rcp_extern void rcp_old_array_delete(rcp_old_array_ref data)
{
	rcp_delete(rcp_old_array_type, (rcp_data_ref)data);
}

rcp_extern size_t rcp_old_array_count(rcp_old_array_ref array){
	rcp_old_array_ref core = array;
	return core->count;
}
rcp_extern size_t rcp_old_array_empty(rcp_old_array_ref array){
	rcp_old_array_ref core = array;
	return !core->count;
}
rcp_extern rcp_type_ref rcp_old_array_data_type(rcp_old_array_ref array)
{
	rcp_old_array_ref core = array;
	return core->type;
}
rcp_extern rcp_data_ref rcp_old_array_data(rcp_old_array_ref array, size_t index){
	rcp_old_array_ref core = array;
#ifdef RCP_SELF_TEST
	if (!core->data){
		rcp_error("array:index");
		return NULL;
	}
	if (index >= core->size){
		rcp_error("array:index");
		return NULL;
	}
	if (!core->type){
		rcp_error("array:no type");
		return NULL;
	}
#endif
	return (rcp_data_ref)(core->data + core->type->size * index);
}
rcp_extern void rcp_old_array_append_data(rcp_old_array_ref array, void *data)
{
	rcp_old_array_ref core = array;
#ifdef RCP_SELF_TEST
	if (!core->type){
		rcp_error("array:no type");
		return;
	}
#endif
	if (core->count==core->size){
		size_t storage_size = core->size * 2;
		if (!storage_size)
			storage_size = 16;
		core->data = realloc(core->data, core->type->size * storage_size);
		core->size = storage_size;
	}
	core->type->copy(
			core->type, data, core->data + core->type->size * core->count);
	core->count ++;
}

rcp_extern void rcp_old_array_pop(rcp_old_array_ref array, void *data){
	rcp_old_array_ref core = array;
	if (! core->count)
		return;
	core->count --;
	rcp_data_ref back = core->data + core->type->size * core->count;
	core->type->copy(
			core->type, back, data);
	rcp_deinit(core->type, back);
}

rcp_extern rcp_old_array_iterater_ref rcp_old_array_begin(rcp_old_array_ref array)
{
	rcp_old_array_ref core = array;
	if (core->count)
		return core->data;
	return NULL;
}
rcp_extern rcp_old_array_iterater_ref rcp_old_array_iterater_next(
		rcp_old_array_ref array, rcp_old_array_iterater_ref itr)
{
	rcp_old_array_ref core = array;
	void* ret = (void*)itr + core->type->size;
	if (ret < core->data + core->type->size * core->count)
		return (rcp_old_array_iterater_ref)ret;
	return NULL;
}
rcp_extern rcp_data_ref rcp_old_array_iterater_data(
		rcp_old_array_ref array, rcp_old_array_iterater_ref itr)
{
	return (rcp_data_ref)itr;
}
