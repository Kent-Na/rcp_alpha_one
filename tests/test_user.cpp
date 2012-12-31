#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "../rcp_json.h"
#include "../types/rcp_array.h"
#include "../types/rcp_array_list.h"
#include "../types/rcp_string.h"
#include "../types/rcp_type_list.h"
#include "../rcp_record.h"
#include "../rcp_user.h"


int test_user(){
	{
		rcp_record_ref rec = rcp_record_new(rcp_ref_array);
		rcp_array_ref ary = (rcp_array_ref)rcp_record_data(rec);
		rcp_record_ref tmp;
		tmp = rcp_string_new_rec("login");
		rcp_array_append_data(
				rcp_ref_array,
				ary,
				(rcp_data_ref)&tmp);
		rcp_permission_t pms = rcp_permission_from_array(rec);
		if (pms != RCP_PMS_LOGIN)
			rcp_error("bad permission resolv");
	}
	rcp_info("user done");
	return 0;
}
