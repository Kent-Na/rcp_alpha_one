#include "rcp_pch.h"
#include "rcp_utility.h"
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
/*
int rcp_json_parse_as_structure(const char **begin, const char *end){
	rcp_json_skip_space(begin, end);
	const char ch = rcp_json_get_next_char(*begin, end);
	if (ch != '{'){
		rcp_error("json:not object");
		return -1;
	}
	
	rcp_json_parse_object_as_structure(begin, end);
}

int rcp_json_parse_object_as_structure(const char **begin, const char *end)
{
	const char* ptr = *begin;
	char ch = rcp_json_get_next_char(ptr, end);
	const rcp_structure_ref

	if (ch != '{'){
		rcp_error("json:not object");
		return -1;
	}

	ptr ++;

	while (1){
		//get key
		const char* key;
		rcp_json_skip_space(&ptr, end);
		rcp_json_parse_string(begin, end, &key);

		rcp_structure_ref st;
		rcp_parameter_ref param = rcp_parameter_from_str(st, key);
		rcp_type_ref type = rcp_parameter_type(param); 

		if (ch != ':'){
			//end of array
			return -1;
		}

		//get object
		rcp_json_skip_space(&ptr, end);
		rcp_type_parse_json(type, begin, end);
		rcp_json_skip_space(&ptr, end);
		ch = rcp_json_get_next_char(ptr, end);

		if (ch == ','){
			//next element
			continue;
		}

		if (ch == '}'){
			//end of array
			return 0;
		}

		return -1;
	}
}
*/
void rcp_json_parse(const char **begin, const char *end)
{
	//const char* ptr = *begin;

	rcp_json_skip_space(begin, end);

	const char ch = rcp_json_get_next_char(*begin, end);
	
	if (ch == '{'){
		rcp_json_parse_object(begin, end);
	}
	else if (ch == '['){
		rcp_json_parse_array(begin ,end);
	}
	else if (ch == '\"'){
		const char* str = NULL;;
		rcp_json_parse_string(begin ,end, &str);
	}
	else if (ch == '-' || rcp_json_is_number(ch)){
		struct rcp_json_number num;
		rcp_json_parse_number(begin, end, &num);
	}
	else if (ch == 't'){
		rcp_json_parse_literal(begin, end, "true", 4);
	}
	else if (ch == 'f'){
		rcp_json_parse_literal(begin, end, "false", 5);
	}
	else if (ch == 'n'){
		rcp_json_parse_literal(begin, end, "null", 4);
	}
}

int rcp_json_parse_array(const char **begin, const char *end)
{
	const char* ptr = *begin;
	char ch = rcp_json_get_next_char(ptr, end);

	if (ch != '['){
		rcp_error("json:not array");
		return -1;
	}

	ptr ++;

	while (1){
		rcp_json_parse(begin, end);

		rcp_json_skip_space(&ptr, end);
		ch = rcp_json_get_next_char(ptr, end);

		if (ch == ','){
			//next element
			continue;
		}

		if (ch == ']'){
			//end of array
			return 0;
		}

		return -1;
	}
}

int rcp_json_parse_object(const char **begin, const char *end)
{
	const char* ptr = *begin;
	char ch = rcp_json_get_next_char(ptr, end);

	if (ch != '{'){
		rcp_error("json:not object");
		return -1;
	}

	ptr ++;

	while (1){
		rcp_json_skip_space(&ptr, end);
		//get key
		const char* key;
		rcp_json_parse_string(begin, end, &key);
		rcp_json_skip_space(&ptr, end);

		if (ch != ':'){
			return -1;
		}

		//get object
		rcp_json_parse(begin, end);
		rcp_json_skip_space(&ptr, end);
		ch = rcp_json_get_next_char(ptr, end);

		if (ch == ','){
			//next element
			continue;
		}

		if (ch == '}'){
			//end of array
			return 0;
		}

		return -1;
	}
}

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

int rcp_json_parse_string(const char **begin, const char *end, 
		const char **result)
{
	const char* ptr = *begin;
	char ch = rcp_json_get_next_char(ptr, end);

	if (ch != '\"'){
		rcp_error("not string");
		return -1;
	}

	struct rcp_json_string out;
	rcp_json_string_init(&out);
	
	while (1){

		ptr ++;
		ch = rcp_json_get_next_char(ptr, end);

		if (ch == '\"'){
			//end of string
			rcp_json_string_put(&out, '\0');
			*begin = ptr + 1;
			*result = out.begin;
			return 0;
		}
		if (ch == '\\'){
			ptr ++;
			ch = rcp_json_get_next_char(ptr, end);

			switch (ch){
				case '\"':
					rcp_json_string_put(&out, '\"');
					break;
				case '\\':
					rcp_json_string_put(&out, '\\');
					break;
				case '/':
					rcp_json_string_put(&out, '/');
					break;
				case 'b':
					rcp_json_string_put(&out, '\b');
					break;
				case 'f':
					rcp_json_string_put(&out, '\f');
					break;
				case 'n':
					rcp_json_string_put(&out, '\n');
					break;
				case 'r':
					rcp_json_string_put(&out, '\r');
					break;
				case 't':
					rcp_json_string_put(&out, '\t');
					break;
				case 'u':
					{
						rcp_error("json:can not handle unicode escape");
						return;
					}
					break;
				default:
					rcp_error("json:undefined escape");
					rcp_json_string_free(&out);
					return -1;
			}
			continue;
		}
		{
			rcp_json_string_put(&out, ch);
		}
	}
}

int rcp_json_parse_literal(const char **begin, const char *end, 
		const char *literal, size_t len)
{
	if (*begin+len > end){
		rcp_error("json:not enough char");
		return -1;
	}
	
	const char *ptr_src = *begin + 1;
	const char *ptr_dst = literal + 1;
	int i;
	for (i = 1; i < len; i++){
		if (ptr_src != ptr_dst){
			rcp_error("json:undefined literal");
			return -1;
		}
	}
	*begin = ptr_src;
	return 0;
}


int rcp_json_parse_number(const char** begin, const char* end,
		struct rcp_json_number *result)
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
		return -1;
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
			return -1;
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
			return -1;
		}
	}

	*begin = ptr;
	result->significand = significand * sign;
	result->exponential_part = exponential_part*exp_sign + 
		exponential_part_offset;
	return 0;
}

