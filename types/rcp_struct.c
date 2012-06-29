#include "../rcp_pch.h"
#include "../rcp_utility.h"

#define RCP_INTERNAL_STRUCTURE

#include "../rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_string.h"
#include "rcp_struct.h"

rcp_struct_type_ref rcp_struct_type_new(uint16_t param_count){
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
			type->init(type, data+param->offset);
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
			type->deinit(type, data+param->offset);
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
			type->copy(type, src+param->offset, dst+param->offset);
		else if (type->init)
			type->init(type, dst+param->offset);
	}
}
/*
struct rcp_type_core rcp_struct_type_def = {
	sizeof(struct rcp_struct_core),
	RCP_TYPE_MAP,
	"pm_task",
	rcp_struct_init,//init
	rcp_struct_deinit,//free
	rcp_struct_copy,//copy
	NULL,//comp
};
*/

rcp_data_ref rcp_struct_data(rcp_struct_ref st, rcp_struct_param_ref param)
{
	return ((void*)st) + param->offset;
}

rcp_struct_param_ref rcp_struct_type_begin(rcp_struct_type_ref type)
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
		rcp_struct_type_ref type, rcp_struct_param_ref param)
{
	struct rcp_type_struct_ext *ext = (void*)(type + 1);
	rcp_struct_param_ref params = (void*)(ext + 1);
	rcp_struct_param_ref next = param+1;
	if (next < params + ext->param_count)
		return next;
	return NULL;
}

/*
struct rcp_structure_parameter* rcp_parameter_from_str(
		rcp_structure_ref st, const char* str)
{
	struct rcp_structure_core *core = st;
	struct rcp_structure_parameter *p = 
		st + sizeof (struct rcp_structure_core);

	//binaly search

	size_t min = 0;
	size_t max = core->parameter_count-1;

	while (max >= min){
		size_t mid = (min+max)>>1;
		int cmp = strcmp(p[mid].name, str);

		if (cmp<0)
			min = mid+1;
		else if (cmp>0)
			max = mid-1;
		else
			return p + mid;
	}

	return NULL;
}
*/
