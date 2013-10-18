#include "../def/rcp_type.h"
#include "../def/rcp_data.h"
#include "../def/rcp_string.h"
#include "../def/rcp_record.h"
#include "../def/rcp_connection.h"

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_type_core{
	size_t size;
	uint16_t type_id;
	rcp_string_ref type_name;

	//It is allowed to re-init data just after init. but not when modefied
	//or init with extra data like "init_with_c_str".
	void (*init)(rcp_type_ref type, rcp_data_ref data);

	//Release all allocated memory on init or somewhere else.
	//Don't release data itself.
	void (*deinit)(rcp_type_ref type, rcp_data_ref data);

	//Deplecated. Will be replaced with "copied".
	//deap copy(may be... except ref type.)
	//Before call it, src must be inited and dst must be deinited state.
	void (*copy)(rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);

	int (*compare)(rcp_type_ref type, rcp_data_ref l, rcp_data_ref r);

	void (*write_json)(
			rcp_type_ref type, rcp_data_ref data, rcp_string_ref out);
	void (*send_as_command)(rcp_type_ref type, rcp_data_ref data,
			rcp_connection_ref con);
	//unused
	void (*set)(rcp_type_ref type, rcp_data_ref dst,
			rcp_type_ref key_type, rcp_data_ref key_data,
			rcp_type_ref data_type, rcp_data_ref data_data);
	//unused
	void (*append)(rcp_type_ref type, rcp_data_ref dst,
			rcp_type_ref data_type, rcp_data_ref data_data);
	void (*unset)(rcp_type_ref type, rcp_data_ref dst,
			rcp_type_ref key_type, rcp_data_ref key_data);

	//Member access. *io_data will be null when value is missing.
	void (*at)(rcp_type_ref *io_type, rcp_data_ref *io_data,
			rcp_type_ref key_type, rcp_data_ref key_data);

	void (*copied)(rcp_type_ref type, rcp_data_ref data);

	//Array_replace
	int8_t (*replace)(rcp_type_ref type, rcp_data_ref target,
			int32_t begin, int32_t end, rcp_data_ref input);
	//Dict_merge
	int8_t (*merge)(rcp_type_ref type, rcp_data_ref target,
			rcp_data_ref input);
};
#endif

rcp_extern void rcp_type_delete(rcp_type_ref type);
rcp_extern void rcp_type_set_name(rcp_type_ref type, rcp_string_ref str);
rcp_extern rcp_string_ref rcp_type_name(rcp_type_ref type);

//alloc and init
rcp_extern rcp_data_ref rcp_new(rcp_type_ref type);
//deinit and dealloc
rcp_extern void rcp_delete(rcp_type_ref type, rcp_data_ref data);

rcp_extern rcp_data_ref rcp_alloc(rcp_type_ref type);
rcp_extern void rcp_dealloc(rcp_data_ref data);

rcp_extern void rcp_init(rcp_type_ref type,
		rcp_data_ref data);

rcp_extern void rcp_deinit(rcp_type_ref type,
		rcp_data_ref data);

//Deinit dst then move src data to dst.
//After that, src is deinited and dst is inited state.
rcp_extern void rcp_move(rcp_type_ref type, 
		rcp_data_ref src, rcp_data_ref dst);

//Deinit dst then copy src data to dst using copy method.
//After that, src and dst are inited state.
rcp_extern void rcp_copy(rcp_type_ref type,
		rcp_data_ref src, rcp_data_ref dst);

//swap 2 values. don't use this function. It have a bug.
//rcp_extern void rcp_swap(rcp_type_ref type,
//		rcp_data_ref src, rcp_data_ref dst);

rcp_extern int rcp_compare(rcp_type_ref type, 
		rcp_data_ref l, rcp_data_ref r);

rcp_extern void rcp_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref out);

rcp_extern void rcp_send_as_command(rcp_type_ref type, rcp_data_ref data,
			rcp_connection_ref con);
rcp_extern void rcp_set(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref key_type, rcp_data_ref key_data,
		rcp_type_ref data_type, rcp_data_ref data_data);
rcp_extern void rcp_append(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref data_type, rcp_data_ref data_data);
rcp_extern void rcp_unset(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref key_type, rcp_data_ref key_data);
rcp_extern void rcp_at(rcp_type_ref *io_type, rcp_data_ref *io_data,
			rcp_type_ref key_type, rcp_data_ref key_data);
rcp_extern int8_t rcp_replace(rcp_type_ref type, rcp_data_ref target,
		int32_t begin, int32_t end, rcp_data_ref input);
rcp_extern int8_t rcp_merge(rcp_type_ref type, rcp_data_ref target,
		rcp_data_ref input);
