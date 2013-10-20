#include "rcp_pch.h"
#include "rcp_utility.h"

#include "rcp_caster.h"
#include "types/rcp_alias.h"
#include "types/rcp_type_list.h"

#define RCP_INTERNAL_STRUCTURE
#include "types/rcp_record.h"

rcp_data_ref rcp_cast_l1(rcp_type_ref src_type, rcp_data_ref src_data,
		rcp_type_ref dst_type)
{
	if (src_type == rcp_ref_type){
		rcp_record_ref rec = *(rcp_record_ref*)src_data;
		if (rcp_record_type(rec) == dst_type)
			return rcp_record_data(rec);
		return NULL;
	}
	return NULL;
}	
