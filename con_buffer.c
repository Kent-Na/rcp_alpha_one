#include "rcp_pch.h"
#include "rcp_defines.h"
#include "con_buffer.h"

void con_buffer_init(struct con_buffer *buffer, size_t len)
{
	buffer->front = malloc(len);
	buffer->back = buffer->front + len;
	buffer->begin = buffer->front;
	buffer->end = buffer->front;
}

void con_buffer_deinit(struct con_buffer *buffer)
{
	free(buffer->front);
}

void con_buffer_verify(struct con_buffer *buffer)
{
#ifdef RCP_SELF_TEST
	if (buffer->end > buffer->back){
		rcp_error("buffer over run!!!\n");
	}
	if (buffer->begin > buffer->end){
		rcp_error("undefined data used!!!\n");
	}
#endif
}

void *con_buffer_data(struct con_buffer *buffer)
{
	return buffer->begin;
}

void *con_buffer_data_end(struct con_buffer *buffer)
{
	return buffer->end;
}

size_t con_buffer_data_size(struct con_buffer *buffer)
{
	return buffer->end - buffer->begin;
}

void *con_buffer_space(struct con_buffer *buffer)
{
	return buffer->end;
}

size_t con_buffer_space_size(struct con_buffer *buffer)
{
	return  buffer->back - buffer->end;
}

void *con_buffer_supplied(struct con_buffer *buffer, size_t len)
{
	buffer->end += len;	
	con_buffer_verify(buffer);
}

void *con_buffer_consume(struct con_buffer *buffer, size_t len)
{
	void *n = buffer->begin+len;
	void *o = buffer->begin;
	if (n > buffer->end)
		return NULL;
	buffer->begin = n;
	con_buffer_verify(buffer);
	return o;
}
void *con_buffer_consumed(struct con_buffer *buffer, size_t len)
{
	buffer->begin += len;
	con_buffer_verify(buffer);
}

void *con_buffer_consumed_at(struct con_buffer *buffer, void* ptr)
{
	buffer->begin = ptr;
	con_buffer_verify(buffer);
}

void con_buffer_cleanup(struct con_buffer *buffer)
{
	if (buffer->begin == buffer->end){
		//no remain data
		buffer->begin = buffer->front;
		buffer->end = buffer->front;
		return;
	}

	if (buffer->begin != buffer->front){
		size_t remain = buffer->end - buffer->begin;
		memmove(buffer->front, buffer->begin, remain);
		buffer->begin = buffer->front;
		buffer->end = buffer->front + remain;
	}
}

