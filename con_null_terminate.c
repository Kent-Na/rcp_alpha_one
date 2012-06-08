#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_connection.h"
#include "con_buffer.h"
#include "con_null_terminate.h"

//struct rcp_connection_layer2_class_part
//con_null_terminate_class_part = CON_NULL_TERMINAE_CLASS_PART;

void con_null_terminate_init(rcp_connection_ref con)
{
	struct con_null_terminate *st = rcp_connection_l2(con);
	con_buffer_init(&st->buffer, RCP_PROTOCOL_JSON_BUFFER_SIZE);
}

void con_null_terminate_deinit(rcp_connection_ref con)
{
	struct con_null_terminate *st = rcp_connection_l2(con);
	con_buffer_deinit(&st->buffer);
}

void con_null_terminate_send(
		rcp_connection_ref con, void *data, size_t len)
{
	struct rcp_connection_class *klass = rcp_connection_class(con);

	unsigned char null_t = '\0';

	klass->l1.send(con, data, len);
	klass->l1.send(con, &null_t, sizeof null_t);
}

void con_null_terminate_on_receive(rcp_connection_ref con)
{
	struct rcp_connection_class *klass = rcp_connection_class(con);
	struct con_null_terminate *st = rcp_connection_l2(con);

	void *space = con_buffer_space(&st->buffer);
	size_t size = con_buffer_space_size(&st->buffer);

	size_t len = klass->l1.receive(con, space, size);
	con_buffer_supplied(&st->buffer, len);
}

rcp_err con_null_terminate_next_command(
		rcp_connection_ref con, void **command_begin, void **command_end)
{
	struct con_null_terminate *st = rcp_connection_l2(con);
	
	char *buffer_begin = con_buffer_data(&st->buffer); 
	char *buffer_end = con_buffer_data_end(&st->buffer);

	char *p;
	for (p = buffer_begin; p < buffer_end; p++)
		if (*p == '\0')
			break;

	//p = command_end
	if (p != buffer_end){
		*command_begin = buffer_begin;
		*command_end = p; 
		con_buffer_consumed_at(&st->buffer, p+1);
		rcp_info(*command_begin);
		return 0;
	}
	return -1;
}

void con_null_terminate_clean_space(rcp_connection_ref con)
{
	struct con_null_terminate *st = rcp_connection_l2(con);
	con_buffer_cleanup(&st->buffer);
}
