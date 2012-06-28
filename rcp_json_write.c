
void rcp_record_write_json(rcp_record_ref rec, rcp_string_ref out)
{
	if (!rec){
		rcp_string_append_c_str(out,"null");
		return;
	}
	rcp_type_ref type = rcp_record_type(rec);
	rcp_data_ref data = rcp_record_data(rec);
	if (type == rcp_string_type)
		rcp_string(data, out);
	else if (type == rcp_int64_type)
		rcp_int64(data, out);
	else if (type == rcp_double_type)
		rcp_double(data, out);
	else if (type == rcp_map_type)
		rcp_map(data, out);
	else if (type == rcp_array_type)
		rcp_array(data, out);
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

void rcp_number_write_json(uint64_t value, rcp_string_ref out)
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
void rcp_int64_write_json(void *value, rcp_string_ref out){
	int64_t num = *(int64_t*)value;
	if (num < 0){
		rcp_string_put(out, '-');
		num *= -1;
	}
	rcp_number(num, out);
}
void rcp_double_write_json(void *value, rcp_string_ref out){
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
	rcp_int64(&exp, out);
}
void rcp_map_write_json(rcp_map_ref map, rcp_string_ref out)
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
		rcp_string(rcp_map_node_key(map, node), out);

		rcp_string_put(out, ':');

		rcp_record_ref value = NULL;
		rcp_copy(rcp_ref_type, rcp_map_node_value(map, node),
				(rcp_data_ref)&value);
		rcp_record(value, out);
		rcp_record_release(value);
		
		node = rcp_map_node_next(node);
		if (node)
			rcp_string_put(out, ',');
	}
	rcp_string_put(out, '}');
}

void rcp_array_write_json(rcp_array_ref array, rcp_string_ref out)
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
		rcp_record(value, out);
		rcp_record_release(value);
		
		itr = rcp_array_iterater_next(array, itr);
		if (itr)
			rcp_string_put(out, ',');
	}
	rcp_string_put(out, ']');
}
void rcp_string_write_json(rcp_string_ref str, rcp_string_ref out)
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
