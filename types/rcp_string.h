typedef rcp_data_ref rcp_string_ref;

rcp_extern rcp_data_ref rcp_string_new(const char* c_str);

rcp_extern void rcp_string_delete(rcp_string_ref str);
rcp_extern void rcp_string_init_with_c_str(
		rcp_data_ref data, const char *str);
rcp_extern const char *rcp_string_c_str(rcp_string_ref str);
rcp_extern size_t rcp_string_c_str_len(rcp_string_ref str);
rcp_extern void rcp_string_put(rcp_string_ref str, char ch);
rcp_extern void rcp_string_append_c_str(
		rcp_string_ref str, const char *c_str);
rcp_extern void rcp_string_set_c_str(
		rcp_string_ref str, const char *c_str);
rcp_extern rcp_record_ref rcp_string_new_rec(const char* c_str);

