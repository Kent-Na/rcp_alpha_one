#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_type.h"
rcp_record_ref rcp_record_new(rcp_type_ref type)
{
	rcp_record_ref rec = malloc(type->size + sizeof *rec);
	rec->type = type;
	if (type->init)
		type->init(rcp_record_data(rec));
	rec->ref_count = 1;
	return rec;
}

void rcp_record_delete(rcp_record_ref rec);
void rcp_record_retain(rcp_record_ref rec)
{
	rec->ref_count ++;
}
void rcp_record_release(rcp_record_ref rec)
{
	if (!rec)
		return;
	rec->ref_count --;
	if (!rec->ref_count)
		rcp_record_delete(rec);
}

void rcp_record_delete(rcp_record_ref rec)
{
	if (!rec)
		return;
	if (rec->type->free)
		rec->type->free(rcp_record_data(rec));
	free(rec);
}

rcp_type_ref rcp_record_type(rcp_record_ref rec)
{
	return rec->type;
}

void* rcp_record_data(rcp_record_ref rec)
{
	return rec + 1;
}

void rcp_move(rcp_type_ref type, void *src, void *dst)
{
	if (type->free)
		type->free(dst);
	memcpy(dst, src, type->size);
	if (type->init)
		type->init(src);
}
void rcp_copy(rcp_type_ref type, void *src, void *dst)
{
	if (! type->copy){
		rcp_error("can not copy that type");
		return;
	}
	type->copy(src, dst);
}
void rcp_swap(rcp_type_ref type, void *src, void *dst)
{
	void *tmp = malloc(type->size);
	memcpy(tmp, src, type->size);
	memcpy(src, dst, type->size);
	memcpy(dst, tmp, type->size);
	free(tmp);
}
