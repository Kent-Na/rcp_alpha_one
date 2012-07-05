#include "../rcp_pch.h"
#include "../rcp_defines.h"
#include "../rcp_utility.h"
#include "../rcp_record.h"
#include "rcp_type_list.h"

#define RCP_INTERNAL_STRUCTURE

#include "../rcp_type.h"
#include "rcp_array.h"


rcp_extern rcp_record_ref rcp_array_new_rec(rcp_type_ref type)
{
	rcp_record_ref array = rcp_record_new(rcp_array_type);
	rcp_array_ref core = (rcp_array_ref)rcp_record_data(array);
	rcp_array_init_with_type(core, type);
	return array;
}

void rcp_array_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_array_ref core = (rcp_array_ref)data;
	core->data = NULL;
	core->size = 0;
	core->count = 0;
	core->type = NULL;
}
void rcp_array_init_with_type(void *data, rcp_type_ref type)
{
	rcp_array_ref core = data;
	core->data = NULL;
	core->size = 0;
	core->count = 0;
	core->type = type;
}

void rcp_array_deinit(rcp_type_ref type, rcp_data_ref data)
{
	int i;
	rcp_array_ref core = (rcp_array_ref)data;
	if (core->type && core->type->deinit){
		for (i = 0; i< core->count; i++){
			core->type->deinit(
					core->type,core->data + core->type->size * i);
		}
	}

	free(core->data);
}

rcp_extern rcp_array_ref rcp_array_new(rcp_type_ref type)
{
	rcp_array_ref ar = (rcp_array_ref)rcp_alloc(rcp_array_type);
	rcp_array_init_with_type(ar, type);
	return ar;
}
rcp_extern void rcp_array_delete(rcp_array_ref data)
{
	rcp_delete(rcp_array_type, (rcp_data_ref)data);
}

rcp_extern size_t rcp_array_count(rcp_array_ref array){
	rcp_array_ref core = array;
	return core->count;
}
rcp_extern size_t rcp_array_empty(rcp_array_ref array){
	rcp_array_ref core = array;
	return !core->count;
}
rcp_extern rcp_type_ref rcp_array_data_type(rcp_array_ref array)
{
	rcp_array_ref core = array;
	return core->type;
}
rcp_extern rcp_data_ref rcp_array_data(rcp_array_ref array, size_t index){
	rcp_array_ref core = array;
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
rcp_extern void rcp_array_append(rcp_array_ref array, void *data)
{
	rcp_array_ref core = array;
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

rcp_extern void rcp_array_pop(rcp_array_ref array, void *data){
	rcp_array_ref core = array;
	if (! core->count)
		return;
	core->count --;
	rcp_data_ref back = core->data + core->type->size * core->count;
	core->type->copy(
			core->type, back, data);
	rcp_deinit(core->type, back);
}

rcp_extern rcp_array_iterater_ref rcp_array_begin(rcp_array_ref array)
{
	rcp_array_ref core = array;
	if (core->count)
		return core->data;
	return NULL;
}
rcp_extern rcp_array_iterater_ref rcp_array_iterater_next(
		rcp_array_ref array, rcp_array_iterater_ref itr)
{
	rcp_array_ref core = array;
	void* ret = (void*)itr + core->type->size;
	if (ret < core->data + core->type->size * core->count)
		return (rcp_array_iterater_ref)ret;
	return NULL;
}
rcp_extern rcp_data_ref rcp_array_iterater_data(
		rcp_array_ref array, rcp_array_iterater_ref itr)
{
	return (rcp_data_ref)itr;
}
