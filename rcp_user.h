#include "def/rcp_string.h"
#include "def/rcp_permission.h"

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


typedef uint64_t rcp_permission_t;

#define RCP_PMS_LOGIN	(1<<0)
#define RCP_PMS_READ	(1<<1)
#define RCP_PMS_WRITE	(1<<2)
//To give and rob permission someone.
#define RCP_PMS_PMS		(1<<3)
//To kill or summon sub context.
#define RCP_PMS_CTX		(1<<4)

#define RCP_PMS_STR_LOGIN	("login")
#define RCP_PMS_STR_READ	("read")
#define RCP_PMS_STR_WRITE	("write")
#define RCP_PMS_STR_PMS		("permission")
#define RCP_PMS_STR_CTX		("context")

rcp_extern
rcp_permission_t rcp_permission_from_array(rcp_record_ref rec);

rcp_extern
rcp_record_ref rcp_permission_to_array(rcp_permission_t);
