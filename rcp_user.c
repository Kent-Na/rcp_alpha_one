#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "types/rcp_string.h"

#include "rcp_user.h"

void rcp_user_record_init(struct rcp_user_record *user)
{
	user->username = NULL;
	user->salt = NULL;
	user->password_hash = NULL;
}
void rcp_user_record_copy_username(
		struct rcp_user_record *user,
		const char* username)
{
	if (user->username)
		free(user->username);
	size_t len = strlen(username) + 1;
	user->username = malloc(len);
	memcpy(user->username, username, len);
}
void rcp_user_record_copy_salt(
		struct rcp_user_record *user,
		const char* salt)
{
	if (user->salt)
		free(user->salt);
	size_t len = strlen(salt) + 1;
	user->salt = malloc(len);
	memcpy(user->salt, salt, len);
}
void rcp_user_record_copy_password_hash(
		struct rcp_user_record *user,
		const char* password_hash)
{
	if (user->password_hash)
		free(user->password_hash);
	size_t len = strlen(password_hash) + 1;
	user->password_hash= malloc(len);
	memcpy(user->password_hash, password_hash, len);
}
void rcp_user_record_deinit(struct rcp_user_record *user)
{
	if (user->username)
		free(user->username);
	if (user->salt)
		free(user->salt);
	if (user->password_hash)
		free(user->password_hash);
}


#define RCP_USER_SALT_LEN (16)

//salt length = 16 byte
void rcp_user_salt_gen(char* out)
{
	uint8_t r_num[24];
	RAND_bytes(r_num, 24);
	char* b64 = rcp_encode_base64(r_num, 24);
	int i;
	for (i = 0; i<RCP_USER_SALT_LEN ; i++)
		out[i] = b64[i];
	free(b64);
}

void rcp_user_hash_gen(
		const char* password, 
		const char* user_salt, 
		uint8_t* out)
{
	size_t password_len = strlen(password);
	size_t s_salt_len = strlen(RCP_SERVER_SALT);
	size_t len = password_len+RCP_USER_SALT_LEN+s_salt_len;
	uint8_t cat_str[len+1];

	uint8_t *ptr = cat_str;
	memcpy(ptr, password, password_len);

	ptr += password_len;
	memcpy(ptr, user_salt, RCP_USER_SALT_LEN);

	ptr += RCP_USER_SALT_LEN;
	memcpy(ptr, RCP_SERVER_SALT, s_salt_len);

	SHA1(cat_str, len, out);
}

void rcp_user_hash_str_gen(
		uint8_t* hash,
		uint8_t* out){

	const char* hex_table = "0123456789abcdef";
	int i;
	for (i= 0; i<20; i++){
		out[i*2] = hex_table[hash[i]&0x0f];
		out[i*2+1] = hex_table[(hash[i]>>4)&0x0f];
	}
	out[40] = '\0';
}
int rcp_user_create(const char* username, const char* password)
{
	char user_salt[17];
	rcp_user_salt_gen(user_salt);
	user_salt[16]='\0';

	char p_hash[20];
	rcp_user_hash_gen(password, user_salt, p_hash);
	char p_hash_str[41];
	rcp_user_hash_str_gen(p_hash, p_hash_str);

	struct rcp_user_record u_rec;
	rcp_user_record_init(&u_rec);
	
	rcp_user_load(username, &u_rec);
	if (u_rec.username){
		rcp_user_record_deinit(&u_rec);
		return -1;
	}

	rcp_user_record_copy_username(&u_rec, username);
	rcp_user_record_copy_salt(&u_rec, user_salt);
	rcp_user_record_copy_password_hash(&u_rec, p_hash_str);

	rcp_user_store(&u_rec);

	rcp_user_record_deinit(&u_rec);
	return 0;
}

int rcp_user_exist(const char* username)
{
	struct rcp_user_record u_rec;
	rcp_user_record_init(&u_rec);
	
	rcp_user_load(username, &u_rec);
	if (u_rec.username){
		rcp_user_record_deinit(&u_rec);
		return 1;
	}
	return 0;
}

int rcp_user_autenticate(const char* username, const char* password)
{
	struct rcp_user_record u_rec;
	rcp_user_record_init(&u_rec);
	
	rcp_user_load(username, &u_rec);
	if (! u_rec.username){
		return 0;
	}

	char p_hash[20];
	rcp_user_hash_gen(password, u_rec.salt, p_hash);
	char p_hash_str[41];
	rcp_user_hash_str_gen(p_hash, p_hash_str);

	if (strcmp(p_hash_str, u_rec.password_hash) == 0){
		rcp_user_record_deinit(&u_rec);
		return 1;
	}
	rcp_user_record_deinit(&u_rec);
	return 0;
}
