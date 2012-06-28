#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_command_list.h"
#include "rcp_command.h"
#include "rcp_epoll.h"
#include "rcp_io.h"

#include "rcp_types.h"
#include "rcp_sender.h"
#include "rcp_receiver.h"

#include "rcp_json.h"

rcp_extern rcp_record_ref con_json_execute(
		rcp_receiver_ref con, const char *begin, const char *end)
{
	const char *cmd_begin = (const char *)begin;
	return rcp_json_parse(&cmd_begin, end);
}


void cmp_json_init(void *userdata);
void cmp_json_deinit(void *userdata);
void cmp_json_build(void *userdata, rcp_record_ref rec);
void cmp_json_result(void *userdata, 
		const uint8_t **begin, const uint8_t **end);
void cmp_json_clean_up(void *userdata);

struct cmp_json{
	rcp_string_ref str;
};

struct rcp_sender_l0_class cmp_json_class={
	sizeof (struct cmp_json),
	cmp_json_init,
	cmp_json_deinit,
	cmp_json_build,
	cmp_json_result,
	cmp_json_clean_up
};

void cmp_json_init(void *userdata)
{
	struct cmp_json* st = userdata;
	st->str = NULL;
}
void cmp_json_deinit(void *userdata)
{
	struct cmp_json* st = userdata;
	if (st->str)
		rcp_string_delete(st->str);
}
void cmp_json_build(void *userdata, rcp_record_ref rec)
{
	struct cmp_json* st = userdata;
	if (st->str){
		rcp_caution("cmp:json:not clean uped");
		cmp_json_clean_up(userdata);
	}
	rcp_string_ref cmd_str = rcp_string_new(NULL);
	rcp_json_write_record(rec, cmd_str);
	st->str = cmd_str;
}

void cmp_json_result(void *userdata, 
		const uint8_t **begin, const uint8_t **end)
{
	struct cmp_json* st = userdata;
	if (!st->str){
		rcp_caution("cmp:json:not built");
		*begin = NULL;
		*end = NULL;
	}
	*begin = rcp_string_c_str(st->str);
	*end = *begin + rcp_string_c_str_len(st->str);
}

void cmp_json_clean_up(void *userdata)
{
	struct cmp_json* st = userdata;
	rcp_string_delete(st->str);
	st->str = NULL;
}
