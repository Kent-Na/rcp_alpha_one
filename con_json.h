rcp_extern void con_json_execute(
		rcp_connection_ref con, const char *begin, const char *end);

#define CON_JSON_CLASS_PART \
{\
	NULL,\
	NULL,\
	con_json_execute,\
}
