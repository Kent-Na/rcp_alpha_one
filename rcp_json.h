

rcp_extern void rcp_json_parse(const char **begin, const char *end);


rcp_extern int rcp_json_parse_string(const char **begin, const char *end, 
		const char **result);

rcp_extern int rcp_json_parse_literal(const char **begin, const char *end, 
		const char *literal, size_t len);

struct rcp_json_number{
	int64_t significand;
	int64_t exponential_part;
};

rcp_extern int rcp_json_parse_number(const char** begin, const char* end,
		struct rcp_json_number *result);

