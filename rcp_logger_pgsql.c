#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_user.h"
#include "rcp_server.h"

void rcp_log_access(struct sockaddr *address)
{

	char buffer[128];
	if (address->sa_family == AF_INET){
		struct sockaddr_in *addr_in = (void*)address;
		inet_ntop(addr_in->sin_family,
				&addr_in->sin_addr.s_addr,
				buffer, sizeof buffer);
	}
	else if (address->sa_family == AF_INET6){
		struct sockaddr_in6 *addr_in = (void*)address;
		inet_ntop(addr_in->sin6_family,
				&addr_in->sin6_addr.s6_addr,
				buffer, sizeof buffer);
	}
	else{
		rcp_error("unknown family");
		return;
	}

	PGconn* con = rcp_db_connection();

	if (! con){
		rcp_error("connection to database");
		return;
	}

	const char* sql_template = 
		"INSERT INTO access_log (address)"
		"VALUES ($1::inet);";
	const size_t param_num = 1;
	void* values[param_num];
	int lengths[param_num];
	int binary[param_num];

	values[0] = buffer;
	lengths[0] = strlen(buffer);
	binary[0] = 0;

	PGresult* result = PQexecParams(
			con,
			sql_template,
			param_num,
			NULL,
			(const char*const*)values,
			lengths,
			binary,
			0);
	PQclear(result);
	return;
}
