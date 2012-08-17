#include "def/rcp_record.h"
#include "def/rcp_data.h"
#include "def/rcp_type.h"

void rcp_record_change_type(rcp_record_ref rec, rcp_type_ref dst_type);
rcp_data_ref rcp_cast_l1(rcp_type_ref src_type, rcp_data_ref src_data,
		rcp_type_ref dst_type);
