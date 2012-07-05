#include "../rcp_pch.h"
#include "../rcp_defines.h"
#include "../rcp_utility.h"
#include "../rcp_buffer.h"

#include "../rcp_io.h"
#include "../rcp_receiver.h"
#include "../rcp_sender.h"
#include "con_web_socket.h"

const uint8_t http_request_receiving = 0;
const uint8_t http_header_receiving = 1;
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

int con_web_socket_http_on_receive(rcp_receiver_ref con);
void con_web_socket_frame_on_receive(rcp_receiver_ref con);
void con_web_socket_http_request(rcp_receiver_ref con, rcp_io_ref io);
void con_web_socket_perse_http_field(
		rcp_receiver_ref con, char *line);
int con_web_socket_http_next_field(rcp_receiver_ref con, rcp_io_ref io);
void con_web_socket_send_http_header(
		rcp_receiver_ref con, rcp_io_ref io);

void con_web_socket_init(rcp_receiver_ref con){
	struct con_web_socket *st = rcp_receiver_l2(con);
	rcp_buffer_init(&st->buffer, RCP_PROTOCOL_JSON_BUFFER_SIZE);
	st->http_state = http_request_receiving;	
}
void con_web_socket_deinit(rcp_receiver_ref con){
	struct con_web_socket *st = rcp_receiver_l2(con);
	rcp_buffer_deinit(&st->buffer);
}

void con_web_socket_on_receive(rcp_receiver_ref con, rcp_io_ref io)
{
	rcp_info("on_receive");
	struct con_web_socket *st = rcp_receiver_l2(con);

	void *space = rcp_buffer_space(&st->buffer);
	size_t size = rcp_buffer_space_size(&st->buffer);

	size_t len = rcp_io_receive(io, space, size);
	rcp_buffer_supplied(&st->buffer, len);

	if (st->http_state == http_request_receiving){
		con_web_socket_http_request(con, io);
	}
	if (st->http_state == http_header_receiving){
		while (!con_web_socket_http_next_field(con, io)){}
	}
}

void con_web_socket_http_request(rcp_receiver_ref con, rcp_io_ref io)
{
	struct con_web_socket *st = rcp_receiver_l2(con);

	unsigned char *d_begin = rcp_buffer_data(&st->buffer); 
	unsigned char *d_end = rcp_buffer_data_end(&st->buffer);
	const char expected[] = "GET /rcp HTTP/1.1\r\n";
	size_t ex_len = sizeof expected - 1;

	if (ex_len > d_end - d_begin)
		return;

	const char *p = expected;
	unsigned char *d = d_begin;

	while (*p && *p++ == *d++);
	if (*p){
		rcp_error("http request");
		//rcp_io_close(io);
		return;
	}

	rcp_buffer_consumed_at(&st->buffer, d);
	st->http_state = http_header_receiving;	
}
int con_web_socket_http_next_field(rcp_receiver_ref con, rcp_io_ref io)
{
	struct con_web_socket *st = rcp_receiver_l2(con);

	unsigned char *d_begin = rcp_buffer_data(&st->buffer); 
	unsigned char *d_end = rcp_buffer_data_end(&st->buffer);

	unsigned char *d;

	for (d = d_begin; d < d_end-1; d++)
		if (d[0] == '\r' && d[1] == '\n')
			break;

	if (d < d_end-1){
		char* field = malloc(d-d_begin+1);
		memcpy(field, d_begin, d-d_begin);
		field[d-d_begin] = '\0';
		if (d_begin != d)
			con_web_socket_perse_http_field(con, field);
		free(field);
		rcp_buffer_consumed_at(&st->buffer, d+2);
		if (d_begin == d){
			st->http_state = http_upgraded;	
			con_web_socket_send_http_header(con, io);
		}
		return 0;
	}

	return -1;
}

