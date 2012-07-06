#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "rcp_alias.h"

#define RCP_INTERNAL_STRUCTURE

#include "../rcp_type.h"

struct rcp_alias_ext{
	rcp_type_ref real_type;
};

struct rcp_type_core rcp_alias_template = {
	0,
	20,
	"alias",
	NULL,//init
	NULL,//deinit
	NULL,//copy
	NULL,//comp
	NULL,//json
};

rcp_extern rcp_type_ref rcp_alias_type_new(rcp_type_ref real_type)
{
	rcp_type_ref core = malloc(sizeof *core + 
			sizeof (struct rcp_alias_ext));

	memcpy(core, real_type, sizeof (*core));
	core->size = real_type->size;

	if (real_type->init)
		core->init = rcp_alias_init;
	if (real_type->deinit)
		core->deinit = rcp_alias_deinit;
	if (real_type->copy)
		core->copy = rcp_alias_copy;
	if (real_type->compare)
		core->compare = rcp_alias_compare;
	if (real_type->write_json)
		core->write_json = rcp_alias_write_json;

	struct rcp_alias_ext *ext = (void*)(core + 1);
	ext->real_type = real_type;
	return core;
}

rcp_type_ref rcp_alias_real_type(rcp_type_ref type)
{
	return *(rcp_type_ref*)(type + 1);
}

void rcp_alias_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_type_ref real_type = rcp_alias_real_type(type);
	rcp_init(real_type, data);
}
void rcp_alias_deinit(rcp_type_ref type, rcp_data_ref data)
{
	rcp_type_ref real_type = rcp_alias_real_type(type);
	rcp_deinit(real_type, data);
}
void rcp_alias_copy(rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
{
	rcp_type_ref real_type = rcp_alias_real_type(type);
	rcp_copy(real_type, src, dst);
}
int rcp_alias_compare(rcp_type_ref type, rcp_data_ref l, rcp_data_ref r)
{
	rcp_type_ref real_type = rcp_alias_real_type(type);
	return rcp_compare(real_type, l, r);
}
void rcp_alias_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref out)
{
	rcp_type_ref real_type = rcp_alias_real_type(type);
	rcp_write_json(real_type, data, out);
}
