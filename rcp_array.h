

typedef rcp_record_ref rcp_array_ref;

typedef struct rcp_array_core *rcp_array_core_ref;

rcp_extern struct rcp_type_core rcp_type_array;

rcp_extern rcp_array_ref rcp_array_new(rcp_type_ref type);

rcp_extern size_t rcp_array_count(rcp_array_ref array);
rcp_extern void* rcp_array_data(rcp_array_ref array, size_t index);
rcp_extern void rcp_array_append(rcp_array_ref array, void *data);
