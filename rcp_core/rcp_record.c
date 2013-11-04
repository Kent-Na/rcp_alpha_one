#include "rcp_type_pch.h"
#include "rcp_utility.h"

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
	rec->frags = 0;
	return rec;
}

rcp_extern rcp_record_ref rcp_record_new_with(
		rcp_type_ref type, rcp_data_ref data)
{
	if (!data)
		return NULL;
	rcp_record_ref rec = malloc(type->size + sizeof *rec);
	rec->type = type;
	rcp_copy(type, data, rcp_record_data(rec));
	rec->ref_count = 1;
	rec->frags = 0;
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
	if (rec->frags & RCP_REC_FRAG_TAGED)
		rcp_error("releasing taged record");
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
	return (rcp_data_ref)(rec + 1);
}

uint32_t rcp_record_ref_count(rcp_record_ref rec)
{
	return rec->ref_count;
}

void rcp_record_tag(rcp_record_ref rec)
{
	rec->frags |= RCP_REC_FRAG_TAGED;
}
