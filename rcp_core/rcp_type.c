#include "rcp_type_pch.h"
#include "rcp_utility.h"
#include "rcp_string.h"

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

void rcp_default_init(rcp_type_ref type, rcp_data_ref data){ }
void rcp_default_deinit(rcp_type_ref type, rcp_data_ref data){ }
void rcp_default_copied(rcp_type_ref type, rcp_data_ref data){ }
int  rcp_default_compare(
        rcp_type_ref type, rcp_data_ref l, rcp_data_ref r){ 
    rcp_error("missing implementation of \"compare\".");
    return 0;
}
void rcp_default_write_json(
        rcp_type_ref type, rcp_data_ref data, rcp_string_ref out){ }
void rcp_default_at(rcp_type_ref *io_type, rcp_data_ref *io_data,
        rcp_type_ref key_type, rcp_data_ref key_data)
{
    *io_type = NULL;
    *io_data = NULL;
}
int8_t rcp_default_replace(rcp_type_ref type, rcp_data_ref target,
        int32_t begin, int32_t end, rcp_data_ref input)
{
    return -1;
}
int8_t rcp_default_merge(rcp_type_ref type, rcp_data_ref target,
        rcp_data_ref input)
{ 
    return -1;
}
void rcp_default_unset(rcp_type_ref type, rcp_data_ref dst,
        rcp_type_ref key_type, rcp_data_ref key_data){ }

void rcp_move(rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
{
	rcp_deinit(type, dst);
	memcpy(dst, src, type->size);
}

void rcp_copy(rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
{
    memcpy(dst, src, type->size);
	if (! type->copied)
		return;
	type->copied(type, dst);
}

rcp_extern void rcp_init(rcp_type_ref type,
		rcp_data_ref data)
{
    type->init(type, data);
}

rcp_extern void rcp_deinit(rcp_type_ref type,
		rcp_data_ref data)
{
    type->deinit(type, data);
}


rcp_extern void rcp_copied(rcp_type_ref type,
		rcp_data_ref data)
{
    type->copied(type, data);
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
rcp_extern void rcp_unset(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref key_type, rcp_data_ref key_data)
{
	type->unset(type, dst, key_type, key_data);
}
rcp_extern void rcp_at(rcp_type_ref *io_type, rcp_data_ref *io_data,
			rcp_type_ref key_type, rcp_data_ref key_data)
{
	(*io_type)->at(io_type, io_data, key_type, key_data);
}
rcp_extern int8_t rcp_replace(rcp_type_ref type, rcp_data_ref target,
		int32_t begin, int32_t end, rcp_data_ref input)
{
	return type->replace(type, target, begin, end, input);
}
rcp_extern int8_t rcp_merge(rcp_type_ref type, rcp_data_ref target,
		rcp_data_ref input)
{
	return type->merge(type, target, input);
}
