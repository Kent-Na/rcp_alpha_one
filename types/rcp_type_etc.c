#include "../rcp_pch.h"
#include "../rcp_utility.h"

#define RCP_INTERNAL_STRUCTURE

#include "../rcp_type.h"
#include "../rcp_record.h"
#include "rcp_type_list.h"


void rcp_bool8_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(uint8_t*)dst = *(uint8_t*)src;
}
void rcp_bool32_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(uint32_t*)dst = *(uint32_t*)src;
}


void rcp_ref_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_record_ref *rec = (rcp_record_ref*)data;
	*rec = NULL;
}

void rcp_ref_deinit(rcp_type_ref type, rcp_data_ref data)
{
	rcp_record_ref *rec = (rcp_record_ref*)data;
	rcp_record_release(*rec);
}

void rcp_ref_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
{
	rcp_record_ref *src_rec = (rcp_record_ref*)src;
	rcp_record_ref *dst_rec = (rcp_record_ref*)dst;
	rcp_record_retain(*src_rec);
	*dst_rec = *src_rec;
}

void rcp_ref_set(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref key_type, rcp_data_ref key_data,
		rcp_type_ref data_type, rcp_data_ref data_data)
{
	rcp_record_ref rec = *(rcp_record_ref*)dst;
	rcp_set(rcp_record_type(rec), rcp_record_data(rec),
			key_type, key_data, data_type, data_data);
}

void rcp_ref_unset(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref key_type, rcp_data_ref key_data)
{
	rcp_record_ref rec = *(rcp_record_ref*)dst;
	rcp_unset(rcp_record_type(rec), rcp_record_data(rec),
			key_type, key_data);
}

void rcp_ref_append(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref data_type, rcp_data_ref data_data)
{
	rcp_record_ref rec = *(rcp_record_ref*)dst;
	rcp_append(rcp_record_type(rec), rcp_record_data(rec),
			data_type, data_data);
}

void rcp_ref_at(rcp_type_ref *io_type, rcp_data_ref *io_data,
		rcp_type_ref key_type, rcp_data_ref key_data)
{
	rcp_record_ref rec = *(rcp_record_ref*)*io_data;
	*io_type = rcp_record_type(rec);
	*io_data = rcp_record_data(rec);
	rcp_at(io_type, io_data, key_type, key_data);
}
