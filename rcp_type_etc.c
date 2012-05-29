#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_type.h"

void rcp_uint8_type_copy(void *src, void *dst);

struct rcp_type_core rcp_bool8_type = {
	sizeof(uint8_t),
	NULL,//init
	NULL,//free
	rcp_uint8_type_copy,
	NULL,//comp
};

void rcp_uint32_type_copy(void *src, void *dst);

struct rcp_type_core rcp_bool32_type = {
	sizeof(uint32_t),
	NULL,//init
	NULL,//free
	rcp_uint32_type_copy,
	NULL,//comp
};

struct rcp_type_core rcp_type_null = {
	0,
	NULL,//init
	NULL,//free
	NULL,//copy
	NULL,//comp
};

void rcp_ref_type_init(void *data)
{
	rcp_record_ref *rec = data;
	*rec = NULL;
}

void rcp_ref_type_free(void *data)
{
	rcp_record_ref *rec = data;
	rcp_record_release(*rec);
	*rec = NULL;
}

void rcp_ref_type_copy(void *src, void *dst)
{
	rcp_record_ref *src_rec = src;
	rcp_record_ref *dst_rec = dst;
	rcp_record_retain(*src_rec);
	rcp_ref_type_free(dst);
	*dst_rec = *src_rec;
}

struct rcp_type_core rcp_ref_type = {
	sizeof (rcp_record_ref),
	rcp_ref_type_init,//init
	rcp_ref_type_free,//free
	rcp_ref_type_copy,//copy
	NULL,//comp
};
