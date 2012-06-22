#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_string.h"
#include "rcp_tree.h"
#include "rcp_map.h"
#include "rcp_array.h"

#include "rcp_json.h"

inline int rcp_json_is_number(char ch)
{
	return '0' <= ch && ch <= '9';
}

inline int rcp_json_is_space(char ch)
{
	return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

inline char rcp_json_get_next_char(const char *ptr, const char *end)
{
	if (ptr >= end)
		return '\0';
	else 
		return *ptr;
}

inline void rcp_json_skip_space(const char **ptr, const char *end)
{
	char ch = rcp_json_get_next_char(*ptr, end);
	while (rcp_json_is_space(ch)){
		(*ptr)++;
		ch = rcp_json_get_next_char(*ptr, end);
	}
}

rcp_record_ref rcp_json_parse(
		const char **begin, const char *end)
{
	//const char* ptr = *begin;

	rcp_json_skip_space(begin, end);

	const char ch = rcp_json_get_next_char(*begin, end);
	
	if (ch == '{'){
		return rcp_json_parse_object(begin, end);
	}
	else if (ch == '['){
		return rcp_json_parse_array(begin ,end);
	}
	else if (ch == '\"'){
		return rcp_json_parse_string(begin ,end);
	}
	else if (ch == '-' || rcp_json_is_number(ch)){
		return rcp_json_parse_number(begin ,end);
	}
	else if (ch == 't'){
		if (rcp_json_parse_literal(begin, end, "true") != 0){
			return NULL;
		}
		rcp_record_ref rec = rcp_record_new(rcp_bool8_type);
		uint8_t *data = (uint8_t*)rcp_record_data(rec);
		*data = 1;
		return rec;
	}
	else if (ch == 'f'){
		if (rcp_json_parse_literal(begin, end, "false") != 0){
			return NULL;
		}
		rcp_record_ref rec = rcp_record_new(rcp_bool8_type);
		uint8_t *data = (uint8_t*)rcp_record_data(rec);
		*data = 0;
		return rec;
	}
	else if (ch == 'n'){
		if (rcp_json_parse_literal(begin, end, "null") != 0){
			return NULL;
		}
		return rcp_record_new(rcp_null_type);
	}
	return NULL;
}

rcp_record_ref rcp_json_parse_array(const char **begin, const char *end)
{
	const char* ptr = *begin;
	char ch = rcp_json_get_next_char(ptr, end);

	if (ch != '['){
		rcp_error("json:not array");
		return NULL;
	}

	ptr ++;

	rcp_record_ref array_rec = rcp_array_new_rec(rcp_ref_type);
	rcp_array_ref array = rcp_record_data(array_rec);
	rcp_record_ref rec = NULL;

	while (1){
		rcp_json_skip_space(&ptr, end);
		rec = rcp_json_parse(&ptr, end);
		if (!rec){
			rcp_caution("json:array value");
			break;
		}

		rcp_array_append(array, &rec);
		rcp_record_release(rec);
		rec = NULL;

		rcp_json_skip_space(&ptr, end);
		ch = rcp_json_get_next_char(ptr, end);
		ptr ++;

		if (ch == ','){
			//next element
			continue;
		}

		if (ch == ']'){
			//end of array
			*begin = ptr;
			return array_rec;
		}
		break;
	}
	rcp_record_release(rec);
	rcp_record_release(array_rec);
	return NULL;
}

rcp_record_ref rcp_json_parse_object(const char **begin, const char *end)
{
	const char* ptr = *begin;
	char ch = rcp_json_get_next_char(ptr, end);


	if (ch != '{'){
		rcp_error("json:not object");
		return NULL;
	}

	ptr ++;

	rcp_record_ref map_rec = rcp_map_new_rec(rcp_string_type, rcp_ref_type);
	rcp_map_ref map = rcp_record_data(map_rec);
	rcp_record_ref key = NULL;
	rcp_record_ref value = NULL;

	while (1){
		rcp_json_skip_space(&ptr, end);
		key = rcp_json_parse_string(&ptr, end);
		if (!key){
			rcp_caution("json:key");
			break;
		}

		rcp_json_skip_space(&ptr, end);
		ch = rcp_json_get_next_char(ptr, end);
		ptr ++;
		if (ch != ':'){
			rcp_caution("json:separator");
			break;
		}

		//get object
		value = rcp_json_parse(&ptr, end);
		if (!value){
			rcp_caution("json:value");
			break;
		}
		rcp_json_skip_space(&ptr, end);

		rcp_map_node_ref node = rcp_map_node_new(map);
		rcp_move(rcp_string_type,
				rcp_record_data(key),rcp_map_node_key(map, node));
		rcp_move(rcp_ref_type,
				(rcp_data_ref)&value,rcp_map_node_value(map, node));
		rcp_map_set(map, node);

		rcp_record_init(key);
		rcp_record_release(key);
		key = NULL;
		value = NULL;

		ch = rcp_json_get_next_char(ptr, end);
		ptr ++;

		if (ch == ','){
			//next element
			continue;
		}

		if (ch == '}'){
			*begin = ptr;
			return map_rec;
		}

		break;
	}

	//fail
	rcp_record_release(key);
	rcp_record_release(value);
	rcp_record_release(map_rec);
	return NULL;
}

rcp_record_ref rcp_json_parse_string(const char **begin, const char *end)
{
	const char* ptr = *begin;
	char ch = rcp_json_get_next_char(ptr, end);

	if (ch != '\"'){
		rcp_error("json:not string");
		return NULL;
	}

	rcp_record_ref out_rec = rcp_string_new_rec(NULL);
	rcp_string_ref out = rcp_record_data(out_rec);
	
	while (1){

		ptr ++;
		ch = rcp_json_get_next_char(ptr, end);

		if (ch == '\"'){
			//end of string
			rcp_string_put(out, '\0');
			*begin = ptr + 1;
			return out_rec;
		}
		if (ch == '\\'){
			ptr ++;
			ch = rcp_json_get_next_char(ptr, end);

			if (ch == '\"')
				rcp_string_put(out, '\"');
			else if (ch == '\\')
				rcp_string_put(out, '\\');
			else if (ch == '/')
				rcp_string_put(out, '/');
			else if (ch == 'b')
				rcp_string_put(out, '\b');
			else if (ch == 'f')
				rcp_string_put(out, '\f');
			else if (ch == 'n')
				rcp_string_put(out, '\n');
			else if (ch == 'r')
				rcp_string_put(out, '\r');
			else if (ch == 't')
				rcp_string_put(out, '\t');
			else if (ch == 'u'){
				rcp_error("json:can not handle unicode escape");
				break;
			}
			else{
				rcp_error("json:undefined escape");
				break;
			}

			continue;
		}
		{
			rcp_string_put(out, ch);
		}
	}
	rcp_record_release(out_rec);
	return NULL;
}

int rcp_json_parse_literal(const char **begin, const char *end, 
		const char *literal)
{
	const char *ptr_src = *begin + 1;
	const char *ptr_dst = literal + 1;
	while (*ptr_dst){
		if (ptr_src > end){
			rcp_error("json:not enough char");
			return -1;
		}
		if (*ptr_src != *ptr_dst){
			rcp_error("json:undefined literal");
			return -1;
		}
		ptr_src++;
		ptr_dst++;
	}
	*begin = ptr_src;
	return 0;
}

rcp_extern rcp_record_ref rcp_json_parse_number(
		const char** begin, const char* end)
{
	int sign = 1;
	int exp_sign = 1;
	int64_t significand= 0;
	//uint64_t fraction_part = 0;
	int64_t exponential_part_offset = 0;
	int64_t exponential_part = 0;

	const char *ptr = *begin;
	char ch;

	ch = rcp_json_get_next_char(ptr, end);

	if (ch == '-'){
		sign = -1;
		ptr++;
	}

	// interger part
	ch = rcp_json_get_next_char(ptr, end);

	if (ch == '0'){
		ptr++;
	}
	else if (rcp_json_is_number(ch)){
		while (rcp_json_is_number(ch)){
			significand = significand * 10 + (ch - '0');
			ptr ++;
			ch = rcp_json_get_next_char(ptr, end);
		}
	}
	else{
		rcp_error("json:not a number(int)");
		return NULL;
	}
	
	// fraction part
	ch = rcp_json_get_next_char(ptr, end);

	if (ch == '.'){
		ptr++;

		ch = rcp_json_get_next_char(ptr, end);

		if (rcp_json_is_number(ch)){
			while (rcp_json_is_number(ch)){
				significand = significand * 10 + (ch - '0');
				exponential_part_offset --;
				ptr ++;
				ch = rcp_json_get_next_char(ptr, end);
			}
		}
		else{
			rcp_error("json:not a number(frac)");
			return NULL;
		}
	}
	significand*=sign;

	ch = rcp_json_get_next_char(ptr, end);

	// exponential part
	if (ch == 'e' || ch == 'E'){
		ptr++;

		ch = rcp_json_get_next_char(ptr, end);

		if (ch == '-'){
			ptr++;
			exp_sign = -1;
		}
		else if (ch == '+'){
			ptr++;
		}

		ch = rcp_json_get_next_char(ptr, end);

		if (rcp_json_is_number(ch)){
			while (rcp_json_is_number(ch)){
				exponential_part = exponential_part * 10 + (ch - '0');
				ptr ++;
				ch = rcp_json_get_next_char(ptr, end);
			}
		}
		else{
			rcp_error("json:not a number(exp)");
			return NULL;
		}
	}

	*begin = ptr;
	exponential_part = exponential_part*exp_sign + 
		exponential_part_offset;

	if (exponential_part == 0){
		//int64_t
		rcp_record_ref rec = rcp_record_new(rcp_int64_type);
		int64_t *dat = (int64_t*)rcp_record_data(rec);
		*dat = significand;
		return rec;
	}
	else{
		//double
		rcp_record_ref rec = rcp_record_new(rcp_double_type);
		double *dat = (double*)rcp_record_data(rec);
		*dat = significand * pow(10.0 , exponential_part);
		return rec;
	}
}

void rcp_json_write_record(rcp_record_ref rec, rcp_string_ref out)
{
	if (!rec){
		rcp_string_append_c_str(out,"null");
		return;
	}
	rcp_type_ref type = rcp_record_type(rec);
	rcp_data_ref data = rcp_record_data(rec);
	if (type == rcp_string_type)
		rcp_json_write_string(data, out);
	else if (type == rcp_int64_type)
		rcp_json_write_int64(data, out);
	else if (type == rcp_double_type)
		rcp_json_write_double(data, out);
	else if (type == rcp_map_type)
		rcp_json_write_map(data, out);
	else if (type == rcp_array_type)
		rcp_json_write_array(data, out);
	else if (type == rcp_bool8_type){
		if (*(uint8_t*)rcp_record_data(rec))
			rcp_string_append_c_str(out,"true");
		else
			rcp_string_append_c_str(out,"false");
	}
	else if (type == rcp_bool32_type){
		if (*(uint32_t*)rcp_record_data(rec))
			rcp_string_append_c_str(out,"true");
		else
			rcp_string_append_c_str(out,"false");
	}
	else if (type == rcp_null_type)
		rcp_string_append_c_str(out,"null");
	else 
		rcp_string_append_c_str(out,"null");
}

void rcp_json_write_number(uint64_t value, rcp_string_ref out)
{
	uint64_t num = value;
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
void rcp_json_write_int64(void *value, rcp_string_ref out){
	int64_t num = *(int64_t*)value;
	if (num < 0){
		rcp_string_put(out, '-');
		num *= -1;
	}
	rcp_json_write_number(num, out);
}
void rcp_json_write_double(void *value, rcp_string_ref out){
	double num = *(double*)value;

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
	rcp_json_write_int64(&exp, out);
}
void rcp_json_write_map(rcp_map_ref map, rcp_string_ref out)
{

#ifdef RCP_SELF_TEST
	if (rcp_map_key_type(map) != rcp_string_type){
		rcp_error("json:map key type");
		return;
	}
	if (rcp_map_value_type(map) != rcp_ref_type){
		rcp_error("json:map value type");
		return;
	}

#endif

	rcp_map_node_ref node = rcp_map_begin(map);
	rcp_string_put(out, '{');
	while (node){
		rcp_json_write_string(rcp_map_node_key(map, node), out);

		rcp_string_put(out, ':');

		rcp_record_ref value = NULL;
		rcp_copy(rcp_ref_type, rcp_map_node_value(map, node),
				(rcp_data_ref)&value);
		rcp_json_write_record(value, out);
		rcp_record_release(value);
		
		node = rcp_map_node_next(node);
		if (node)
			rcp_string_put(out, ',');
	}
	rcp_string_put(out, '}');
}

void rcp_json_write_array(rcp_array_ref array, rcp_string_ref out)
{
#ifdef RCP_SELF_TEST
	if (rcp_array_data_type(array) != rcp_ref_type){
		rcp_error("json:array value type");
		return;
	}
#endif

	rcp_array_iterater_ref itr = rcp_array_begin(array);
	rcp_string_put(out, '[');
	while (itr){

		rcp_record_ref value = NULL;
		rcp_copy(rcp_ref_type, rcp_array_iterater_data(array, itr), 
				(rcp_data_ref)&value);
		rcp_json_write_record(value, out);
		rcp_record_release(value);
		
		itr = rcp_array_iterater_next(array, itr);
		if (itr)
			rcp_string_put(out, ',');
	}
	rcp_string_put(out, ']');
}
void rcp_json_write_string(rcp_string_ref str, rcp_string_ref out)
{
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
