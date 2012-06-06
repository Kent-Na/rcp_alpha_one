//This file was generated by program.
#include "rcp_pch.h"
#include "rcp_utility.h"
#define RCP_INTERNAL_STRUCTURE
#include "rcp_type.h"
#include "rcp_type_list.h"
#define RCP_TYPE_POINTER (0)

void rcp_uint8_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(uint8_t*)dst = *(uint8_t*)src;
}
int rcp_uint8_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(uint8_t*)l - *(uint8_t*)r;
	//so use that.
	if (*(uint8_t*)l<*(uint8_t*)r)
		return -1;
	if (*(uint8_t*)l>*(uint8_t*)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_uint8_type_def = {
	sizeof(uint8_t),
	RCP_TYPE_UINT8,
	"uint8",
	NULL,//init
	NULL,//free
	rcp_uint8_type_copy,
	rcp_uint8_type_compare,
};

void rcp_uint16_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(uint16_t*)dst = *(uint16_t*)src;
}
int rcp_uint16_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(uint16_t*)l - *(uint16_t*)r;
	//so use that.
	if (*(uint16_t*)l<*(uint16_t*)r)
		return -1;
	if (*(uint16_t*)l>*(uint16_t*)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_uint16_type_def = {
	sizeof(uint16_t),
	RCP_TYPE_UINT16,
	"uint16",
	NULL,//init
	NULL,//free
	rcp_uint16_type_copy,
	rcp_uint16_type_compare,
};

void rcp_uint32_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(uint32_t*)dst = *(uint32_t*)src;
}
int rcp_uint32_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(uint32_t*)l - *(uint32_t*)r;
	//so use that.
	if (*(uint32_t*)l<*(uint32_t*)r)
		return -1;
	if (*(uint32_t*)l>*(uint32_t*)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_uint32_type_def = {
	sizeof(uint32_t),
	RCP_TYPE_UINT32,
	"uint32",
	NULL,//init
	NULL,//free
	rcp_uint32_type_copy,
	rcp_uint32_type_compare,
};

void rcp_uint64_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(uint64_t*)dst = *(uint64_t*)src;
}
int rcp_uint64_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(uint64_t*)l - *(uint64_t*)r;
	//so use that.
	if (*(uint64_t*)l<*(uint64_t*)r)
		return -1;
	if (*(uint64_t*)l>*(uint64_t*)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_uint64_type_def = {
	sizeof(uint64_t),
	RCP_TYPE_UINT64,
	"uint64",
	NULL,//init
	NULL,//free
	rcp_uint64_type_copy,
	rcp_uint64_type_compare,
};

void rcp_int8_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(int8_t*)dst = *(int8_t*)src;
}
int rcp_int8_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(int8_t*)l - *(int8_t*)r;
	//so use that.
	if (*(int8_t*)l<*(int8_t*)r)
		return -1;
	if (*(int8_t*)l>*(int8_t*)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_int8_type_def = {
	sizeof(int8_t),
	RCP_TYPE_INT8,
	"int8",
	NULL,//init
	NULL,//free
	rcp_int8_type_copy,
	rcp_int8_type_compare,
};

void rcp_int16_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(int16_t*)dst = *(int16_t*)src;
}
int rcp_int16_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(int16_t*)l - *(int16_t*)r;
	//so use that.
	if (*(int16_t*)l<*(int16_t*)r)
		return -1;
	if (*(int16_t*)l>*(int16_t*)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_int16_type_def = {
	sizeof(int16_t),
	RCP_TYPE_INT16,
	"int16",
	NULL,//init
	NULL,//free
	rcp_int16_type_copy,
	rcp_int16_type_compare,
};

void rcp_int32_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(int32_t*)dst = *(int32_t*)src;
}
int rcp_int32_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(int32_t*)l - *(int32_t*)r;
	//so use that.
	if (*(int32_t*)l<*(int32_t*)r)
		return -1;
	if (*(int32_t*)l>*(int32_t*)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_int32_type_def = {
	sizeof(int32_t),
	RCP_TYPE_INT32,
	"int32",
	NULL,//init
	NULL,//free
	rcp_int32_type_copy,
	rcp_int32_type_compare,
};

void rcp_int64_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(int64_t*)dst = *(int64_t*)src;
}
int rcp_int64_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(int64_t*)l - *(int64_t*)r;
	//so use that.
	if (*(int64_t*)l<*(int64_t*)r)
		return -1;
	if (*(int64_t*)l>*(int64_t*)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_int64_type_def = {
	sizeof(int64_t),
	RCP_TYPE_INT64,
	"int64",
	NULL,//init
	NULL,//free
	rcp_int64_type_copy,
	rcp_int64_type_compare,
};

void rcp_float_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(float*)dst = *(float*)src;
}
int rcp_float_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(float*)l - *(float*)r;
	//so use that.
	if (*(float*)l<*(float*)r)
		return -1;
	if (*(float*)l>*(float*)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_float_type_def = {
	sizeof(float),
	RCP_TYPE_FLOAT,
	"float",
	NULL,//init
	NULL,//free
	rcp_float_type_copy,
	rcp_float_type_compare,
};

void rcp_double_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(double*)dst = *(double*)src;
}
int rcp_double_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(double*)l - *(double*)r;
	//so use that.
	if (*(double*)l<*(double*)r)
		return -1;
	if (*(double*)l>*(double*)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_double_type_def = {
	sizeof(double),
	RCP_TYPE_DOUBLE,
	"double",
	NULL,//init
	NULL,//free
	rcp_double_type_copy,
	rcp_double_type_compare,
};

void rcp_pointer_type_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(void**)dst = *(void**)src;
}
int rcp_pointer_type_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	//this implement causes over flow
	//return *(void**)l - *(void**)r;
	//so use that.
	if (*(void**)l<*(void**)r)
		return -1;
	if (*(void**)l>*(void**)r)
		return +1;
	return 0;
}
const struct rcp_type_core rcp_pointer_type_def = {
	sizeof(void*),
	RCP_TYPE_POINTER,
	"pointer",
	NULL,//init
	NULL,//free
	rcp_pointer_type_copy,
	rcp_pointer_type_compare,
};
