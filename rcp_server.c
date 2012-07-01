#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"
#include "rcp_types.h"
#include "rcp_tree.h"
#include "rcp_event.h"
#include "rcp_io.h"
#include "rcp_sender.h"
#include "rcp_receiver.h"
#include "rcp_connection.h"
#include "rcp_listener.h"
#include "rcp_context.h"
#include "rcp_sender_classes.h"
#include "rcp_server.h"

///
//context
//

rcp_tree_ref contexts = NULL;

void rcp_context_manager_init()
{
	contexts = rcp_tree_new(rcp_uint32_type_compare, NULL);
}

rcp_context_ref rcp_context_get(rcp_context_id_t id)
{
	rcp_tree_node_ref node = rcp_tree_find(contexts, &id);
	if (node == NULL)
		return NULL;
	uint32_t *key = rcp_tree_node_data(node);
	rcp_context_ref ctx = (void*)(key + 1);
	return ctx;
}

rcp_context_ref rcp_context_new(uint32_t id)
{
	rcp_tree_node_ref node = rcp_tree_node_new(
			sizeof (uint32_t) + rcp_context_size);

	uint32_t *key = rcp_tree_node_data(node);
	*key = id;
	rcp_context_ref ctx = (void*)(key + 1);
	rcp_context_init(ctx);
	rcp_tree_add(contexts, node);
	rcp_context_page_in(ctx);
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

rcp_sender_cluster_ref cluster = NULL;

void rcp_shared_sender_cluster_init()
{
	cluster = rcp_sender_cluster_new();
}
rcp_sender_cluster_ref rcp_shared_sender_cluster()
{
	return cluster;
}
