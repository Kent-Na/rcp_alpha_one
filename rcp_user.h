#include "def/rcp_string.h"

struct rcp_user_record{
	char* username;
	char* salt;
	char* password_hash;
};

void rcp_user_record_init(struct rcp_user_record *user);
void rcp_user_record_copy_username(
		struct rcp_user_record *user,
		const char* username);
void rcp_user_record_copy_salt(
		struct rcp_user_record *user,
		const char* salt);
void rcp_user_record_copy_password_hash(
		struct rcp_user_record *user,
		const char* password_hash);
void rcp_user_record_deinit(struct rcp_user_record *user);

struct rcp_user_info{
	rcp_string_ref username;
	uint64_t id;
};

int rcp_user_create(const char* username, const char* password);
int rcp_user_exist(const char* username);
int rcp_user_autenticate(const char* username, const char* password);

///
//db specific
//

void rcp_user_store(struct rcp_user_record *u_rec);

//u_rec->username is NULL when user not found.
void rcp_user_load(const char* username, struct rcp_user_record *u_rec);

#define RCP_PMS_LOGIN	(1<<0)
#define RCP_PMS_READ	(1<<1)
#define RCP_PMS_WRITE	(1<<2)
#define RCP_PMS_PMS		(1<<3)
