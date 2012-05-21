
typedef struct rcp_type_core *rcp_type_ref;

struct rcp_type_core{
	//size_t size;
	void (*init)(void *data);
	void (*free)(void *data);
	void (*copy)(void *src, void *dst);
	int (*compare)(void *src, void *dst);

	size_t size;
	//int (*parameter_from_name)(void *base, rcp_string_ref *name,
	//		void **param, rcp_type_ref type);
};
