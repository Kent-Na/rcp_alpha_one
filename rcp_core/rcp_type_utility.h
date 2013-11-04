#include "def/rcp_type.h"
#include "def/rcp_data.h"
#include "def/rcp_struct.h"
#include "def/rcp_record.h"
#include "def/rcp_dict.h"
#include "def/rcp_array.h"

rcp_extern rcp_record_ref rcp_dict_find_c_str(
		rcp_type_ref dict_type, rcp_dict_ref dict, 
		const char *key, rcp_type_ref type);
rcp_extern void rcp_dict_to_struct(
		rcp_type_ref in_type, rcp_dict_ref in,
		rcp_type_ref out_type, rcp_struct_ref out);


rcp_extern int rcp_type_is_int(rcp_type_ref type);
rcp_extern int rcp_type_is_uint(rcp_type_ref type);

rcp_extern int64_t rcp_int_as_int(rcp_type_ref type, rcp_data_ref data);
rcp_extern uint64_t rcp_uint_as_uint(rcp_type_ref type, rcp_data_ref data);

rcp_extern int rcp_record_is_null(rcp_record_ref rec);

rcp_extern void rcp_data_at(
		rcp_type_ref *io_type, rcp_data_ref *io_data, 
		rcp_array_ref path);

rcp_data_ref rcp_cast_l1(rcp_type_ref src_type, rcp_data_ref src_data,
		rcp_type_ref dst_type);
