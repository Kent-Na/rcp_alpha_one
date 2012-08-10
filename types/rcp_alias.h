#include "../def/rcp_type.h"
#include "../def/rcp_data.h"
#include "../def/rcp_string.h"

rcp_extern rcp_type_ref rcp_alias_type_new(rcp_type_ref real_type);
rcp_type_ref rcp_alias_real_type(rcp_type_ref type);
int rcp_alias_is(rcp_type_ref type);

void rcp_alias_init(rcp_type_ref type, rcp_data_ref data);
void rcp_alias_deinit(rcp_type_ref type, rcp_data_ref data);
void rcp_alias_copy(rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);
int rcp_alias_compare(rcp_type_ref type, rcp_data_ref l, rcp_data_ref r);
void rcp_alias_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref out);
