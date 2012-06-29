
struct rcp_user_info{
	rcp_string_ref username;
	uint64_t id;
};

int rcp_user_create(const char* username, const char* password);
int rcp_user_autenticate(const char* username, const char* password);

#define RCP_PMS_READ
#define RCP_PMS_WRITE
#define RCP_PMS_PMS
