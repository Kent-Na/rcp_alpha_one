#include "def/rcp_record.h"
#include "def/rcp_string.h"

//type table
//json		<->		rcp

//object	<->		dict(string, ref) 
//array		<->		array(ref)
//string	<->		string
//number	<->		double or int64
//bool		<->		bool8
//null		<->		null

//object	<--		struct

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
