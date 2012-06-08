#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_connection.h"
#include "con_buffer.h"
#include "con_web_socket.h"

const uint8_t http_header_receiving = 0;
const uint8_t http_upgraded = 2;

const uint8_t ws_not_yet = 0;
const uint8_t ws_header_received = 1;
const uint8_t ws_payload_len_received = 2;
const uint8_t ws_masking_key_received = 3;

const uint8_t CON_WEB_SOCKET_FIN = 0x80;
const uint8_t CON_WEB_SOCKET_RSV1 = 0x40;
const uint8_t CON_WEB_SOCKET_RSV2 = 0x20;
const uint8_t CON_WEB_SOCKET_RSV3 = 0x10;

const uint8_t CON_WEB_SOCKET_OPCODE = 0x0f;

const uint8_t CON_WEB_SOCKET_FRAME_CONTINUATION = 0x00;
const uint8_t CON_WEB_SOCKET_FRAME_TEXT = 0x01;
const uint8_t CON_WEB_SOCKET_FRAME_BINARY = 0x02;
const uint8_t CON_WEB_SOCKET_FRAME_CLOSE = 0x08;
const uint8_t CON_WEB_SOCKET_FRAME_PING = 0x09;
const uint8_t CON_WEB_SOCKET_FRAME_PONG = 0x0A;

const uint8_t CON_WEB_SOCKET_MASK = 0x80;
const uint8_t CON_WEB_SOCKET_PAYLOAD_LEN = 0x7f;

const char *ws_header_0 = "HTTP/1.1 101 Switching Protocols";
const char *ws_header_1 = "Upgrade: websocket";
const char *ws_header_2 = "connection: upgrade";
const char *ws_header_3 = "Sec-WebSocket-Accept: ";
//const char *ws_header_4 = "Sec-WebSocket-Protocol: rcp";

int con_web_socket_http_on_receive(rcp_connection_ref con);
void con_web_socket_frame_on_receive(rcp_connection_ref con);
void con_web_socket_perse_http_field(
		rcp_connection_ref con, char *line);
int con_web_socket_http_next_field(rcp_connection_ref con);
void con_web_socket_send_http_header(
		rcp_connection_ref con);

void con_web_socket_init(rcp_connection_ref con){
	struct con_web_socket *st = rcp_connection_l2(con);
	con_buffer_init(&st->buffer, RCP_PROTOCOL_JSON_BUFFER_SIZE);
	st->http_state = http_header_receiving;	
}
void con_web_socket_deinit(rcp_connection_ref con){
	struct con_web_socket *st = rcp_connection_l2(con);
	con_buffer_deinit(&st->buffer);
}

void con_web_socket_on_receive(rcp_connection_ref con)
{
	rcp_info("on_receive");
	struct rcp_connection_class *klass = rcp_connection_class(con);
	struct con_web_socket *st = rcp_connection_l2(con);

	void *space = con_buffer_space(&st->buffer);
	size_t size = con_buffer_space_size(&st->buffer);

	size_t len = klass->l1.receive(con, space, size);
	con_buffer_supplied(&st->buffer, len);

	if (st->http_state == http_header_receiving){
		while (!con_web_socket_http_next_field(con)){}
	}
}

int con_web_socket_http_next_field(rcp_connection_ref con)
{
	struct con_web_socket *st = rcp_connection_l2(con);

	unsigned char *d_begin = con_buffer_data(&st->buffer); 
	unsigned char *d_end = con_buffer_data_end(&st->buffer);

	unsigned char *d;

	for (d = d_begin; d < d_end-1; d++)
		if (d[0] == '\r' && d[1] == '\n')
			break;

	if (d < d_end-1){
		char* field = malloc(d-d_begin);
		memcpy(field, d_begin, d-d_begin);
		field[d-d_begin] = '\0';
		if (d_begin != d)
			con_web_socket_perse_http_field(con, field);
		free(field);
		con_buffer_consumed_at(&st->buffer, d+2);
		if (d_begin == d){
			st->http_state = http_upgraded;	
			con_web_socket_send_http_header(con);
		}
		return 0;
	}

	return -1;
}

void con_web_socket_perse_http_field(
		rcp_connection_ref con, char *line)
{
	struct con_web_socket *st = rcp_connection_l2(con);

	//reprace first ';' to '\0'	

	char* name = line;
	char* value = NULL;

	char* p = line;
	while (*p != ':'){
		if (*p == '\0'){
			rcp_error("http header");
			return;	
		}		
		p++;
	}

	*p = '\0';
	value = p+1;

	while (*value == ' ')
		value ++;

	const char *ws_field0 = "Sec-WebSocket-Key";	
	if (strcmp(ws_field0,name)==0){
		while (*value == ' ')
			value ++;
		if (strlen(value)<24){
			rcp_error("Sec-WebSocket-Key");
			return;
		}
		memcpy(st->ws_key, value, 24);
	}
}

