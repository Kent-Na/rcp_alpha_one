#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#include "rcp_type.h"
#include "rcp_record.h"
#include "types/rcp_string.h"
#include "types/rcp_map.h"
#include "types/rcp_array.h"
#include "types/rcp_struct.h"
#include "types/rcp_dict.h"
#include "types/rcp_type_list.h"

void rcp_null_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref out)
{
	rcp_string_append_c_str(out,"null");
}

void rcp_ref_write_json(rcp_type_ref type, 
		rcp_data_ref data, rcp_string_ref out)
{
	rcp_record_ref rec = *(rcp_record_ref *)data;
	if (!rec || !rcp_record_data(rec)){
		rcp_null_write_json(rcp_null_type, NULL, out);
		return;
	}
	rcp_write_json(rcp_record_type(rec), rcp_record_data(rec), out);	
}

void rcp_bool8_write_json(rcp_type_ref type, 
		rcp_data_ref value, rcp_string_ref out)
{
	if (*(uint8_t*)value)
		rcp_string_append_c_str(out,"true");
	else
		rcp_string_append_c_str(out,"false");
}

void rcp_bool32_write_json(rcp_type_ref type, 
		rcp_data_ref value, rcp_string_ref out)
{
	if (*(uint8_t*)value)
		rcp_string_append_c_str(out,"true");
	else
		rcp_string_append_c_str(out,"false");
}

void rcp_uint_write_json(uint64_t num, rcp_string_ref out)
{
	if (num == 0){
		rcp_string_put(out, '0');
		return;
	}

	char d_stack[32];
	char *p = d_stack;
	while (num != 0){
		*p = num % 10;
		p ++;
		num /= 10;
	}

	while (p != d_stack){
		p --;
		rcp_string_put(out, '0' + *p);
	}
}

void rcp_int_write_json(int64_t value, rcp_string_ref out){
	if (value< 0){
		rcp_string_put(out, '-');
		rcp_uint_write_json(-value, out);
	}
	else{
		rcp_uint_write_json(value, out);
	}
}

void rcp_uint64_write_json(rcp_type_ref type,
		rcp_data_ref value, rcp_string_ref out)
{
	rcp_uint_write_json(*(uint64_t*)value, out);
}

void rcp_uint32_write_json(rcp_type_ref type,
		rcp_data_ref value, rcp_string_ref out)
{
	rcp_uint_write_json(*(uint32_t*)value, out);
}

void rcp_uint16_write_json(rcp_type_ref type,
		rcp_data_ref value, rcp_string_ref out)
{
	rcp_uint_write_json(*(uint16_t*)value, out);
}

void rcp_uint8_write_json(rcp_type_ref type,
		rcp_data_ref value, rcp_string_ref out)
{
	rcp_uint_write_json(*(uint8_t*)value, out);
}

void rcp_int64_write_json(rcp_type_ref type,
		rcp_data_ref value, rcp_string_ref out)
{
	rcp_int_write_json(*(uint64_t*)value, out);
}

void rcp_int32_write_json(rcp_type_ref type,
		rcp_data_ref value, rcp_string_ref out)
{
	rcp_int_write_json(*(uint32_t*)value, out);
}

void rcp_int16_write_json(rcp_type_ref type,
		rcp_data_ref value, rcp_string_ref out)
{
	rcp_int_write_json(*(uint16_t*)value, out);
}

void rcp_int8_write_json(rcp_type_ref type,
		rcp_data_ref value, rcp_string_ref out)
{
	rcp_int_write_json(*(uint8_t*)value, out);
}

void rcp_float_val_write_json(double num, rcp_string_ref out){

	if (num == 0.0){
		rcp_string_put(out, '0');
		return;
	}

	int exp = floor(log10(num));
	double x = num*pow(10.0,-exp);

	if (x < 0)
		rcp_string_put(out, '-');
	x = fabs(x);

	uint64_t frac = x * 10000000000LL;

	char d_stack[32];
	char *p = d_stack;
	while (frac!= 0){
		*p = frac % 10;
		p ++;
		frac /= 10;
	}

	if (p != d_stack){
		p --;
		rcp_string_put(out, '0' + *p);
		rcp_string_put(out, '.');
	}

	while (p != d_stack){
		p --;
		rcp_string_put(out, '0' + *p);
	}

	rcp_string_put(out, 'e');
	rcp_int_write_json(exp, out);
}

