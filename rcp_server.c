#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_type.h"
#include "rcp_type_number.h"
#include "rcp_tree.h"
#include "rcp_epoll.h"
#include "rcp_connection.h"
#include "rcp_context.h"
#include "rcp_server.h"

rcp_tree_ref contexts = NULL;

void rcp_context_manager_init()
{
	contexts = rcp_tree_new(rcp_uint32_type.compare);
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
	return ctx;
}


rcp_epoll_action_ref plain_json = NULL;
void rcp_listen_start(int epfd)
{
	if (plain_json)
		rcp_error("already listening");
	else
		plain_json = rcp_listener_plain_json_new(epfd);
}

void rcp_listen_end()
{
	if (!plain_json)
		rcp_error("not yet listening");
	else
		rcp_listener_plain_json_release(plain_json);
}
