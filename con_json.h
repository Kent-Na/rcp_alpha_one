rcp_extern rcp_record_ref con_json_execute(
		rcp_receiver_ref con, const char *begin, const char *end);

#define CON_JSON_CLASS_PART \
{\
	NULL,\
	NULL,\
	con_json_execute,\
}

extern struct rcp_sender_l0_class cmp_json_class;
