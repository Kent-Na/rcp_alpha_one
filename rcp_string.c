#include "rcp_pch.h"
#include "rcp_utility.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_string.h"

struct rcp_string_core{
	char * str;
	size_t size;//allocated memory size
	size_t count;
};

typedef struct rcp_string_core *rcp_string_core_ref;

//string type
void rcp_string_type_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_string_core_ref core = data;
	core->str = NULL;
	core->size = 0;
	core->count = 0;
}
void rcp_string_type_init_with_c_str(
		rcp_type_ref type, rcp_data_ref data, const char *str)
{
	if (!str){
		rcp_string_type_init(type, data);
		return;
	}
	rcp_string_core_ref core = data;
	size_t len = strlen(str) + 1;
	core->str = malloc(len);
	memcpy(core->str, str, len);

	core->size = len;
	core->count = len;
}
void rcp_string_type_deinit(rcp_type_ref type, rcp_data_ref data)
{
	rcp_string_core_ref core = data;
	free(core->str);
}
void rcp_string_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
{
	rcp_string_core_ref src_core = src;
	rcp_string_core_ref dst_core = dst;
	rcp_string_type_init_with_c_str(type, dst, src_core->str);
}

int rcp_string_type_compare(
		rcp_type_ref type, rcp_data_ref l, rcp_data_ref r)
{
	rcp_string_core_ref l_core = l;
	rcp_string_core_ref r_core = r;
	return strcmp(l_core->str, r_core->str);
}

const struct rcp_type_core rcp_string_type_def = {
	sizeof (struct rcp_string_core),
	RCP_TYPE_STRING,
	"string",
	rcp_string_type_init,
	rcp_string_type_deinit,
	rcp_string_type_copy,
	rcp_string_type_compare,
};

const char *rcp_string_type_c_str(void *str)
{
	rcp_string_core_ref core = str;
	return core->str;
}

const char *rcp_string_c_str(rcp_string_ref str)
{
	rcp_string_core_ref core = rcp_record_data(str);
	return core->str;
}
size_t rcp_string_c_str_len(rcp_string_ref str){
	rcp_string_core_ref core = rcp_record_data(str);
	return core->count;
}

void rcp_string_put(rcp_string_ref str, char ch){
	rcp_string_core_ref core = rcp_record_data(str);
	if (core->count == core->size){
		size_t storage_size = core->size * 2;
		if (!storage_size)
			storage_size = 64;
		core->str = realloc(core->str, storage_size);
		core->size = storage_size;
	}
	core->str[core->count] = ch;
	core->count++;
}
rcp_extern void rcp_string_append_c_str(
		rcp_string_ref str, const char *c_str){
	const char *ch = c_str;
	while (*ch){
		rcp_string_put(str, *ch);
		ch ++;
	}
}

rcp_string_ref rcp_string_new(const char* c_str)
{
	rcp_string_ref str = rcp_record_new(rcp_string_type);
	rcp_string_set_c_str(str, c_str);
	return str;
}

void rcp_string_set_c_str(rcp_string_ref str, const char* c_str)
{
	rcp_string_core_ref core = rcp_record_data(str);
	rcp_string_type_deinit(rcp_string_type, core);
	rcp_string_type_init_with_c_str(rcp_string_type, core, c_str);
}

