
struct rcp_user_info{
	rcp_string_ref username;
};

int rcp_user_create(const char* username, const char* password);
int rcp_user_autenticate(const char* username, const char* password);
