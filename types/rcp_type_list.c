//This file was generated by program.
#include "../rcp_pch.h"
#include "../rcp_utility.h"
#define RCP_INTERNAL_STRUCTURE
#include "../rcp_tree.h"
#include "../rcp_type.h"
#include "../rcp_json_write.h"
#include "../rcp_send_as_command.h"
#include "rcp_number.h"
#include "rcp_map.h"
#include "rcp_old_array.h"
#include "rcp_string.h"
#include "rcp_type_etc.h"

struct rcp_type_core rcp_null_type_def = {
	0,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	rcp_null_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_ref_type_def = {
	sizeof(rcp_record_ref*),
	1,
	NULL,
	rcp_ref_init,
	rcp_ref_deinit,
	rcp_ref_copy,
	NULL,
	rcp_ref_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	rcp_ref_at,
};

struct rcp_type_core rcp_map_type_def = {
	sizeof(struct rcp_map_core),
	16,
	NULL,
	rcp_map_init,
	rcp_map_deinit,
	NULL,
	NULL,
	rcp_map_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_old_array_type_def = {
	sizeof(struct rcp_old_array_core),
	18,
	NULL,
	rcp_old_array_init,
	rcp_old_array_deinit,
	NULL,
	NULL,
	rcp_old_array_write_json,
	rcp_old_array_send_as_command,
	rcp_old_array_set,
	rcp_old_array_append,
	NULL,
	NULL,
};

struct rcp_type_core rcp_string_type_def = {
	sizeof(struct rcp_string_core),
	20,
	NULL,
	rcp_string_init,
	rcp_string_deinit,
	rcp_string_copy,
	rcp_string_compare,
	rcp_string_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_bool8_type_def = {
	1,
	25,
	NULL,
	NULL,
	NULL,
	rcp_bool8_copy,
	NULL,
	rcp_bool8_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_bool32_type_def = {
	4,
	26,
	NULL,
	NULL,
	NULL,
	rcp_bool32_copy,
	NULL,
	rcp_bool32_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_uint8_type_def = {
	1,
	32,
	NULL,
	NULL,
	NULL,
	rcp_uint8_copy,
	rcp_uint8_compare,
	rcp_uint8_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_uint16_type_def = {
	2,
	33,
	NULL,
	NULL,
	NULL,
	rcp_uint16_copy,
	rcp_uint16_compare,
	rcp_uint16_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_uint32_type_def = {
	4,
	34,
	NULL,
	NULL,
	NULL,
	rcp_uint32_copy,
	rcp_uint32_compare,
	rcp_uint32_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_uint64_type_def = {
	8,
	35,
	NULL,
	NULL,
	NULL,
	rcp_uint64_copy,
	rcp_uint64_compare,
	rcp_uint64_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_int8_type_def = {
	1,
	40,
	NULL,
	NULL,
	NULL,
	rcp_int8_copy,
	rcp_int8_compare,
	rcp_int8_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_int16_type_def = {
	2,
	41,
	NULL,
	NULL,
	NULL,
	rcp_int16_copy,
	rcp_int16_compare,
	rcp_int16_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_int32_type_def = {
	4,
	42,
	NULL,
	NULL,
	NULL,
	rcp_int32_copy,
	rcp_int32_compare,
	rcp_int32_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_int64_type_def = {
	8,
	43,
	NULL,
	NULL,
	NULL,
	rcp_int64_copy,
	rcp_int64_compare,
	rcp_int64_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_float_type_def = {
	4,
	49,
	NULL,
	NULL,
	NULL,
	rcp_float_copy,
	rcp_float_compare,
	rcp_float_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_double_type_def = {
	8,
	50,
	NULL,
	NULL,
	NULL,
	rcp_double_copy,
	rcp_double_compare,
	rcp_double_write_json,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct rcp_type_core rcp_pointer_type_def = {
	sizeof(void*),
	2,
	NULL,
	NULL,
	NULL,
	rcp_pointer_copy,
	rcp_pointer_compare,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

const rcp_type_ref rcp_null_type = &rcp_null_type_def;

const rcp_type_ref rcp_ref_type = &rcp_ref_type_def;

const rcp_type_ref rcp_map_type = &rcp_map_type_def;

const rcp_type_ref rcp_old_array_type = &rcp_old_array_type_def;

const rcp_type_ref rcp_string_type = &rcp_string_type_def;

const rcp_type_ref rcp_bool8_type = &rcp_bool8_type_def;

const rcp_type_ref rcp_bool32_type = &rcp_bool32_type_def;

const rcp_type_ref rcp_uint8_type = &rcp_uint8_type_def;

const rcp_type_ref rcp_uint16_type = &rcp_uint16_type_def;

const rcp_type_ref rcp_uint32_type = &rcp_uint32_type_def;

const rcp_type_ref rcp_uint64_type = &rcp_uint64_type_def;

const rcp_type_ref rcp_int8_type = &rcp_int8_type_def;

const rcp_type_ref rcp_int16_type = &rcp_int16_type_def;

const rcp_type_ref rcp_int32_type = &rcp_int32_type_def;

const rcp_type_ref rcp_int64_type = &rcp_int64_type_def;

const rcp_type_ref rcp_float_type = &rcp_float_type_def;

const rcp_type_ref rcp_double_type = &rcp_double_type_def;

const rcp_type_ref rcp_pointer_type = &rcp_pointer_type_def;
