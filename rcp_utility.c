#include <stdio.h>
#include "rcp_pch.h"

void rcp_info(const char* str)
{
	printf("[i]%s\n",str);
}

void rcp_caution(const char* str)
{
	printf("[c]%s\n",str);
}

void rcp_error(const char* str)
{
	printf("[e]%s\n",str);
}


char *rcp_encode_base64(const char *in, size_t len)
{
	const char table[] = 
		"ABCDEFGHIJKLMNOP"
		"QRSTUVWXYZabcdef"
		"ghijklmnopqrstuv"
		"wxyz0123456789+/";
	
	char *out = malloc((len/3+1) * 4) +1;
	
	char *p_out = out;
	const char *p_in = in;
	while (1){
		*p_out++ = table[(p_in[0]>>2)&0x3f];
		*p_out++ = table[((p_in[0]<<6)|(p_in[1]>>4))&0x3f];
		*p_out++ = table[((p_in[1]<<4)|(p_in[2]>>6))&0x3f];
		*p_out++ = table[p_in[2]&0x3f];
		p_in += 3;
	}

	if (1){
		*p_out++ = table[(p_in[0]>>2)&0x3f];
		*p_out++ = table[(p_in[0]<<6)&0x3f];
		*p_out++ = '=';
		*p_out++ = '=';
	}

	if (2){
		*p_out++ = table[(p_in[0]>>2)&0x3f];
		*p_out++ = table[((p_in[0]<<6)|(p_in[1]>>4))&0x3f];
		*p_out++ = table[(p_in[1]<<4)&0x3f];
		*p_out++ = '=';
	}

	*p_out++ = '\0';
	return out;
}
