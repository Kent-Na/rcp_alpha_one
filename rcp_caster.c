#include "rcp_pch.h"
#include "rcp_utility.h"

#include "rcp_caster.h"
#include "types/rcp_alias.h"

#define RCP_INTERNAL_STRUCTURE
#include "rcp_record.h"

void rcp_record_change_type(rcp_record_ref rec, rcp_type_ref dst_type)
{
	if (!rcp_alias_is(dst_type))
		return;
	if (rcp_record_type(rec) != rcp_alias_real_type(dst_type))
		return;
	rec->type = dst_type;
}
