typedef struct rcp_string_core* rcp_string_ref;

struct rcp_string_core{
	uint32_t ref_count;
};

rcp_extern const char *rcp_string_c_str(rcp_string_ref str);
rcp_extern rcp_string_ref rcp_string_new(const char* str);
rcp_extern rcp_string_ref rcp_string_retain(rcp_string_ref str);
rcp_extern void rcp_string_release(rcp_string_ref str);
rcp_extern int rcp_string_compare(rcp_string_ref l, rcp_string_ref r);

rcp_extern struct rcp_type_core rcp_type_string;
