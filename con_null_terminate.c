#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "con_null_terminate.h"

//struct rcp_connection_layer2_class_part
//con_null_terminate_class_part = CON_NULL_TERMINAE_CLASS_PART;

void con_null_terminate_init(void *state)
{
	struct con_null_terminate *st = state;
	st->buffer_begin = st->buffer;
	st->buffer_end = st->buffer;
	st->null_terminater = 0;
}

void* con_null_terminate_generate_fotter(void *state)
{
	struct con_null_terminate *st = state;
	return &(st->null_terminater);
}

size_t con_null_terminate_last_footer_size(const void *state)
{
	return 1;
}

size_t con_null_terminate_space_len(const void *state)
{
	const struct con_null_terminate *st = state;
	size_t buffer_size = RCP_PROTOCOL_JSON_BUFFER_SIZE;
	return st->buffer + buffer_size - st->buffer_end;
}

void* con_null_terminate_space_ptr(const void *state)
{
	const struct con_null_terminate *st = state;

#ifdef RCP_SELF_TEST
	size_t buffer_size = RCP_PROTOCOL_JSON_BUFFER_SIZE;
	const char *end = st->buffer + buffer_size;
	int f0 = st->buffer_end >= st->buffer;
	int f1 = st->buffer_end <= end;
	int f2 = st->buffer_begin >= st->buffer;
	int f3 = st->buffer_begin <= end;
	if (! (f0 && f1 && f2 && f3)){
		rcp_error("buffer range");
	}
#endif
	return st->buffer_end;
}

void con_null_terminate_data_added(void* state, size_t len)
{
	struct con_null_terminate *st = state;
	st->buffer_end += len;

#ifdef RCP_SELF_TEST
	size_t buffer_size = RCP_PROTOCOL_JSON_BUFFER_SIZE;
	if (st->buffer_end - st->buffer > buffer_size){
		rcp_error("buffer over run!!!\n");
	}
#endif
}

rcp_err con_null_terminate_next_command(
		void *state, void **command_begin, void **command_end)
{
	struct con_null_terminate *st = state;
	
	char *buffer_begin = st->buffer_begin;
	char *buffer_end = st->buffer_end;

	char *p;
	for (p = buffer_begin; p < buffer_end; p++)
		if (*p == '\0')
			break;

	//p = command_end
	if (p != buffer_end){
		*command_begin = buffer_begin;
		*command_end = p; 
		st->buffer_begin = p+1;
		rcp_info(*command_begin);
		return 0;
	}
	return -1;
}

void con_null_terminate_clean_space(void *state)
{
	struct con_null_terminate *st = state;

	if (st->buffer_begin == st->buffer_end){
		//no remain data
		st->buffer_begin = st->buffer;
		st->buffer_end = st->buffer;
		return;
	}

	if (st->buffer_begin != st->buffer){
		size_t remain = st->buffer_end - st->buffer_begin;
		memmove(st->buffer, st->buffer_begin, remain);
		st->buffer_begin = st->buffer;
		st->buffer_end = st->buffer + remain;
	}
}
