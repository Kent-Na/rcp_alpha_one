#include "rcp_pch.h"
#include "rcp_utility.h"

#include "rcp_type.h"
#include "rcp_type_number.h"
#include "rcp_type_etc.h"
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
	while (rcp_json_is_space(**ptr)){
		(*ptr)++;
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
		rcp_record_ref rec = rcp_record_new(&rcp_type_bool8);
		uint8_t *data = rcp_record_data(rec);
		*data = 1;
		return rec;
	}
	else if (ch == 'f'){
		if (rcp_json_parse_literal(begin, end, "false") != 0){
			return NULL;
		}
		return rcp_record_new(&rcp_type_bool8);
	}
	else if (ch == 'n'){
		if (rcp_json_parse_literal(begin, end, "null") != 0){
			return NULL;
		}
		return rcp_record_new(&rcp_type_null);
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

	rcp_array_ref array = rcp_array_new(&rcp_ref_type);
	rcp_record_ref rec = NULL;

	while (1){
		rec = rcp_json_parse(begin, end);
		if (!rec)
			break;

		rcp_array_append(array, &rec);
		rcp_record_delete(rec);
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
			return array;
		}
		break;
	}
	rcp_record_delete(rec);
	return NULL;
}

rcp_record_ref rcp_json_parse_object(const char **begin, const char *end)
{
	const char* ptr = *begin;
	char ch = rcp_json_get_next_char(ptr, end);

	rcp_map_ref map = rcp_map_new(&rcp_string_type, &rcp_ref_type);

	if (ch != '{'){
		rcp_error("json:not object");
		return NULL;
	}

	ptr ++;

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
		rcp_move(&rcp_string_type,
				rcp_record_data(key),rcp_map_node_key(map, node));
		rcp_move(&rcp_ref_type,
				&value,rcp_map_node_value(map, node));

		rcp_map_set(map, node);

		rcp_record_delete(key);
		//rcp_record_delete(value);
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
			return map;
		}

		break;
	}

	//fail
	rcp_record_delete(key);
	rcp_record_delete(value);
	rcp_record_delete(map);
	return NULL;
}
/*
struct rcp_json_string{
	char *begin;
	size_t string_length;
	size_t storage_size; 
};

void rcp_json_string_init(struct rcp_json_string *str){
	size_t storage_size = 64;
	str->begin = malloc(storage_size);
	str->string_length= 0;
	str->storage_size = storage_size;
}

void rcp_json_string_free(struct rcp_json_string *str){
	free(str->begin);
}
inline void rcp_json_string_put(struct rcp_json_string *str, char ch)
{
	if (str->string_length == str->storage_size){
		size_t storage_size = str->storage_size;
		str->begin = realloc(str->begin, storage_size*2);
	}
	str->begin[str->string_length] = ch;
	str->string_length ++;
}
*/

rcp_record_ref rcp_json_parse_string(const char **begin, const char *end)
{
	const char* ptr = *begin;
	char ch = rcp_json_get_next_char(ptr, end);

	if (ch != '\"'){
		rcp_error("json:not string");
		return NULL;
	}

	rcp_string_ref out = rcp_string_new(NULL);
	
	while (1){

		ptr ++;
		ch = rcp_json_get_next_char(ptr, end);

		if (ch == '\"'){
			//end of string
			rcp_string_put(out, '\0');
			*begin = ptr + 1;
			return out;
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
	rcp_record_delete(out);
}

int rcp_json_parse_literal(const char **begin, const char *end, 
		const char *literal)
{
	const char *ptr_src = *begin + 1;
	const char *ptr_dst = literal + 1;
	int i;
	while (ptr_dst){
		if (ptr_src > end){
			rcp_error("json:not enough char");
			return -1;
		}
		if (ptr_src != ptr_dst){
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
		rcp_record_ref rec = rcp_record_new(&rcp_int64_type);
		int64_t *dat = rcp_record_data(rec);
		*dat = significand;
		return rec;
	}
	else{
		//double
		rcp_record_ref rec = rcp_record_new(&rcp_double_type);
		double *dat = rcp_record_data(rec);
		*dat = ldexp((double)significand, exponential_part);
		return rec;
	}
}