void rcp_double_write_json(rcp_type_ref type,
		rcp_data_ref value, rcp_string_ref out)
{
	rcp_float_val_write_json(*(double*)value, out);
}

void rcp_float_write_json(rcp_type_ref type,
		rcp_data_ref value, rcp_string_ref out)
{
	rcp_float_val_write_json(*(float*)value, out);
}

void rcp_struct_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out)
{
	rcp_struct_ref st = (rcp_struct_ref)data;
	rcp_struct_param_ref param = rcp_struct_type_begin(type);

	rcp_string_put(out, '{');
	while (param){
		rcp_write_json(rcp_string_type,
				(rcp_data_ref)rcp_struct_param_name(param), out);

		rcp_string_put(out, ':');

		rcp_write_json(rcp_struct_param_type(param), 
				rcp_struct_data(st, param), out);
		
		param = rcp_struct_param_next(type, param);
		if (param)
			rcp_string_put(out, ',');
	}
	rcp_string_put(out, '}');
}

void rcp_dict_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out)
{
	rcp_dict_ref dict = (rcp_dict_ref)data;

	const rcp_type_ref key_type = rcp_dict_type_key_type(type);
	const rcp_type_ref data_type = rcp_dict_type_data_type(type);

#ifdef RCP_SELF_TEST
	if (key_type != rcp_string_type){
		rcp_error("json:dict key type");
		return;
	}
#endif

	rcp_dict_node_ref node = rcp_dict_begin(dict);
	rcp_string_put(out, '{');
	while (node){
		rcp_write_json(rcp_string_type,
				rcp_dict_node_key(type, node), out);

		rcp_string_put(out, ':');

		rcp_write_json(data_type, 
				rcp_dict_node_data(type, node), out);
		
		node = rcp_dict_node_next(node);
		if (node)
			rcp_string_put(out, ',');
	}
	rcp_string_put(out, '}');
}

void rcp_map_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out)
{
	rcp_map_ref map = (rcp_map_ref)data;

#ifdef RCP_SELF_TEST
	if (rcp_map_key_type(map) != rcp_string_type){
		rcp_error("json:map key type");
		return;
	}
#endif

	rcp_type_ref val_type = rcp_map_value_type(map);

	rcp_map_node_ref node = rcp_map_begin(map);
	rcp_string_put(out, '{');
	while (node){
		rcp_write_json(rcp_string_type,
				rcp_map_node_key(map, node), out);

		rcp_string_put(out, ':');

		rcp_write_json(val_type, 
				rcp_map_node_value(map, node), out);
		
		node = rcp_map_node_next(node);
		if (node)
			rcp_string_put(out, ',');
	}
	rcp_string_put(out, '}');
}

void rcp_array_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out)
{
	rcp_array_ref array = (rcp_array_ref)data;
	rcp_type_ref array_type = type;
	rcp_type_ref elem_type = rcp_array_type_data_type(type);

	rcp_array_iterater_ref itr = rcp_array_begin(array);
	rcp_string_put(out, '[');
	while (itr){
		rcp_write_json(elem_type, 
				rcp_array_iterater_data(itr), out);

		itr = rcp_array_iterater_next(array_type, array, itr);
		if (itr)
			rcp_string_put(out, ',');
	}
	rcp_string_put(out, ']');
}

void rcp_string_write_json(rcp_type_ref type,
		rcp_data_ref data, rcp_string_ref out)
{
	rcp_string_ref str = (rcp_string_ref)data;

	const char* c_str = rcp_string_c_str(str);

	rcp_string_put(out, '\"');
	while (*c_str){
		char ch = *c_str;
		if (ch == '"')
			rcp_string_append_c_str(out, "\\\"");
		else if (ch == '\\')
			rcp_string_append_c_str(out, "\\\\");
		else if (ch == '/')
			rcp_string_append_c_str(out, "\\/");
		else if (ch == '\b')
			rcp_string_append_c_str(out, "\\b");
		else if (ch == '\f')
			rcp_string_append_c_str(out, "\\f");
		else if (ch == '\n')
			rcp_string_append_c_str(out, "\\n");
		else if (ch == '\r')
			rcp_string_append_c_str(out, "\\r");
		else if (ch == '\t')
			rcp_string_append_c_str(out, "\\t");
		else{
			rcp_string_put(out, ch);
		}
		c_str++;
	}
	rcp_string_put(out, '\"');
}
