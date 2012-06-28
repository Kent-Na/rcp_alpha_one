
typedef uint16_t rcp_type_id_t;

#ifdef RCP_INTERNAL_STRUCTURE
typedef const struct rcp_type_core *rcp_type_ref;
typedef void *rcp_data_ref;
typedef struct rcp_record_core *rcp_record_ref;

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

	void (*at)(rcp_type_ref type, rcp_data_ref dst,
			rcp_type_ref *io_type, rcp_data_ref *io_data);

	void (*set)(rcp_type_ref type, rcp_data_ref dst, rcp_record_ref src);
	void (*append)(rcp_type_ref type, rcp_data_ref dst, rcp_record_ref src);
	void (*unset)(rcp_type_ref type, rcp_data_ref dst, rcp_record_ref src);
	void (*write_json)(
			rcp_type_ref type, rcp_data_ref data, rcp_data_ref out);
};

struct rcp_record_core{
	rcp_type_ref type;
	uint32_t ref_count;
};
#else
typedef struct rcp_type_opaqe	*rcp_type_ref;
typedef struct rcp_record_opaqe	*rcp_record_ref;
typedef struct rcp_data_opaqe *rcp_data_ref;
#endif

//Allocate memory block and init it.
rcp_extern rcp_record_ref rcp_record_new(rcp_type_ref type);
rcp_extern rcp_record_ref rcp_record_retain(rcp_record_ref rec);
rcp_extern void rcp_record_release(rcp_record_ref rec);
rcp_extern void rcp_record_init(rcp_record_ref type);
rcp_extern void rcp_record_deinit(rcp_record_ref type);
rcp_extern rcp_type_ref rcp_record_type(rcp_record_ref rec);
rcp_extern rcp_data_ref rcp_record_data(rcp_record_ref rec);

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

rcp_extern int rcp_compair(rcp_type_ref type, 
		rcp_data_ref l, rcp_data_ref r);
//rcp_extern rcp_data_ref rcp_data(rcp_record_ref rec);

//don't rcp_release_record returned value...
