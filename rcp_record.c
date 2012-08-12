#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_record.h"
#include "rcp_type.h"

rcp_extern void rcp_record_delete(rcp_record_ref);

rcp_record_ref rcp_record_new(rcp_type_ref type)
{
	rcp_record_ref rec = malloc(type->size + sizeof *rec);
	rec->type = type;
	if (type->init)
		type->init(type, rcp_record_data(rec));
	rec->ref_count = 1;
	return rec;
}

rcp_record_ref rcp_record_retain(rcp_record_ref rec)
{
	if (!rec)
		return NULL;
	rec->ref_count ++;
	return rec;
}

void rcp_record_release(rcp_record_ref rec)
{
	if (!rec)
		return;
	rec->ref_count --;
	if (!rec->ref_count)
		rcp_record_delete(rec);
}

rcp_extern void rcp_record_init(rcp_record_ref rec)
{
	rcp_init(rcp_record_type(rec), rcp_record_data(rec));
}
rcp_extern void rcp_record_deinit(rcp_record_ref rec)
{
	rcp_deinit(rcp_record_type(rec), rcp_record_data(rec));
}

void rcp_record_delete(rcp_record_ref rec)
{
	if (!rec)
		return;
	if (rec->type->deinit)
				rec->type->deinit(rec->type, rcp_record_data(rec));
		free(rec);
}

rcp_type_ref rcp_record_type(rcp_record_ref rec)
{
	return rec->type;
}

rcp_data_ref rcp_record_data(rcp_record_ref rec)
{
#ifdef RCP_SELF_TEST
	if (!rec){
		rcp_caution("request null rec data");
			return NULL;
	}
#endif
	return (rcp_data_ref)(rec + 1);
}

