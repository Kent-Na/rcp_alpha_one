
#ifdef __cplusplus
#define rcp_extern extern "C"
#else
#define rcp_extern extern
#endif

#define field_sizeof(t, f) (sizeof(((t*)0)->f))

rcp_extern void rcp_info(const char* str);
rcp_extern void rcp_caution(const char* str);
rcp_extern void rcp_error(const char* str);
#define rcp_assert(cond, str) {if(!(cond)){rcp_error(str);return;}}

rcp_extern char *rcp_encode_base64(const char *in, size_t len);

typedef int rcp_err;
