#include "rcp_pch.h"
#include "rcp_utility.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_string.h"

struct rcp_string_core{
	char *str;
	size_t size;//allocated memory size
	size_t count;
};

typedef struct rcp_string_core *rcp_string_core_ref;

rcp_data_ref rcp_string_new(const char *c_str)
{
	rcp_data_ref str = rcp_alloc(rcp_string_type);
	rcp_string_init_with_c_str(str, c_str);
	return str;
}
rcp_extern void rcp_string_delete(rcp_string_ref str)
{
	rcp_deinit(rcp_string_type, str);
	rcp_dealloc(str);
}
//string type
void rcp_string_init(rcp_type_ref type, rcp_data_ref data)
{
	rcp_string_core_ref core = data;
	core->str = NULL;
	core->size = 0;
	core->count = 0;
}
void rcp_string_deinit(rcp_type_ref type, rcp_data_ref data)
{
	rcp_string_core_ref core = data;
	free(core->str);
}
void rcp_string_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst)
{
	rcp_string_core_ref src_core = src;
	rcp_string_deinit(rcp_string_type, dst);
	rcp_string_init_with_c_str(dst, src_core->str);
}

int rcp_string_compare(
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
	rcp_string_init,
	rcp_string_deinit,
	rcp_string_copy,
	rcp_string_compare,
};

void rcp_string_init_with_c_str(
		rcp_data_ref data, const char *c_str)
{
	if (!c_str){
		rcp_string_init(rcp_string_type, data);
		return;
	}
	rcp_string_core_ref core = data;
	size_t len = strlen(c_str) + 1;
	core->str = malloc(len);
	memcpy(core->str, c_str, len);

	core->size = len;
	core->count = len;
}

const char *rcp_string_type_c_str(void *str)
{
	rcp_string_core_ref core = str;
	return core->str;
}

const char *rcp_string_c_str(rcp_string_ref str)
{
	rcp_string_core_ref core = str;
	return core->str;
}
size_t rcp_string_c_str_len(rcp_string_ref str){
	rcp_string_core_ref core = str;
	return core->count;
}

void rcp_string_put(rcp_string_ref str, char ch){
	rcp_string_core_ref core = str;
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

rcp_record_ref rcp_string_new_rec(const char* c_str)
{
	rcp_record_ref rec = rcp_record_new(rcp_string_type);
	rcp_string_core_ref core = rcp_record_data(rec);
	rcp_string_init_with_c_str(core, c_str);
	return rec;
}

void rcp_string_set_c_str(rcp_string_ref str, const char* c_str)
{
	rcp_string_core_ref core = str;
	rcp_string_deinit(rcp_string_type, core);
	rcp_string_init_with_c_str(core, c_str);
}

