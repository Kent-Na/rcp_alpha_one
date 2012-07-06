#include "def/rcp_type.h"
#include "def/rcp_data.h"
#include "def/rcp_string.h"
#include "def/rcp_record.h"

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_type_core{
	size_t size;
	uint16_t type_id;
	const char* type_name;

	//It is allowed to re-init data just after init. but not when modefied
	//or init with extra data like "init_with_c_str".
	void (*init)(rcp_type_ref type, rcp_data_ref data);

	//Release all allocated memory on init or somewhere else.
	//Don't release data itself.
	void (*deinit)(rcp_type_ref type, rcp_data_ref data);

	//deap copy
	//Before call it, src must be inited and dst must be deinited state.
	void (*copy)(rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);

	int (*compare)(rcp_type_ref type, rcp_data_ref l, rcp_data_ref r);

	void (*write_json)(
			rcp_type_ref type, rcp_data_ref data, rcp_string_ref out);

	void (*at)(rcp_type_ref type, rcp_data_ref dst,
			rcp_type_ref *io_type, rcp_data_ref *io_data);

	void (*set)(rcp_type_ref type, rcp_data_ref dst, rcp_record_ref src);
	void (*append)(rcp_type_ref type, rcp_data_ref dst, rcp_record_ref src);
	void (*unset)(rcp_type_ref type, rcp_data_ref dst, rcp_record_ref src);

};
#endif

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

//swap 2 values
rcp_extern void rcp_swap(rcp_type_ref type,
		rcp_data_ref src, rcp_data_ref dst);

rcp_extern int rcp_compare(rcp_type_ref type, 
		rcp_data_ref l, rcp_data_ref r);

rcp_extern void rcp_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref out);
