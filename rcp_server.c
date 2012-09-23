#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#include "rcp_connection.h"
#include "rcp_listener.h"
#include "rcp_context.h"
#include "rcp_sender_classes.h"
#include "rcp_server.h"
#include "rcp_timeout.h"

#include "rcp_type.h"
#include "types/rcp_type_list.h"
#include "types/rcp_dict.h"
#include "types/rcp_dict_list.h"
#include "types/rcp_number.h"

///
//context
//

rcp_context_ref root_context = NULL;

rcp_context_ref rcp_root_context()
{
	if (root_context)
		return root_context;
	root_context = rcp_context_new();
	rcp_context_load_from_file(root_context);
	rcp_page_in_r(root_context);
	return root_context;
}

///
//listener
//
rcp_event_action_ref plain_json = NULL;
rcp_event_action_ref plain_ws_json = NULL;
rcp_event_action_ref plain_wss_json = NULL;
void rcp_listen_start(int epfd)
{
	if (plain_json)
		rcp_error("already listening");
	else
		plain_json = rcp_listener_plain_json_new(epfd);

	if (plain_ws_json)
		rcp_error("already listening");
	else
		plain_ws_json = rcp_listener_ws_json_new(epfd);

	if (plain_wss_json)
		rcp_error("already listening");
	else
		plain_wss_json = rcp_listener_wss_json_new(epfd);
}

void rcp_listen_end()
{
	if (!plain_json)
		rcp_error("not yet listening");
	else
		rcp_listener_delete(plain_json);

	if (!plain_ws_json)
		rcp_error("not yet listening");
	else
		rcp_listener_delete(plain_ws_json);

	if (!plain_wss_json)
		rcp_error("not yet listening");
	else
		rcp_listener_delete(plain_wss_json);
}

rcp_open_timeout_ref open_timeout = NULL;
rcp_extern rcp_open_timeout_ref rcp_open_timeout()
{
	return open_timeout;
}
rcp_extern void rcp_open_timeout_setup(int epfd)
{
	open_timeout = rcp_timeout_setup_timer(epfd);
}

///
//shared sender
//

static rcp_sender_cluster_ref cluster = NULL;

void rcp_shared_sender_cluster_init()
{
	cluster = rcp_sender_cluster_new();
}
rcp_sender_cluster_ref rcp_shared_sender_cluster()
{
	return cluster;
}

///
//database
//

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
	if (!con)
		rcp_error("db connect fail");
}

void rcp_db_disconnect()
{
	PQfinish(con);
}

PGconn* rcp_db_connection()
{
	return con;
}
