#include "rcp_pch.h"
#include "rcp_utility.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_string.h"
#include "rcp_struct.h"

void rcp_struct_type_init(rcp_type_ref type, rcp_data_ref data){
	struct rcp_struct_core *s_core;	
	struct rcp_struct_parameter *parameters;	

	int i;

	for (i = 0; i<s_core->parameter_count; i++){
		rcp_struct_parameter_ref param = parameters+i;
		rcp_type_ref type = param->type;
		if (type->init)
			type->init(type, data);
	}
}

void rcp_struct_type_deinit(rcp_type_ref type, rcp_data_ref data){
	struct rcp_struct_core *s_core;	
	struct rcp_struct_parameter *parameters;	

	int i;

	for (i = 0; i<s_core->parameter_count; i++){
		rcp_struct_parameter_ref param = parameters+i;
		rcp_type_ref type = param->type;
		if (type->deinit)
			type->deinit(type, data);
	}
}

void rcp_struct_type_copy(rcp_type_ref type, 
		rcp_data_ref src, rcp_data_ref dst){
	struct rcp_struct_core *s_core;	
	struct rcp_struct_parameter *parameters;	

	int i;

	for (i = 0; i<s_core->parameter_count; i++){
		rcp_struct_parameter_ref param = parameters+i;
		rcp_type_ref type = param->type;
		if (type->copy)
			type->copy(type, src+param->offset, dst+param->offset);
		else if (type->init)
			type->init(type, dst+param->offset);
	}
}

struct rcp_type_core rcp_struct_type_def = {
	sizeof(struct rcp_struct_core),
	RCP_TYPE_MAP,
	"pm_task",
	rcp_struct_type_init,//init
	rcp_struct_type_deinit,//free
	NULL,//copy
	NULL,//comp
};
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
