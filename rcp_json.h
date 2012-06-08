

//type table
//json		<->		rcp

//object	<->		map(string, ref) 
//array		<->		array(ref)
//string	<->		string
//number	<->		double or int64
//bool		<->		bool8
//null		<->		null

//map		<--		struct

rcp_extern rcp_record_ref rcp_json_parse(
		const char **begin, const char *end);

rcp_extern rcp_record_ref rcp_json_parse_object(
		const char **begin, const char *end);
rcp_extern rcp_record_ref rcp_json_parse_array(
		const char **begin, const char *end);

rcp_extern rcp_record_ref rcp_json_parse_string(
		const char **begin, const char *end);

rcp_extern int rcp_json_parse_literal(const char **begin, const char *end, 
		const char *literal);

rcp_extern rcp_record_ref rcp_json_parse_number(
		const char** begin, const char* end);

//rcp_extern int rcp_json_parse_number(const char** begin, const char* end,
//		struct rcp_json_number *result);

rcp_extern void rcp_json_write_record(
		rcp_record_ref rec, rcp_string_ref out);
rcp_extern void rcp_json_write_map(rcp_map_ref rec, rcp_string_ref out);
rcp_extern void rcp_json_write_array(rcp_array_ref rec, rcp_string_ref out);
rcp_extern void rcp_json_write_string(
		rcp_string_ref rec, rcp_string_ref out);
