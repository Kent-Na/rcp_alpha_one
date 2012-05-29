#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_type.h"
#include "rcp_array.h"

struct rcp_array_core{
	rcp_type_ref type;
	void *data;
	size_t size;//allocated data count
	size_t count;//used data count
};


void rcp_type_array_init(void *data){
	rcp_array_core_ref core = data;
	core->data = NULL;
	core->size = 0;
	core->count = 0;
	core->type = NULL;
}

void rcp_type_array_free(void *data){
	int i;
	rcp_array_core_ref core = data;
	if (core->type->free){
		for (i = 0; i< core->count; i++){
			core->type->free(core->data + core->type->size * i);
		}
	}

	free(core->data);
	core->data = NULL;
	core->size = 0;
	core->count = 0;
	core->type = NULL;
}

rcp_extern struct rcp_type_core rcp_type_array = {
	sizeof (struct rcp_array_core),//size
	rcp_type_array_init,//init
	rcp_type_array_free,//free
	NULL,//copy
	NULL,//comp
};

rcp_extern rcp_array_ref rcp_array_new(rcp_type_ref type)
{
	rcp_array_ref array = rcp_record_new(&rcp_type_array);
	rcp_array_core_ref core = rcp_record_data(array);
	core->type = type;
}

rcp_extern size_t rcp_array_count(rcp_array_ref array){
	rcp_array_core_ref core = rcp_record_data(array);
	return core->count;
}
rcp_extern void* rcp_array_data(rcp_array_ref array, size_t index){
	rcp_array_core_ref core = rcp_record_data(array);
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
		rcp_error("array:type");
		return NULL;
	}
#endif
	return core->data + core->type->size * index;
}
rcp_extern void rcp_array_append(rcp_array_ref array, void *data)
{

	rcp_array_core_ref core = rcp_record_data(array);
#ifdef RCP_SELF_TEST
	if (core->count > core->size){
		rcp_error("array:size");
		return;
	}
#endif
	if (core->count==core->size){
		core->data = realloc(core->data, core->type->size * core->size *2);
		core->size *= 2;
	}
	core->type->copy(data, core->data + core->type->size * core->count);
	core->count ++;
}
