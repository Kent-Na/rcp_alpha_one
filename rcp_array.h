

typedef rcp_data_ref rcp_array_ref;

typedef struct rcp_array_core *rcp_array_core_ref;

typedef void *rcp_array_iterater_ref;

rcp_extern rcp_record_ref rcp_array_new_rec(rcp_type_ref type);
rcp_extern rcp_data_ref rcp_array_new(rcp_type_ref type);
rcp_extern void rcp_array_delete(rcp_array_ref data);

rcp_extern size_t rcp_array_count(rcp_array_ref array);
rcp_extern rcp_data_ref rcp_array_data(rcp_array_ref array, size_t index);
rcp_extern rcp_type_ref rcp_array_data_type(rcp_array_ref array);
rcp_extern void rcp_array_append(rcp_array_ref array, void *data);
rcp_extern rcp_array_iterater_ref rcp_array_begin(rcp_array_ref array);

//return NULL if last
rcp_extern rcp_array_iterater_ref rcp_array_iterater_next(
		rcp_array_ref array, rcp_array_iterater_ref itr);
rcp_extern rcp_data_ref rcp_array_iterater_data(
		rcp_array_ref array, rcp_array_iterater_ref itr);
