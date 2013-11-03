
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

#ifdef __APPLE__
static inline uint16_t be16toh(uint16_t val){
	uint8_t in[2];
	uint16_t* in_p = (uint16_t*)in; 
	*in_p = val;
	uint8_t out[2];
	out[0] = in[1];
	out[1] = in[0];
	uint16_t* out_p = (uint16_t*)out; 
	return *out_p;
}
static inline uint64_t be64toh(uint64_t val){
	uint8_t in[8];
	uint64_t* in_p = (uint64_t*)in; 
	*in_p = val;
	uint8_t out[8];
	out[0] = in[7];
	out[1] = in[6];
	out[2] = in[5];
	out[3] = in[4];
	out[4] = in[3];
	out[5] = in[2];
	out[6] = in[1];
	out[7] = in[0];
	uint64_t* out_p = (uint64_t*)out; 
	return *out_p;
}

#define htobe16(v) be16toh(v)
#define htobe64(v) be64toh(v)

#endif
