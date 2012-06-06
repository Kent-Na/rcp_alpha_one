
struct con_null_terminate{
	struct con_buffer buffer;
};

void con_null_terminate_init(rcp_connection_ref con);
void con_null_terminate_deinit(rcp_connection_ref con);

void con_null_terminate_send(
		rcp_connection_ref con, void *data, size_t len);
void con_null_terminate_on_receive(rcp_connection_ref con);
rcp_err con_null_terminate_next_command(
		rcp_connection_ref con, void **command_begin, void **command_end);
void con_null_terminate_clean_space(rcp_connection_ref con);

#define CON_NULL_TERMINAE_CLASS_PART \
{\
	con_null_terminate_init,\
	con_null_terminate_deinit,\
	con_null_terminate_send,\
	con_null_terminate_on_receive,\
	con_null_terminate_next_command,\
	con_null_terminate_clean_space\
}
