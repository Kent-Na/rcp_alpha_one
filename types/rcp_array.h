#include "../def/rcp_array.h"
#include "../def/rcp_type.h"
#include "../def/rcp_data.h"
#include "../def/rcp_record.h"

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_type_array_ext{
	rcp_type_ref data_type;
};

#endif

//array type
rcp_type_ref rcp_array_type_data_type(rcp_type_ref type);

//array
rcp_extern rcp_array_ref rcp_array_new(rcp_type_ref array_type);
rcp_extern void rcp_array_delete(
		rcp_type_ref rcp_array_type, rcp_array_ref array);

void rcp_array_init(rcp_type_ref type, rcp_data_ref data);
void rcp_array_deinit(rcp_type_ref type, rcp_data_ref data);
void rcp_array_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);

rcp_extern int rcp_array_owning_data(rcp_array_ref array);

rcp_extern void* rcp_array_raw_data(rcp_array_ref array);
rcp_extern size_t rcp_array_len(rcp_array_ref array);

rcp_extern void rcp_array_append_data(
		rcp_type_ref array_type, rcp_array_ref array,
		rcp_data_ref data);

//array iterater
rcp_array_iterater_ref rcp_array_begin(
		rcp_type_ref array_type, rcp_array_ref array);
rcp_array_iterater_ref rcp_array_iterater_next(
		rcp_type_ref array_type, rcp_array_ref array, 
		rcp_array_iterater_ref itr);
rcp_data_ref rcp_array_iterater_data(rcp_array_iterater_ref itr);
