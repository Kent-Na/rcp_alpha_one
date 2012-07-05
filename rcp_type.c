#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"

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
void rcp_move(rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
{
	rcp_deinit(type, dst);
	memcpy(dst, src, type->size);
}

void rcp_copy(rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
{
	if (! type->copy){
		rcp_error("can not copy that type");
		return;
	}
	type->copy(type, src, dst);
}

void rcp_swap(rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
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
rcp_extern void rcp_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref str)
{
	type->write_json(type, data, str);	
}
