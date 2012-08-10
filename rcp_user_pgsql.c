#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "types/rcp_string.h"
#include "rcp_user.h"

#include <libpq-fe.h>

static PGconn* con = NULL;

void rcp_db_connect()
{
	const char* connectInfoKey[] = {
		"user",
		"password",
		"dbname",
		NULL
	};

	const char* connectInfoValue[] = {
		RCP_DB_USER,
		RCP_DB_PASSWORD,
		RCP_DB_NAME,
		NULL
	};

	con = PQconnectdbParams(connectInfoKey, connectInfoValue, 1);
}

void rcp_db_disconnect()
{
	PQfinish(con);
}

void rcp_user_store(struct rcp_user_record *u_rec)
{
	if (! con){
		rcp_error("connection to database");
		return;
	}

	const char* sql_template = 
		"INSERT INTO users (name, salt, password_hash)"
		"VALUES ($1::text,$2::text,$3::text);";
	const size_t param_num = 3;
	void* values[param_num];
	int lengths[param_num];
	int binary[param_num];

	values[0] = u_rec->username;
	values[1] = u_rec->salt;	
	values[2] = u_rec->password_hash;

	lengths[0] = strlen(u_rec->username);
	lengths[1] = strlen(u_rec->salt);
	lengths[2] = strlen(u_rec->password_hash);
	
	binary[0] = 0;
	binary[1] = 0;
	binary[2] = 0;

	PGresult* result = PQexecParams(
			con,
			sql_template,
			param_num,
			NULL,
			(const char*const*)values,
			lengths,
			binary,
			1);
	PQclear(result);
	return;
}

void rcp_user_load(const char* username, struct rcp_user_record *u_rec)
{
	if (! con){
		rcp_error("connection to database");
		return;
	}

	const char* sql_template = 
		"SELECT * FROM users WHERE name = $1::text;";
	const size_t param_num = 1;
	void* values[param_num];
	int lengths[param_num];
	int binary[param_num];

	values[0] = (void*)username;
	lengths[0] = strlen(username);
	binary[0] = 0;

	PGresult* result = PQexecParams(
			con,
			sql_template,
			param_num,
			NULL,
			(const char*const*)values,
			lengths,
			binary,
			1);
	if (PQntuples(result) == 0){
		PQclear(result);
		return;
	}
	if (PQntuples(result) > 1){
		PQclear(result);
		rcp_error("duplicative user record!!");
		return;
	}

	rcp_user_record_copy_username(u_rec, 
			PQgetvalue(result, 0, PQfnumber(result, "name")));
	rcp_user_record_copy_salt(u_rec, 
			PQgetvalue(result, 0, PQfnumber(result, "salt")));
	rcp_user_record_copy_password_hash(u_rec, 
			PQgetvalue(result, 0, PQfnumber(result, "password_hash")));

	PQclear(result);
	return;	
}
