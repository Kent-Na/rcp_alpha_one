#include "rcp_type_pch.h"
#include "rcp_utility.h"
#include "rcp_alias.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"

struct rcp_alias_ext{
	rcp_type_ref real_type;
};

rcp_extern rcp_type_ref rcp_alias_type_new(rcp_type_ref real_type)
{
    const size_t alias_type_size = 
        sizeof (struct rcp_type_core) + sizeof (struct rcp_alias_ext);

	rcp_type_ref core = malloc(alias_type_size);
	memcpy(core, real_type, sizeof (*core));

	core->size = real_type->size;
	//core->type_id = 
	//core->type_name = 

	struct rcp_alias_ext *ext = (void*)(core + 1);
	ext->real_type = real_type;
	return core;
}

rcp_type_ref rcp_alias_real_type(rcp_type_ref type)
{
	return *(rcp_type_ref*)(type + 1);
}
