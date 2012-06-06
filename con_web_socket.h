
struct con_web_socket{
	struct con_buffer buffer;

	uint8_t ws_key[24];
	uint8_t http_state;
	uint8_t ws_state;

	uint8_t ws_header1;
	uint8_t ws_header2;

	uint64_t ws_payload_len;
	uint8_t ws_masking_key[4];
};

void con_web_socket_init(rcp_connection_ref con);
void con_web_socket_send(
		rcp_connection_ref con, void *data, size_t len);
void con_web_socket_on_receive(rcp_connection_ref con);
rcp_err con_web_socket_next_command(
		rcp_connection_ref con, void **command_begin, void **command_end);
void con_web_socket_clean_space(rcp_connection_ref con);

#define CON_WEB_SOCKET_CLASS_PART \
{\
	con_web_socket_init,\
	NULL,/*release*/\
	con_web_socket_send,\
	con_web_socket_on_receive,\
	con_web_socket_next_command,\
	con_web_socket_clean_space\
}
