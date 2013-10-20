#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "../types/rcp_json.h"
#include "../types/rcp_type_list.h"
#include "../types/rcp_record.h"


int test_json(){
	{
		const char *in = "300";
		rcp_record_ref out;
		out = rcp_json_parse_number(&in, in+3);
		if (rcp_record_type(out) != rcp_int64_type)
			rcp_error("test:err 012");
		if (*(uint64_t*)rcp_record_data(out) != 300)
			rcp_error("test:err 013");
	}

	{
		const char *in = "-300";
		rcp_record_ref out;
		out = rcp_json_parse_number(&in, in+4);
		if (rcp_record_type(out) != rcp_int64_type)
			rcp_error("test:err 022");
		if (*(int64_t*)rcp_record_data(out) != -300)
			rcp_error("test:err 023");
	}

	/*
	{
		struct rcp_json_number num;
		const char *str = "012";
		const char *ptr = str;
		if (rcp_json_parse_number(&ptr, str+3, &num) != 0){
			rcp_error("test:err 012");
		}
		if (ptr != str + 1){
			rcp_error("test:err 012");
		}
	}

	{
		struct rcp_json_number num;
		const char *str = "1234";
		const char *ptr = str;
		if (rcp_json_parse_number(&ptr, str+4, &num) != 0){
			rcp_error("test:err 1234");
		}
		if (ptr != str + 4){
			rcp_error("test:err 1234");
		}
		if (num.significand != 1234){
			rcp_error("test:err 1234");
		}
		if (num.exponential_part != 0){
			rcp_error("test:err 1234");
		}
	}

	{
		struct rcp_json_number num;
		const char *str = "-5678";
		const char *ptr = str;
		if (rcp_json_parse_number(&ptr, str+5, &num) != 0){
			rcp_error("test:err -5678");
		}
		if (ptr != str + 5){
			rcp_error("test:err -5678");
		}
		if (num.significand != -5678){
			rcp_error("test:err -5678");
		}
		if (num.exponential_part != 0){
			rcp_error("test:err -5678");
		}
	}

	{
		struct rcp_json_number num;
		const char *str = "90e00012433";
		const char *ptr = str;
		if (rcp_json_parse_number(&ptr, str+11, &num) != 0){
			rcp_error("test:err 90e00012433");
		}
		if (ptr != str + 11){
			rcp_error("test:err 90e00012433");
		}
		if (num.significand!= 90){
			rcp_error("test:err 90e00012433");
		}
		if (num.exponential_part != 12433){
			rcp_error("test:err 90e00012433");
		}
	}

	{
		struct rcp_json_number num;
		const char *str = "-342.00124e+243";
		const char *ptr = str;
		if (rcp_json_parse_number(&ptr, str+15, &num) != 0){
			rcp_error("test:err -342.00124e+243");
		}
		if (ptr != str + 15){
			rcp_error("test:err -342.00124e+243");
		}
		if (num.significand!= -34200124){
			rcp_error("test:err -342.00124e+243");
		}
		if (num.exponential_part != 243-5){
			rcp_error("test:err -342.00124e+243");
		}
	}

	{
		const char *str = "\"test\"";
		const char *ptr = str;
		const char *end = str + strlen(str);
		const char *ans = "test";
		const char *out = NULL;
		if (rcp_json_parse_string(&ptr, end+1, &out) != 0){
			rcp_error("test:err");
		}
	}
	{
		const char *str = "\"teststr \\t\\\"\\\\ ??\"";
		const char *end = str + strlen(str);
		const char *ans = "teststr \t\"\\ ??";
		const char *out = NULL;
		if (rcp_json_parse_string(&str, end+1, &out) != 0){
			rcp_error("test:err");
		}
	}
	*/
	rcp_info("json none");
	return 0;
}
