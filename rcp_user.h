#include "def/rcp_string.h"

struct rcp_user_info{
	rcp_string_ref username;
	uint64_t id;
};

int rcp_user_create(const char* username, const char* password);
int rcp_user_exist(const char* username);
int rcp_user_autenticate(const char* username, const char* password);

#define RCP_PMS_LOGIN	(1<<0)
#define RCP_PMS_READ	(1<<1)
#define RCP_PMS_WRITE	(1<<2)
#define RCP_PMS_PMS		(1<<3)
