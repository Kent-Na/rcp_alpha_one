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
	BIO *mem = BIO_new(BIO_s_mem());
	BIO *base64 = BIO_new(BIO_f_base64());
	base64 = BIO_push(base64, mem);
	BIO_write(base64, in, len);
	BIO_flush(base64);

	BUF_MEM *mem_ptr;
	BIO_get_mem_ptr(base64, &mem_ptr);
	
	char *out = malloc(mem_ptr->length+1);
	memcpy(out, mem_ptr->data, mem_ptr->length);
	out[mem_ptr->length-1] = '\0';
	rcp_info(out);

	BIO_free_all(base64);

	return out;
}
