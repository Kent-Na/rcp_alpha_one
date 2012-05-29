
typedef struct rcp_type_core *rcp_type_ref;

struct rcp_type_core{
	size_t size;
	void (*init)(void *data);
	void (*free)(void *data);
	//deap copy
	void (*copy)(void *src, void *dst);
	int (*compare)(void *src, void *dst);

	//int (*parameter_from_name)(void *base, rcp_string_ref *name,
	//		void **param, rcp_type_ref type);
};

typedef struct rcp_record_core *rcp_record_ref;

struct rcp_record_core{
	rcp_type_ref type;
	uint32_t ref_count;
	//rcp_structure_ref structure;
	//uint32_t record_id;
};

rcp_extern rcp_record_ref rcp_record_new(rcp_type_ref type);
rcp_extern void rcp_record_retain(rcp_record_ref rec);
rcp_extern void rcp_record_release(rcp_record_ref rec);
//rcp_extern void rcp_record_delete(rcp_record_ref);
rcp_extern rcp_type_ref rcp_record_type(rcp_record_ref rec);
rcp_extern void* rcp_record_data(rcp_record_ref rec);

rcp_extern void rcp_move(rcp_type_ref type, void *src, void *dst);
rcp_extern void rcp_copy(rcp_type_ref type, void *src, void *dst);
rcp_extern void rcp_swap(rcp_type_ref type, void *src, void *dst);
