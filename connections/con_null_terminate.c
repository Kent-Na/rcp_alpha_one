#include "../rcp_pch.h"
#include "../rcp_defines.h"
#include "../rcp_buffer.h"

#include "../rcp_io.h"
#include "../rcp_receiver.h"
#include "../rcp_sender.h"
#include "con_null_terminate.h"

//struct rcp_receiver_layer2_class_part
//con_null_terminate_class_part = CON_NULL_TERMINAE_CLASS_PART;

void con_null_terminate_init(rcp_receiver_ref con)
{
	struct con_null_terminate *st = rcp_receiver_l2(con);
	rcp_buffer_init(&st->buffer, RCP_PROTOCOL_JSON_BUFFER_SIZE);
}

void con_null_terminate_deinit(rcp_receiver_ref con)
{
	struct con_null_terminate *st = rcp_receiver_l2(con);
	rcp_buffer_deinit(&st->buffer);
}

void con_null_terminate_on_receive(rcp_receiver_ref con, rcp_io_ref io)
{
	struct con_null_terminate *st = rcp_receiver_l2(con);

	void *space = rcp_buffer_space(&st->buffer);
	size_t size = rcp_buffer_space_size(&st->buffer);

	size_t len = rcp_io_receive(io, space, size);
	rcp_buffer_supplied(&st->buffer, len);
}

rcp_err con_null_terminate_next_command(
		rcp_receiver_ref con, void **command_begin, void **command_end)
{
	struct con_null_terminate *st = rcp_receiver_l2(con);
	
	char *buffer_begin = rcp_buffer_data(&st->buffer); 
	char *buffer_end = rcp_buffer_data_end(&st->buffer);

	char *p;
	for (p = buffer_begin; p < buffer_end; p++)
		if (*p == '\0')
			break;

	//p = command_end
	if (p != buffer_end){
		*command_begin = buffer_begin;
		*command_end = p; 
		rcp_buffer_consumed_at(&st->buffer, p+1);
		//rcp_info(*command_begin);
		return 0;
	}
	return -1;
}

void con_null_terminate_clean_space(rcp_receiver_ref con)
{
	struct con_null_terminate *st = rcp_receiver_l2(con);
	rcp_buffer_cleanup(&st->buffer);
}

struct cmp_null_terminate{
	struct rcp_buffer buffer;
};

void cmp_null_terminate_init(void* userdata);
void cmp_null_terminate_deinit(void* userdata);
void cmp_null_terminate_build(void* userdata, 
		const uint8_t *begin, const uint8_t *end);
void cmp_null_terminate_result(void* userdata, 
		const uint8_t **begin, const uint8_t **end);
void cmp_null_terminate_twist(void* userdata);
void cmp_null_terminate_clean_up(void* userdata);

struct rcp_sender_l1_class cmp_null_terminate_class = {
	sizeof (struct cmp_null_terminate),
	cmp_null_terminate_init,
	cmp_null_terminate_deinit,
	cmp_null_terminate_build,
	cmp_null_terminate_result,
	cmp_null_terminate_twist,
	cmp_null_terminate_clean_up
};

void cmp_null_terminate_init(void* userdata)
{
	struct cmp_null_terminate *st = userdata;
	rcp_buffer_init(&st->buffer, RCP_PROTOCOL_JSON_BUFFER_SIZE);
}
void cmp_null_terminate_deinit(void* userdata)
{
	struct cmp_null_terminate *st = userdata;
	rcp_buffer_deinit(&st->buffer);
}
void cmp_null_terminate_build(void* userdata, 
		const uint8_t *begin, const uint8_t *end)
{
	struct cmp_null_terminate *st = userdata;
	if (rcp_buffer_space_size(&st->buffer)<end-begin+1){
		rcp_error("cmp nt not enough space");
		return;
	}
	rcp_buffer_supply(&st->buffer, begin, end-begin);
	*(uint8_t*)rcp_buffer_space(&st->buffer) = '\0';
	rcp_buffer_supplied(&st->buffer, 1);
}
void cmp_null_terminate_result(void* userdata, 
		const uint8_t **begin, const uint8_t **end)
{
	struct cmp_null_terminate *st = userdata;
	*begin = rcp_buffer_data(&st->buffer);
	*end = rcp_buffer_data_end(&st->buffer);
}
void cmp_null_terminate_twist(void* userdata)
{
	return;
}
void cmp_null_terminate_clean_up(void* userdata)
{
	struct cmp_null_terminate *st = userdata;
	rcp_buffer_consumed_all(&st->buffer);
	rcp_buffer_cleanup(&st->buffer);
}
