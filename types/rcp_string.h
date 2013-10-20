#include "../def/rcp_string.h"
#include "../def/rcp_type.h"
#include "../def/rcp_data.h"
#include "../def/rcp_record.h"

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_string_core{
	char *str;
	size_t size;//allocated memory size
	size_t count;
};
#endif

rcp_extern rcp_record_ref rcp_string_new_rec(const char* c_str);

rcp_extern rcp_string_ref rcp_string_new(const char* c_str);
rcp_extern void rcp_string_delete(rcp_string_ref str);

void rcp_string_init(rcp_type_ref type, rcp_data_ref data);
void rcp_string_deinit(rcp_type_ref type, rcp_data_ref data);
void rcp_string_copied(rcp_type_ref type, rcp_data_ref data);
void rcp_string_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);
int rcp_string_compare(
		rcp_type_ref type, rcp_data_ref l, rcp_data_ref r);

rcp_extern void rcp_string_init_with_c_str(
		rcp_string_ref data, const char *str);
rcp_extern const char *rcp_string_c_str(rcp_string_ref str);
rcp_extern size_t rcp_string_c_str_len(rcp_string_ref str);
rcp_extern void rcp_string_put(rcp_string_ref str, char ch);
rcp_extern void rcp_string_append_c_str(
		rcp_string_ref str, const char *c_str);
rcp_extern void rcp_string_set_c_str(
		rcp_string_ref str, const char *c_str);

