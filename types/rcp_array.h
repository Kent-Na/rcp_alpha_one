#include "../def/rcp_array.h"
#include "../def/rcp_array_iterater.h"
#include "../def/rcp_type.h"
#include "../def/rcp_data.h"
#include "../def/rcp_record.h"

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_type_array_ext{
	rcp_type_ref data_type;
};

struct rcp_array_type{
	struct rcp_type_core core;
	struct rcp_type_array_ext ext;
};

struct rcp_array_core{
	void* array;
	size_t data_count;

	//Allocated memory size.
	//If capacity = 0 and array = not NULL, array was externaly allocated.
	//Don't modefy and release array in this case.
	size_t capacity;
};

#endif

//array type
rcp_extern rcp_type_ref rcp_array_type_data_type(rcp_type_ref type);

//array
rcp_extern rcp_array_ref rcp_array_new(rcp_type_ref array_type);
rcp_extern void rcp_array_delete(
		rcp_type_ref rcp_array_type, rcp_array_ref array);

rcp_extern void rcp_array_init(
		rcp_type_ref type, rcp_data_ref data);
rcp_extern void rcp_array_deinit(
		rcp_type_ref type, rcp_data_ref data);
rcp_extern void rcp_array_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);
rcp_extern void rcp_array_at(
		rcp_type_ref *io_type, rcp_data_ref *io_data,
		rcp_type_ref key_type, rcp_data_ref key_data);

rcp_extern int rcp_array_owning_data(rcp_array_ref array);

rcp_extern void* rcp_array_raw_data(rcp_array_ref array);
rcp_extern size_t rcp_array_len(rcp_array_ref array);

rcp_extern rcp_data_ref rcp_array_data_at(
		rcp_type_ref array_type, rcp_array_ref array,
		uint64_t idx);

rcp_extern void rcp_array_append_data(
		rcp_type_ref array_type, rcp_array_ref array,
		rcp_data_ref data);

rcp_extern void rcp_array_clear_data(
		rcp_type_ref array_type, rcp_array_ref array);

//array iterater
rcp_extern rcp_array_iterater_ref rcp_array_begin(rcp_array_ref array);
rcp_extern rcp_array_iterater_ref rcp_array_iterater_next(
		rcp_type_ref array_type, rcp_array_ref array, 
		rcp_array_iterater_ref itr);
rcp_extern rcp_data_ref rcp_array_iterater_data(rcp_array_iterater_ref itr);
