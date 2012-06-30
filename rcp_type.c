#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#define RCP_INTERNAL_STRUCTURE

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

void* rcp_record_data(rcp_record_ref rec)
{
#ifdef RCP_SELF_TEST
	if (!rec){
		rcp_caution("request null rec data");
		return NULL;
	}
#endif
	return rec + 1;
}

rcp_extern rcp_data_ref rcp_new(rcp_type_ref type)
{
	rcp_data_ref data = rcp_alloc(type);
	rcp_init(type, data);
	return data;
}
rcp_extern void rcp_delete(rcp_type_ref type, rcp_data_ref data)
{
	rcp_deinit(type, data);
	rcp_dealloc(data);
}

rcp_extern rcp_data_ref rcp_alloc(rcp_type_ref type)
{
	return malloc(type->size);
}
rcp_extern void rcp_dealloc(rcp_data_ref data)
{
	free(data);
}
rcp_extern void rcp_init(rcp_type_ref type,
		rcp_data_ref data)
{
	if (type->init)
		type->init(type, data);
}

rcp_extern void rcp_deinit(rcp_type_ref type,
		rcp_data_ref data)
{
	if (type->deinit)
		type->deinit(type, data);
}
void rcp_move(rcp_type_ref type, void *src, void *dst)
{
	rcp_deinit(type, dst);
	memcpy(dst, src, type->size);
}

void rcp_copy(rcp_type_ref type, void *src, void *dst)
{
	if (! type->copy){
		rcp_error("can not copy that type");
		return;
	}
	type->copy(type, src, dst);
}

void rcp_swap(rcp_type_ref type, void *src, void *dst)
{
	void *tmp = malloc(type->size);
	memcpy(tmp, src, type->size);
	memcpy(src, dst, type->size);
	memcpy(dst, tmp, type->size);
	free(tmp);
}

rcp_extern int rcp_compair(rcp_type_ref type, 
		rcp_data_ref l, rcp_data_ref r)
{
	return type->compare(type, l, r);
}
