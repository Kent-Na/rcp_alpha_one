#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"
#include "types/rcp_string.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"

rcp_extern void rcp_type_delete(rcp_type_ref type)
{
	rcp_string_delete(type->type_name);
	free(type);
}
rcp_extern void rcp_type_set_name(rcp_type_ref type, rcp_string_ref str)
{
	if (type->type_name && type->type_name != str)
		rcp_string_delete(type->type_name);
	type->type_name = str;
}
rcp_extern rcp_string_ref rcp_type_name(rcp_type_ref type)
{
	return type->type_name;
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

rcp_extern int rcp_compare(rcp_type_ref type, 
		rcp_data_ref l, rcp_data_ref r)
{
	return type->compare(type, l, r);
}
rcp_extern void rcp_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref str)
{
	type->write_json(type, data, str);	
}
rcp_extern void rcp_set(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref key_type, rcp_data_ref key_data,
		rcp_type_ref data_type, rcp_data_ref data_data)
{
	type->set(type, dst, key_type, key_data, data_type, data_data);
}
rcp_extern void rcp_append(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref data_type, rcp_data_ref data_data)
{
	type->append(type, dst, data_type, data_data);
}
rcp_extern void rcp_unset(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref key_type, rcp_data_ref key_data)
{
	type->unset(type, dst, key_type, key_data);
}
