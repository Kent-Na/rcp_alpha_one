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
