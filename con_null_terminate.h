
struct con_null_terminate{
	char buffer[RCP_PROTOCOL_JSON_BUFFER_SIZE];
	char *buffer_begin;
	char *buffer_end;
	char null_terminater;
};

void con_null_terminate_init(void *state);
void* con_null_terminate_generate_fotter(void *state);
size_t con_null_terminate_last_footer_size(const void *state);
size_t con_null_terminate_space_len(const void *state);
void* con_null_terminate_space_ptr(const void *state);
void con_null_terminate_data_added(void* state, size_t len);
rcp_err con_null_terminate_next_command(
		void *state, void **command_begin, void **command_end);
void con_null_terminate_clean_space(void *state);

#define CON_NULL_TERMINAE_CLASS_PART \
{\
	con_null_terminate_init,\
	NULL,/*release*/\
	NULL,/*gen_header*/\
	NULL,/*header_size*/\
	con_null_terminate_generate_fotter,\
	con_null_terminate_last_footer_size,\
	con_null_terminate_space_len,\
	con_null_terminate_space_ptr,\
	con_null_terminate_data_added,\
	con_null_terminate_next_command,\
	con_null_terminate_clean_space\
}