void con_web_socket_perse_http_field(
		rcp_receiver_ref con, char *line)
{
	struct con_web_socket *st = rcp_receiver_l2(con);

	//reprace first ';' to '\0'	

	char* name = line;
	char* value = NULL;

	char* p = line;
	while (*p != ':'){
		if (*p == '\0'){
			rcp_error(line);
			rcp_error("^ http header");
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
		if (strlen(value)!=24){
			rcp_error("Sec-WebSocket-Key");
			return;
		}
		memcpy(st->ws_key, value, 24);
	}
}

void con_web_socket_send_http_header(
		rcp_receiver_ref con, rcp_io_ref io)
{
	struct con_web_socket *st = rcp_receiver_l2(con);

	char* crnl = "\r\n";
	rcp_io_send(io, ws_header_0, strlen(ws_header_0));
	rcp_io_send(io, crnl, 2);
	rcp_io_send(io, ws_header_1, strlen(ws_header_1));
	rcp_io_send(io, crnl, 2);
	rcp_io_send(io, ws_header_2, strlen(ws_header_2));
	rcp_io_send(io, crnl, 2);
	rcp_io_send(io, ws_header_3, strlen(ws_header_3));
	{
		const char *uuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
		unsigned char buffer[24+36];
		memcpy(buffer, st->ws_key, 24);
		memcpy(buffer+24, uuid, 36);
		char hash[20];
		SHA1(buffer, 24+36, (unsigned char*)hash);
		char *key = rcp_encode_base64(hash, 20);
		rcp_io_send(io, key, strlen(key));
		free(key);
	}
	rcp_io_send(io, crnl, 2);
	rcp_io_send(io, crnl, 2);
}

rcp_err con_web_socket_next_command(
		rcp_receiver_ref con, void **command_begin, void **command_end)
{
	struct con_web_socket *st = rcp_receiver_l2(con);

	if (st->http_state == http_header_receiving){
		return -1;
	}

	if (st->ws_state == ws_not_yet){
		const uint8_t *buffer = rcp_buffer_consume(&st->buffer, 2);
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
			const uint8_t *buffer = rcp_buffer_consume(&st->buffer, 2);
			if (!buffer)
				return -1;
			st->ws_payload_len = be16toh(*(uint16_t*)buffer);
			st->ws_state = ws_payload_len_received;
		}
		else{
			const uint8_t *buffer = rcp_buffer_consume(&st->buffer, 8);
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
		const uint8_t *buffer = rcp_buffer_consume(&st->buffer, 4);
		if (!buffer)
			return -1;
		memcpy(st->ws_masking_key ,buffer, 4);
		st->ws_state = ws_masking_key_received;
	}
	if (st->ws_state == ws_masking_key_received){
		uint8_t *buffer = rcp_buffer_consume(
				&st->buffer, st->ws_payload_len);

		if (!buffer)
			return -1;
		int i;
		for (i = 0; i< st->ws_payload_len; i++){
			buffer[i] ^= st->ws_masking_key[i%4];
		}

		*command_begin = buffer;
		*command_end = buffer + st->ws_payload_len; 

		st->ws_state = ws_not_yet;

		return 0;
	}

	return -1;
}
void con_web_socket_clean_space(rcp_receiver_ref con)
{
	struct con_web_socket *st = rcp_receiver_l2(con);
	rcp_buffer_cleanup(&st->buffer);
}

struct cmp_web_socket{
	struct rcp_buffer buffer;
};

void cmp_web_socket_init(void* userdata);
void cmp_web_socket_deinit(void* userdata);
void cmp_web_socket_build(void* userdata, 
		const uint8_t *begin, const uint8_t *end);
void cmp_web_socket_result(void* userdata, 
		const uint8_t **begin, const uint8_t **end);
void cmp_web_socket_twist(void* userdata);
void cmp_web_socket_clean_up(void* userdata);

struct rcp_sender_l1_class cmp_web_socket_class = {
	sizeof (struct cmp_web_socket),
	cmp_web_socket_init,
	cmp_web_socket_deinit,
	cmp_web_socket_build,
	cmp_web_socket_result,
	cmp_web_socket_twist,
	cmp_web_socket_clean_up
};

void cmp_web_socket_init(void* userdata)
{
	struct cmp_web_socket *st = userdata;
	rcp_buffer_init(&st->buffer, RCP_PROTOCOL_JSON_BUFFER_SIZE);
}
void cmp_web_socket_deinit(void* userdata)
{
	struct cmp_web_socket *st = userdata;
	rcp_buffer_deinit(&st->buffer);
}
void cmp_web_socket_build(void* userdata, 
		const uint8_t *begin, const uint8_t *end)
{
	struct cmp_web_socket *st = userdata;
	//to enable this, set CON_WEB_SOCKET_MASK.
	uint8_t mask_enable = 0;
	size_t len = end-begin;

	size_t total_size = len ;
	total_size += 2;
	if (mask_enable)
		total_size += 4;
	if (len < 126)
		total_size += 0;
	else if (len < 0x10000)
		total_size += 2;
	else
		total_size += 8;
	if (rcp_buffer_space_size(&st->buffer)<total_size){
		rcp_error("cmp nt not enough space");
		return;
	}

	uint8_t ws_header1 = CON_WEB_SOCKET_FIN | CON_WEB_SOCKET_FRAME_TEXT;
	rcp_buffer_supply(&st->buffer, &ws_header1, sizeof ws_header1);


	if (len<126) {
		uint8_t ws_header2 = mask_enable | len;
		rcp_buffer_supply(&st->buffer, &ws_header2, sizeof ws_header2);
	}
	else if (len< 0x10000){
		uint8_t ws_header2 = mask_enable | 126;
		rcp_buffer_supply(&st->buffer, &ws_header2, sizeof ws_header2);
		uint16_t ws_p = htobe16(len);
		rcp_buffer_supply(&st->buffer, &ws_p, sizeof ws_p);
	}
	else{
		uint8_t ws_header2 = mask_enable | 127;
		rcp_buffer_supply(&st->buffer, &ws_header2, sizeof ws_header2);
		uint64_t ws_p = htobe64(len);
		rcp_buffer_supply(&st->buffer, &ws_p, sizeof ws_p);
	}

	//uint8_t mask[4] = {0,0,0,0};
	//rcp_buffer_supply(, mask, 4);
	rcp_buffer_supply(&st->buffer, begin, len);
}
void cmp_web_socket_result(void* userdata, 
		const uint8_t **begin, const uint8_t **end)
{
	struct cmp_web_socket *st = userdata;
	*begin = rcp_buffer_data(&st->buffer);
	*end = rcp_buffer_data_end(&st->buffer);
}
void cmp_web_socket_twist(void* userdata)
{
	return;
}
void cmp_web_socket_clean_up(void* userdata)
{
	struct cmp_web_socket *st = userdata;
	rcp_buffer_consumed_all(&st->buffer);
	rcp_buffer_cleanup(&st->buffer);
}
