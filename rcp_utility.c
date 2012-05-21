#include <stdio.h>


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
