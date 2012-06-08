#include "rcp_pch.h"
#include "rcp_utility.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"
#include "rcp_type_list.h"

extern void rcp_uint8_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);

const struct rcp_type_core rcp_bool8_type_def = {
	sizeof(uint8_t),
	RCP_TYPE_BOOL8,
	"bool8",
	NULL,//init
	NULL,//free
	rcp_uint8_type_copy,
	NULL,//comp
};

extern void rcp_uint32_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);

const struct rcp_type_core rcp_bool32_type_def = {
	sizeof(uint32_t),
	RCP_TYPE_BOOL32,
	"bool32",
	NULL,//init
	NULL,//free
	rcp_uint32_type_copy,
	NULL,//comp
};

const struct rcp_type_core rcp_null_type_def = {
	0,
	RCP_TYPE_NULL,
	"null",
	NULL,//init
	NULL,//free
	NULL,//copy
	NULL,//comp
};

void rcp_ref_type_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_record_ref *rec = data;
	*rec = NULL;
}

void rcp_ref_type_deinit(rcp_type_ref type, rcp_data_ref data)
{
	rcp_record_ref *rec = data;
	rcp_record_release(*rec);
}

void rcp_ref_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
{
	rcp_record_ref *src_rec = src;
	rcp_record_ref *dst_rec = dst;
	rcp_record_retain(*src_rec);
	*dst_rec = *src_rec;
}

const struct rcp_type_core rcp_ref_type_def = {
	sizeof (rcp_record_ref),
	RCP_TYPE_REF,
	"ref",
	rcp_ref_type_init,//init
	rcp_ref_type_deinit,//deinit
	rcp_ref_type_copy,//copy
	NULL,//comp
};
