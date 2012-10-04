#include "../rcp_pch.h"
#include "../rcp_utility.h"

#define RCP_INTERNAL_STRUCTURE
#include "../rcp_type.h"
#include "rcp_array.h"

#include "rcp_type_list.h"
#include "rcp_type_utility.h"
#include "../rcp_record.h"

struct rcp_type_core rcp_array_type_def = {
	sizeof(struct rcp_array_core),
	0, //type_id
	NULL,//type_name
	rcp_array_init,
	rcp_array_deinit,
	rcp_array_copy,
	NULL,//compare
	NULL,//rcp_array_write_json,
	NULL,//rcp_array_send_as_command,
	NULL,//rcp_array_set,
	NULL,//rcp_array_append,
	NULL,//rcp_dict_unset,
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
	rcp_array_clear_data(type, core);
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

void rcp_array_at(
		rcp_type_ref *io_type, rcp_data_ref *io_data,
		rcp_type_ref key_type, rcp_data_ref key_data)
{
	rcp_type_ref array_type= *io_type;
	rcp_array_ref array= (rcp_array_ref)*io_data;

	*io_data = NULL;
	*io_type = NULL;

	rcp_assert(array_type, "null_type");
	rcp_assert(array, "null_data");

	if (key_type == rcp_ref_type){
		rcp_record_ref key_rec = *(rcp_record_ref*)key_data;
		key_type = rcp_record_type(key_rec);
		key_data = rcp_record_data(key_rec);
	}

	if (rcp_type_is_int(key_type)){
		int64_t idx = rcp_int_as_int(key_type, key_data);
		rcp_assert(idx<0, "idx error");

		*io_data = rcp_array_data_at(array_type, array, idx);
		*io_type = rcp_array_type_data_type(array_type);

		return;
	}

	else if (rcp_type_is_uint(key_type)){
		uint64_t idx = rcp_uint_as_uint(key_type, key_data);

		*io_data = rcp_array_data_at(array_type, array, idx);
		*io_type = rcp_array_type_data_type(array_type);

		return;
	}
}
rcp_extern int rcp_array_owning_data(rcp_array_ref array)
{
	if (array->array && array->capacity == 0 && array->data_count != 0)
		return 0;
	return 1;
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
		size_t storage_size = array->capacity;
		size_t block_size = (1<<8);//256 data/block
		if (!storage_size)
			storage_size = 32;
		else if (storage_size <= block_size)
			storage_size *= 2;
		else
			storage_size = (array->capacity+block_size)&(~block_size);

		array->array = realloc(array->array, storage_size*data_type->size);
		array->capacity = storage_size;
	}
	void* dst = array->array+array->data_count*data_type->size;
	rcp_copy(data_type, data, dst);
	array->data_count++;
}

rcp_extern void rcp_array_clear_data(
		rcp_type_ref array_type, rcp_array_ref array)
{
	if (!rcp_array_owning_data(array))
		return;
	rcp_type_ref data_type = rcp_array_type_data_type(array_type);
	void* end = array->array+array->data_count*data_type->size;
	void* ptr = array->array;
	for ( ; ptr<end; ptr += data_type->size)
		rcp_deinit(data_type, ptr);
	array->data_count = 0;
}
rcp_extern rcp_data_ref rcp_array_data_at(
		rcp_type_ref array_type, rcp_array_ref array,
		uint64_t idx)
{
	if (idx>=array->data_count)
		return NULL;
	return array->array +
		rcp_array_type_data_type(array_type)->size*idx;
}
rcp_array_iterater_ref rcp_array_begin(rcp_array_ref array)
{
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
