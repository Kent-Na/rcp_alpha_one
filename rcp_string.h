typedef rcp_record_ref rcp_string_ref;


rcp_extern struct rcp_type_core rcp_string_type;

rcp_extern const char *rcp_string_c_str(void *str);
rcp_extern const char *rcp_string_type_c_str(rcp_string_ref str);
rcp_extern void rcp_string_put(rcp_string_ref str, char ch);
rcp_extern void rcp_string_set_c_str(rcp_string_ref str, const char* c_str);
rcp_extern rcp_string_ref rcp_string_new(const char* str);
