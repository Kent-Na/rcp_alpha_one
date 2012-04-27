
#ifdef __cplusplus
#define rcp_extern extern "C"
#else
#define rcp_extern
#endif

rcp_extern void rcp_info(const char* str);
rcp_extern void rcp_caution(const char* str);
rcp_extern void rcp_error(const char* str);

typedef int rcp_err;
