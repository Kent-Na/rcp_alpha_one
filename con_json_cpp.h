
rcp_extern void con_json_cpp_execute(
		rcp_connection_ref con, const char *begin, const char *end);

#define CON_JSON_CPP_CLASS_PART \
{\
	NULL,\
	NULL,\
	con_json_cpp_execute,\
}
