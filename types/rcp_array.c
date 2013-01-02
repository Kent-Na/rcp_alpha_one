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

void rcp_array_set(rcp_type_ref type, rcp_data_ref data,
		rcp_type_ref key_type, rcp_data_ref key_data,
		rcp_type_ref data_type, rcp_data_ref data_data)
{
	rcp_array_ref array = (rcp_array_ref)data;
	if (rcp_array_type_data_type(type)!= data_type)
		return;

	if (key_type == rcp_ref_type){
		rcp_record_ref key_rec = *(rcp_record_ref*)key_data;
		key_type = rcp_record_type(key_rec);
		key_data = rcp_record_data(key_rec);
	}

	if (!rcp_type_is_uint(key_type))
		return;

	uint64_t idx = rcp_uint_as_uint(key_type, key_data);

	rcp_data_ref out = rcp_array_data_at(type, array, idx);
	if (!out)
		return;
		
	rcp_deinit(data_type, out);
	rcp_copy(data_type, data_data, out);
}

rcp_extern void rcp_array_append(
		rcp_type_ref array_type, rcp_data_ref array,
		rcp_type_ref data_type, rcp_data_ref data)
{
	if (rcp_array_type_data_type(array_type)!= data_type)
		return;
	rcp_array_append_data(array_type, (rcp_array_ref)array, data);
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

	if (rcp_type_is_uint(key_type)){
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

rcp_extern int8_t rcp_array_replace(
	rcp_type_ref array_type, rcp_data_ref target_array_data,
	int32_t range_begin, int32_t range_end,
	rcp_data_ref input_array_data)
{
	rcp_array_ref target_array = (rcp_array_ref)target_array_data;
	rcp_array_ref input_array = (rcp_array_ref)input_array_data;
	rcp_type_ref data_type = rcp_array_type_data_type(array_type);
	int i;

	if (!rcp_array_owning_data(target_array)){
		rcp_error("array append");
		return -1;
	}

	if (range_begin < 0)
		range_begin = target_array->data_count+1+range_begin;
	if (range_end < 0)
		range_end = target_array->data_count+1+range_end;

	if (range_begin>range_end)
			return -1;
	if (range_end>target_array->data_count)
		return -1;

	size_t total_data_count= target_array->data_count -
		(range_end-range_begin)+input_array->data_count;
	rcp_array_resize(array_type, target_array, total_data_count);

	//move back part
	size_t move_dst_idx = range_begin+input_array->data_count;
	int32_t move_count = target_array->data_count - range_end;
	memmove(target_array->array+move_dst_idx*data_type->size,
			target_array->array+range_end*data_type->size,
			move_count*data_type->size);

	//insert input
	if (input_array->data_count){
		/*
		memcpy(target_array->array+range_begin*data_type->size,
				input_array->array,
				input_array->data_count*data_type->size);
		for (int i = 0; i<target_array->data_count; i++){
			rcp_copied(data_type, 
				target_array->array+(range_begin+i)*data_type->size)
		}
		*/
		for (i = 0; i<input_array->data_count; i++){
			rcp_copy(data_type, 
				input_array->array+i*data_type->size,
				target_array->array+(range_begin+i)*data_type->size);
		}
	}

	target_array->data_count = total_data_count;
	return 0;
}

void rcp_array_resize(
	rcp_type_ref array_type, rcp_array_ref array, size_t new_size)
{
	rcp_type_ref data_type = rcp_array_type_data_type(array_type);
	const size_t initial_capacity = 16;
	const size_t block_size = 4096;

	if (array->capacity >= new_size)
		return;

	//data count
	size_t new_capacity = array->capacity;
	if (new_capacity< initial_capacity)
		new_capacity= initial_capacity;

	while (new_capacity < new_size){
		if (new_capacity*2 > block_size)
			break;
		new_capacity *= 2;
	}

	if (new_capacity < new_size)
		new_capacity = new_size;

	//bytes
	size_t new_data_size = new_capacity*data_type->size;
	if (new_data_size > block_size){
		new_data_size = (new_data_size/block_size+1)*block_size;
		new_capacity = new_data_size/data_type->size;
	}

	array->array = realloc(array->array, new_data_size);
	array->capacity = new_capacity;
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
rcp_extern rcp_data_ref rcp_array_last(
		rcp_type_ref array_type, rcp_array_ref array)
{
	if (!array->data_count)
		return NULL;
	return array->array +
		rcp_array_type_data_type(array_type)->size*(array->data_count-1);
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
