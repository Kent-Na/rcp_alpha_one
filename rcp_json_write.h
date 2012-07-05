#include "def/rcp_record.h"
#include "def/rcp_string.h"
#include "def/rcp_map.h"
#include "def/rcp_array.h"

void rcp_null_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref out);
void rcp_ref_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref out);

void rcp_map_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);
void rcp_array_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);
void rcp_string_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);

void rcp_bool8_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref out);
void rcp_bool32_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref out);

void rcp_uint64_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);
void rcp_uint32_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);
void rcp_uint16_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);
void rcp_uint8_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);

void rcp_int64_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);
void rcp_int32_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);
void rcp_int16_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);
void rcp_int8_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);

void rcp_double_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);
void rcp_float_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out);