void con_web_socket_send_http_header(
		rcp_connection_ref con)
{
	struct rcp_connection_class *klass = rcp_connection_class(con);
	struct con_web_socket *st = rcp_connection_l2(con);

	char* crnl = "\r\n";
	klass->l1.send(con, ws_header_0, strlen(ws_header_0));
	klass->l1.send(con, crnl, 2);
	klass->l1.send(con, ws_header_1, strlen(ws_header_1));
	klass->l1.send(con, crnl, 2);
	klass->l1.send(con, ws_header_2, strlen(ws_header_2));
	klass->l1.send(con, crnl, 2);
	klass->l1.send(con, ws_header_3, strlen(ws_header_3));
	{
		const char *uuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
		unsigned char buffer[24+36];
		memcpy(buffer, st->ws_key, 24);
		memcpy(buffer+24, uuid, 36);
		char hash[20];
		SHA1(buffer, 24+36, (unsigned char*)hash);
		char *key = rcp_encode_base64(hash, 20);
		klass->l1.send(con, key, strlen(key));
		free(key);
	}
	klass->l1.send(con, crnl, 2);
	klass->l1.send(con, crnl, 2);
}

void con_web_socket_send(
		rcp_connection_ref con, void *data, size_t len)
{
	struct rcp_connection_class *klass = rcp_connection_class(con);

	uint8_t ws_header1 = CON_WEB_SOCKET_FIN | CON_WEB_SOCKET_FRAME_TEXT;
	klass->l1.send(con, &ws_header1, sizeof ws_header1);
	//printf("%x\n",ws_header1);

	uint8_t mask_enable = CON_WEB_SOCKET_MASK;

	if (len<126) {
		uint8_t ws_header2 = mask_enable | len;
		klass->l1.send(con, &ws_header2, sizeof ws_header2);
	}
	else if (len< (1<<16)){
		uint8_t ws_header2 = mask_enable | 126;
		klass->l1.send(con, &ws_header2, sizeof ws_header2);
		uint16_t ws_p = htobe16(len);
		klass->l1.send(con, &ws_p, sizeof ws_p);
	}
	else{
		uint8_t ws_header2 = mask_enable | 127;
		klass->l1.send(con, &ws_header2, sizeof ws_header2);
		uint64_t ws_p = htobe64(len);
		klass->l1.send(con, &ws_p, sizeof ws_p);
	}

	uint8_t mask[4] = {0,0,0,0};
	klass->l1.send(con, mask, 4);
	klass->l1.send(con, data, len);
}

rcp_err con_web_socket_next_command(
		rcp_connection_ref con, void **command_begin, void **command_end)
{
	struct con_web_socket *st = rcp_connection_l2(con);

	if (st->http_state == http_header_receiving){
		return -1;
	}

	if (st->ws_state == ws_not_yet){
		const uint8_t *buffer = con_buffer_consume(&st->buffer, 2);
		if (!buffer)
			return -1;
		st->ws_header1 = buffer[0];
		st->ws_header2 = buffer[1];
		st->ws_payload_len = 
			(st->ws_header2 & CON_WEB_SOCKET_PAYLOAD_LEN);
		st->ws_state = ws_header_received;
	}
	if (st->ws_state == ws_header_received){
		if ((st->ws_payload_len & 0x7e) != 0x7e){
			st->ws_state = ws_payload_len_received;
		}
		else if (st->ws_payload_len == 0x7e){
			const uint8_t *buffer = con_buffer_consume(&st->buffer, 2);
			if (!buffer)
				return -1;
			st->ws_payload_len = be16toh(*(uint16_t*)buffer);
			st->ws_state = ws_payload_len_received;
		}
		else{
			const uint8_t *buffer = con_buffer_consume(&st->buffer, 8);
			if (!buffer)
				return -1;
			st->ws_payload_len = be64toh(*(uint64_t*)buffer);
			st->ws_state = ws_payload_len_received;
		}
	}
	if (st->ws_state == ws_payload_len_received){
		if ( ! (st->ws_header2 & CON_WEB_SOCKET_MASK)){
			rcp_error("web socket spec err");
			return -1;
		}
		const uint8_t *buffer = con_buffer_consume(&st->buffer, 4);
		if (!buffer)
			return -1;
		memcpy(st->ws_masking_key ,buffer, 4);
		st->ws_state = ws_masking_key_received;
	}
	if (st->ws_state == ws_masking_key_received){
		uint8_t *buffer = con_buffer_consume(
				&st->buffer, st->ws_payload_len);

		if (!buffer)
			return -1;
		int i;
		for (i = 0; i< st->ws_payload_len; i++){
			buffer[i] ^= st->ws_masking_key[i%4];
		}

		/*
		uint8_t cpy[st->ws_payload_len + 1];
		memcpy(cpy, buffer, st->ws_payload_len);
		cpy[st->ws_payload_len] = '\0';
		rcp_info(cpy);
*/
		*command_begin = buffer;
		*command_end = buffer + st->ws_payload_len; 

		st->ws_state = ws_not_yet;

		return 0;
	}

	return -1;
}
void con_web_socket_clean_space(rcp_connection_ref con)
{
	struct con_web_socket *st = rcp_connection_l2(con);
	con_buffer_cleanup(&st->buffer);
}
