#include "../rcp_pch.h"

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

	{
		const char *in= "012";
        const char *ptr = in;
		rcp_record_ref out;
		out = rcp_json_parse_number(&ptr, ptr+4);

		if (rcp_record_type(out) != rcp_int64_type)
			rcp_error("test:err 032");
		if (*(int64_t*)rcp_record_data(out) != 12)
			rcp_error("test:err 033");
        if (ptr != in+4)
			rcp_error("test:err 034");
	}

	{
		const char *str = "90e40";
		const char *ptr = str;
		rcp_record_ref out;
		out = rcp_json_parse_number(&ptr, ptr+6);

		if (rcp_record_type(out) != rcp_double_type)
			rcp_error("test:err 042");
		if (*(double*)rcp_record_data(out) != 90e40)
			rcp_error("test:err 043");
        if (ptr != str+11)
			rcp_error("test:err 044");
	}
/*
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
    */

	{
		const char *str = "\"test\"";
		const char *ptr = str;
		const char *end = str + strlen(str);
        rcp_string_ref expected = rcp_string_new("test");

        rcp_record_ref out;
        out = rcp_json_parse_string(&ptr, end+1);

		if (rcp_record_type(out) != rcp_string_type)
			rcp_error("test:err 062");
		if (rcp_compare(rcp_string_type, 
                rcp_record_data(out), (rcp_data_ref)expected) != 0)
			rcp_error("test:err 063");
	}
	{
		const char *str = "\"teststr \\t\\\"\\\\ ??\"";
		const char *end = str + strlen(str);
        rcp_string_ref expected = rcp_string_new("teststr \t\"\\ ??");
        rcp_record_ref out;
        out = rcp_json_parse_string(&str, end+1);

		if (rcp_record_type(out) != rcp_string_type)
			rcp_error("test:err 072");
		if (rcp_compare(rcp_string_type, 
                rcp_record_data(out), (rcp_data_ref)expected) != 0)
			rcp_error("test:err 073");
	}

	rcp_info("json none");
	return 0;
}
