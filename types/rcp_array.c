#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "../def/rcp_array_iterater.h"
#include "../def/rcp_array.h"

#define RCP_INTERNAL_STRUCTURE
#include "../rcp_type.h"
#include "rcp_array.h"

struct rcp_array_core{
	void* array;
	size_t data_count;

	//Allocated memory size.
	//If capacity = 0 and array = not NULL, array was externaly allocated.
	//Don't modefy and release array in this case.
	size_t capacity;
};

struct rcp_type_core rcp_array_type_def = {
	sizeof(struct rcp_array_core),
	0, //type_id
	NULL,//type_name
	rcp_array_init,
	rcp_array_deinit,
	rcp_array_copy,
	NULL,//compare
	//rcp_array_write_json,
	//rcp_array_send_as_command,
	//rcp_array_set,
	//rcp_array_append,
	//rcp_dict_unset,
	NULL,//at
};
	
rcp_type_ref rcp_array_type_data_type(rcp_type_ref type)
{
	return ((struct rcp_type_array_ext*)(type+1))->data_type;
}

rcp_array_ref rcp_array_new(rcp_type_ref array_type)
{
	return (rcp_array_ref)rcp_new(array_type);
}
rcp_extern void rcp_array_delete(
		rcp_type_ref array_type, rcp_array_ref array)
{
	rcp_delete(array_type, (rcp_data_ref)array);
}

//arraying type
void rcp_array_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_array_ref core = (rcp_array_ref)data;
	core->array = NULL;
	core->data_count = 0;
	core->capacity = 0;
}
void rcp_array_deinit(rcp_type_ref type, rcp_data_ref data)
{
	rcp_array_ref core = (rcp_array_ref)data;
	if (!rcp_array_owning_data(core))
		return;
	rcp_type_ref data_type = rcp_array_type_data_type(type);
	void* end = core->array+core->data_count*data_type->size;
	void* ptr = core->array;
	for ( ; ptr<end; ptr += data_type->size)
		rcp_deinit(data_type, ptr);
	if (core->capacity)
		free(core->array);
}
void rcp_array_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
{
	rcp_type_ref data_type = rcp_array_type_data_type(type);
	rcp_array_ref src_core = (rcp_array_ref)src;
	rcp_array_ref dst_core = (rcp_array_ref)dst;

	rcp_init(type, dst);//not required
	dst_core->array = malloc(type->size*src_core->data_count);
	dst_core->data_count = src_core->data_count;
	dst_core->capacity = src_core->capacity;

	int i;
	for (i = 0; i<src_core->data_count; i++){
		size_t offset = i*data_type->size;
		rcp_data_ref src_data = src_core->array+offset;
		rcp_data_ref dst_data = dst_core->array+offset;
		rcp_copy(data_type, src_data, dst_data);
	}
}

rcp_extern int rcp_array_owning_data(rcp_array_ref array)
{
	if (! array->array)
		return 1;
	if (array->capacity)
		return 0;
	return 0;
}

void* rcp_array_raw_data(rcp_array_ref array)
{
	rcp_array_ref core = array;
	return core->array;
}
size_t rcp_array_len(rcp_array_ref array){
	rcp_array_ref core = array;
	return core->data_count;
}

rcp_extern void rcp_array_append_data(
		rcp_type_ref array_type, rcp_array_ref array,
		rcp_data_ref data)
{
	rcp_type_ref data_type = rcp_array_type_data_type(array_type);
	if (!rcp_array_owning_data(array)){
		rcp_error("array append");
		return;
	}
	if (array->capacity == array->data_count){
		size_t storage_size = 16;
		size_t block_size = (1<<12);
		if (array->capacity>=block_size){
			storage_size = (array->capacity&(~4095))+block_size;
		}
		else if (array->capacity){
			storage_size *= 2;
		}
		array->array = realloc(array->array, storage_size);
	}
	void* dst = array->array+array->data_count*data_type->size;
	rcp_copy(data_type, data, dst);
	array->data_count++;
}

rcp_array_iterater_ref rcp_array_begin(
		rcp_type_ref array_type, rcp_array_ref array){
	if (array->data_count)
		return array->array;
	return NULL;
}
rcp_array_iterater_ref rcp_array_iterater_next(
		rcp_type_ref array_type, rcp_array_ref array, 
		rcp_array_iterater_ref itr)
{
	rcp_type_ref data_type = rcp_array_type_data_type(array_type);
	void* end = array->array+array->data_count*data_type->size;
	void* next = (void*)itr+data_type->size;
	if (next<end)
		return next;
	return NULL;
}
rcp_data_ref rcp_array_iterater_data(rcp_array_iterater_ref itr)
{
	return (rcp_data_ref)itr;
}
