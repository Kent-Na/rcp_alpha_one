//This file was generated by "gen_type_list.py".
#include "../rcp_pch.h"
#include "../rcp_utility.h"
#define RCP_INTERNAL_STRUCTURE
#include "rcp_type.h"
#include "rcp_type_list.h"

void rcp_uint8_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(uint8_t*)dst = *(uint8_t*)src;
}
int rcp_uint8_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(uint8_t*)l<*(uint8_t*)r)
		return -1;
	if (*(uint8_t*)l>*(uint8_t*)r)
		return +1;
	return 0;
}

void rcp_uint16_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(uint16_t*)dst = *(uint16_t*)src;
}
int rcp_uint16_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(uint16_t*)l<*(uint16_t*)r)
		return -1;
	if (*(uint16_t*)l>*(uint16_t*)r)
		return +1;
	return 0;
}

void rcp_uint32_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(uint32_t*)dst = *(uint32_t*)src;
}
int rcp_uint32_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(uint32_t*)l<*(uint32_t*)r)
		return -1;
	if (*(uint32_t*)l>*(uint32_t*)r)
		return +1;
	return 0;
}

void rcp_uint64_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(uint64_t*)dst = *(uint64_t*)src;
}
int rcp_uint64_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(uint64_t*)l<*(uint64_t*)r)
		return -1;
	if (*(uint64_t*)l>*(uint64_t*)r)
		return +1;
	return 0;
}

void rcp_int8_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(int8_t*)dst = *(int8_t*)src;
}
int rcp_int8_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(int8_t*)l<*(int8_t*)r)
		return -1;
	if (*(int8_t*)l>*(int8_t*)r)
		return +1;
	return 0;
}

void rcp_int16_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(int16_t*)dst = *(int16_t*)src;
}
int rcp_int16_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(int16_t*)l<*(int16_t*)r)
		return -1;
	if (*(int16_t*)l>*(int16_t*)r)
		return +1;
	return 0;
}

void rcp_int32_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(int32_t*)dst = *(int32_t*)src;
}
int rcp_int32_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(int32_t*)l<*(int32_t*)r)
		return -1;
	if (*(int32_t*)l>*(int32_t*)r)
		return +1;
	return 0;
}

void rcp_int64_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(int64_t*)dst = *(int64_t*)src;
}
int rcp_int64_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(int64_t*)l<*(int64_t*)r)
		return -1;
	if (*(int64_t*)l>*(int64_t*)r)
		return +1;
	return 0;
}

void rcp_float_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(float*)dst = *(float*)src;
}
int rcp_float_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(float*)l<*(float*)r)
		return -1;
	if (*(float*)l>*(float*)r)
		return +1;
	return 0;
}

void rcp_double_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(double*)dst = *(double*)src;
}
int rcp_double_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(double*)l<*(double*)r)
		return -1;
	if (*(double*)l>*(double*)r)
		return +1;
	return 0;
}

void rcp_pointer_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst){
	*(void**)dst = *(void**)src;
}
int rcp_pointer_compare(
		rcp_type_ref ex, rcp_data_ref l, rcp_data_ref r){
	if (*(void**)l<*(void**)r)
		return -1;
	if (*(void**)l>*(void**)r)
		return +1;
	return 0;
}
