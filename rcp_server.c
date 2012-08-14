#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#include "rcp_connection.h"
#include "rcp_listener.h"
#include "rcp_context.h"
#include "rcp_sender_classes.h"
#include "rcp_server.h"

#include "rcp_type.h"
#include "types/rcp_type_list.h"
#include "types/rcp_dict.h"
#include "types/rcp_dict_list.h"
#include "types/rcp_number.h"

///
//context
//

rcp_dict_ref contexts = NULL;

void rcp_context_manager_init()
{
	contexts = (rcp_dict_ref)rcp_new(rcp_uint32_ptr_dict); 
}

rcp_context_ref rcp_context_get(rcp_context_id_t id)
{
	rcp_dict_node_ref node = rcp_dict_find(contexts, (rcp_data_ref)&id);
	if (node == NULL)
		return NULL;
	return *(rcp_context_ref*)rcp_dict_node_data(rcp_uint32_ptr_dict, node);
}

rcp_context_ref rcp_context_create(uint32_t id)
{
	rcp_context_ref ctx = rcp_context_new();

	rcp_dict_node_ref node = rcp_dict_node_new(rcp_uint32_ptr_dict);
	*(uint32_t*)rcp_dict_node_key(rcp_uint32_ptr_dict, node) = id;
	*(rcp_context_ref*)rcp_dict_node_data(rcp_uint32_ptr_dict, node) = ctx;

	rcp_dict_set_node(contexts, node);
	return ctx;
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
