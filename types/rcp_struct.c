#include "../rcp_pch.h"
#include "../rcp_utility.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"
#include "rcp_string.h"
#include "rcp_struct.h"
#include "rcp_type_list.h"

rcp_type_ref rcp_struct_type_new(uint16_t param_count){
	return malloc(sizeof (struct rcp_type_core) + 
			sizeof (struct rcp_type_struct_ext) + 
			sizeof (struct rcp_struct_param_core)*param_count);
}
void rcp_struct_init(rcp_type_ref type, rcp_data_ref data){
	struct rcp_type_struct_ext *ext = (void*)(type + 1);
	rcp_struct_param_ref params = (void*)(ext + 1);

	int i;

	for (i = 0; i<ext->param_count; i++){
		rcp_struct_param_ref param = params+i;
		rcp_type_ref type = param->type;
		if (type->init)
			type->init(type, (void*)data+param->offset);
	}
}

void rcp_struct_deinit(rcp_type_ref type, rcp_data_ref data){
	struct rcp_type_struct_ext *ext = (void*)(type + 1);
	rcp_struct_param_ref params = (void*)(ext + 1);

	int i;

	for (i = 0; i<ext->param_count; i++){
		rcp_struct_param_ref param = params+i;
		rcp_type_ref type = param->type;
		if (type->deinit)
			type->deinit(type, (void*)data+param->offset);
	}
}

void rcp_struct_copy(rcp_type_ref type, 
		rcp_data_ref src, rcp_data_ref dst){
	struct rcp_type_struct_ext *ext = (void*)(type + 1);
	rcp_struct_param_ref params = (void*)(ext + 1);

	int i;

	for (i = 0; i<ext->param_count; i++){
		rcp_struct_param_ref param = params+i;
		rcp_type_ref type = param->type;
		if (type->copy)
			type->copy(type, 
					(void*)src+param->offset, (void*)dst+param->offset);
		else if (type->init)
			type->init(type, (void*)dst+param->offset);
	}
}

rcp_data_ref rcp_struct_data(rcp_struct_ref st, rcp_struct_param_ref param)
{
	return ((void*)st) + param->offset;
}

rcp_struct_param_ref rcp_struct_type_begin(rcp_type_ref type)
{
	struct rcp_type_struct_ext *ext = (void*)(type + 1);
	rcp_struct_param_ref params = (void*)(ext + 1);
	return params;
}
rcp_string_ref rcp_struct_param_name(rcp_struct_param_ref param)
{
	return param->name;	
}
rcp_type_ref rcp_struct_param_type(rcp_struct_param_ref param)
{
	return param->type;	
}
rcp_struct_param_ref rcp_struct_param_next(
		rcp_type_ref type, rcp_struct_param_ref param)
{
	struct rcp_type_struct_ext *ext = (void*)(type + 1);
	rcp_struct_param_ref params = (void*)(ext + 1);
	rcp_struct_param_ref next = param+1;
	if (next < params + ext->param_count)
		return next;
	return NULL;
}


rcp_struct_param_ref rcp_parameter_from_str(
		rcp_type_ref type, rcp_string_ref  str)
{
	struct rcp_type_struct_ext *ext = (void*)(type + 1);
	rcp_struct_param_ref param_base = rcp_struct_type_begin(type);
	size_t min = 0;
	size_t max = ext->param_count - 1;

	//binaly search

	while (max >= min){
		size_t mid = (min+max)>>1;
		rcp_struct_param_ref param_mid = param_base + mid;
		int cmp = rcp_compare(rcp_string_type, 
				(rcp_data_ref)param_mid->name,(rcp_data_ref)str);

		if (cmp<0)
			min = mid+1;
		else if (cmp>0)
			max = mid-1;
		else
			return param_mid;
	}

	return NULL;
}
