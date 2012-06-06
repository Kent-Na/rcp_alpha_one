
typedef uint16_t rcp_type_id_t;

#ifdef RCP_INTERNAL_STRUCTURE
typedef const struct rcp_type_core *rcp_type_ref;
typedef void *rcp_data_ref;
typedef struct rcp_record_core *rcp_record_ref;

struct rcp_type_core{
	size_t size;
	uint16_t type_id;
	const char* type_name;

	void (*init)(rcp_type_ref type, rcp_data_ref data);
	//Release all allocated memory on init or somewhere else.
	//Don't release data itself.
	void (*deinit)(rcp_type_ref type, rcp_data_ref data);
	//deap copy
	//Before call it, src must be inited state.
	//And dst must be deinited state.
	void (*copy)(rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);
	int (*compare)(rcp_type_ref type, rcp_data_ref l, rcp_data_ref r);

	//int (*parameter_from_name)(void *base, rcp_string_ref *name,
	//		void **param, rcp_type_ref type);
};

struct rcp_record_core{
	rcp_type_ref type;
	uint32_t ref_count;
};
#else
typedef struct rcp_type_opaqe	*rcp_type_ref;
typedef struct rcp_record_opaqe	*rcp_record_ref;
typedef struct rcp_data_opaqe	*rcp_data_ref;
#endif

//Allocate memory block and init it.
rcp_extern rcp_record_ref rcp_record_new(rcp_type_ref type);
rcp_extern void rcp_record_retain(rcp_record_ref rec);
rcp_extern void rcp_record_release(rcp_record_ref rec);
rcp_extern rcp_type_ref rcp_record_type(rcp_record_ref rec);
rcp_extern rcp_data_ref rcp_record_data(rcp_record_ref rec);

//Deinit dst then move src data to dst.
//After that, src is deinited and dst is inited state.
rcp_extern void rcp_move(rcp_type_ref type, 
		rcp_data_ref src, rcp_data_ref dst);

//Deinit dst then copy src data to dst using copy method.
//After that, src and dst are inited state.
rcp_extern void rcp_copy(rcp_type_ref type,
		rcp_data_ref src, rcp_data_ref dst);

//swap 2 value
rcp_extern void rcp_swap(rcp_type_ref type,
		rcp_data_ref src, rcp_data_ref dst);

//rcp_extern rcp_data_ref rcp_data(rcp_record_ref rec);

//don't rcp_release_record returned value...
rcp_extern rcp_record_ref rcp_build_tmp(
		rcp_type_ref type, rcp_data_ref src);
rcp_extern void rcp_dealloc(rcp_record_ref tmp_rec);
