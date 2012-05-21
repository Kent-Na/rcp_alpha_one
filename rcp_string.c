#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_type.h"
#include "rcp_string.h"


const char *rcp_string_c_str(rcp_string_ref str)
{
	return (void*)(str + 1);
}
rcp_string_ref rcp_string_new(const char* str)
{
	size_t len = strlen(str) + 1;
	struct rcp_string_core *core = malloc(len + sizeof *core);
	core->ref_count = 1;
	return core;
}
rcp_string_ref rcp_string_retain(rcp_string_ref str){
	str->ref_count++;
}
void rcp_string_release(rcp_string_ref str)
{
	str->ref_count --;
	if (str->ref_count == 0)
		free(str);
}

rcp_string_ref rcp_string_copy(rcp_string_ref src)
{
	return rcp_string_new(rcp_string_c_str(src));
}

int rcp_string_compare(rcp_string_ref l, rcp_string_ref r)
{
	return strcmp(rcp_string_c_str(l),rcp_string_c_str(r));
}

//string type
void rcp_type_string_init(void *val)
{
	rcp_string_ref *str_ref = val;
	*str_ref = NULL;
}
void rcp_type_string_free(void *val)
{
	rcp_string_ref *str_ref = val;
	rcp_string_release(*str_ref);
}
void rcp_type_string_copy(void *src, void *dst)
{
	rcp_string_ref *src_ref = src;
	rcp_string_ref *dst_ref = dst;
	*dst_ref = rcp_string_copy(*src_ref);
}
int rcp_type_string_compare(void *l, void *r)
{
	rcp_string_ref *l_ref = l;
	rcp_string_ref *r_ref = r;
	return rcp_string_compare(*l_ref, *r_ref);
}

struct rcp_type_core rcp_type_string = {
	rcp_type_string_init,
	rcp_type_string_free,
	rcp_type_string_copy,
	rcp_type_string_compare,
	sizeof (rcp_string_ref)
};
