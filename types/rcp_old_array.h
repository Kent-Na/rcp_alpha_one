////
// rcp_old_array is an array stores type infomation in data region.
// don't use this.
//

#include "../def/rcp_type.h"
#include "../def/rcp_data.h"
#include "../def/rcp_record.h"
#include "../def/rcp_old_array.h"
#include "../def/rcp_old_array_iterater.h"

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_old_array_core{
	rcp_type_ref type;
	void *data;
	size_t size;//allocated data count
	size_t count;//used data count
};
#endif 

rcp_extern rcp_record_ref rcp_old_array_new_rec(rcp_type_ref type);

rcp_extern rcp_old_array_ref rcp_old_array_new(rcp_type_ref type);
rcp_extern void rcp_old_array_delete(rcp_old_array_ref data);

void rcp_old_array_init(rcp_type_ref type, rcp_data_ref data);
void rcp_old_array_init_with_type(void *data, rcp_type_ref type);
void rcp_old_array_deinit(rcp_type_ref type, rcp_data_ref data);

void rcp_old_array_set(rcp_type_ref type, rcp_data_ref data,
		rcp_type_ref key_type, rcp_data_ref key_data,
		rcp_type_ref data_type, rcp_data_ref data_data);

void rcp_old_array_append(rcp_type_ref type, rcp_data_ref data,
		rcp_type_ref data_type, rcp_data_ref data_data);

rcp_extern rcp_type_ref rcp_old_array_data_type(rcp_old_array_ref array);

rcp_extern size_t rcp_old_array_count(rcp_old_array_ref array);
rcp_extern size_t rcp_old_array_empty(rcp_old_array_ref array);
rcp_extern rcp_data_ref rcp_old_array_data(rcp_old_array_ref array, size_t index);
rcp_extern void rcp_old_array_append_data(rcp_old_array_ref array, void *data);

//If an array has no data, [array->data] not modefied.
rcp_extern void rcp_old_array_pop(rcp_old_array_ref array, void *data);

rcp_extern rcp_old_array_iterater_ref rcp_old_array_begin(rcp_old_array_ref array);

//return NULL if last
rcp_extern rcp_old_array_iterater_ref rcp_old_array_iterater_next(
		rcp_old_array_ref array, rcp_old_array_iterater_ref itr);
rcp_extern rcp_data_ref rcp_old_array_iterater_data(
		rcp_old_array_ref array, rcp_old_array_iterater_ref itr);
