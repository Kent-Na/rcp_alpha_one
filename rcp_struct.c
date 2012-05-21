#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_string.h"
#include "rcp_type.h"
#include "rcp_struct.h"

rcp_record_ref rcp_record_new(rcp_type_ref type)
{
	rcp_record_ref rec = malloc(type->size + sizeof *rec);
	void *dat = rec + 1;

	if (type->init)
		type->init(dat);
}

rcp_type_ref rcp_record_type(rcp_record_ref rec)
{
	return rec->type;
}

void* rcp_record_data(rcp_record_ref rec)
{
	return rec + 1;
}
/*
rcp_record_ref rcp_structure_record_new(rcp_structure_ref st)
{
	struct rcp_structure_core *core = st;
	rcp_record_ref record = malloc(core->size +
			sizeof (struct rcp_record_core));
	struct rcp_record_core *r_core = record;
	r_core->structure = st;
	r_core->record_id = 0;
	
	return record;
}

rcp_record_ref rcp_structure_record_clone(rcp_record_ref src_record)
{
	struct rcp_record_core *r_core = src_record;
	struct rcp_structure_core *core = r_core->structure;
	size_t r_size = core->size + sizeof (struct rcp_record_core);
	rcp_record_ref dst_record = malloc(r_size);
	memcpy(dst_record, src_record, r_size);
	return dst_record;
}


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
