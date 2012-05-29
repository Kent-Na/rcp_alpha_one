#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_type.h"
#include "rcp_type_number.h"
#include "rcp_type_etc.h"
#include "rcp_string.h"
#include "rcp_tree.h"
#include "rcp_map.h"
#include "rcp_epoll.h"
#include "rcp_connection.h"
#include "rcp_context.h"

struct rcp_context_core{
	rcp_record_ref top_level_record;
	rcp_tree_ref connections;
};

size_t rcp_context_size = sizeof (struct rcp_context_core);

void rcp_context_init(rcp_context_ref ctx){
	ctx->top_level_record = 
		(rcp_record_ref)rcp_map_new(&rcp_uint32_type, &rcp_ref_type);
	ctx->connections = rcp_tree_new(rcp_pointer_type.compare);
	rcp_map_ref map = ((rcp_map_ref)(ctx->top_level_record));

	{
		rcp_map_node_ref node = rcp_map_node_new(map);
		rcp_string_ref v = rcp_string_new("test");
		rcp_move(&rcp_ref_type, &v, rcp_map_node_value(map, node));
		uint32_t *k = rcp_map_node_key(map, node);
		*k = 1;
		rcp_map_set(map, node);
	}
}
void rcp_context_uninit(rcp_context_ref ctx){
	//ctx->top_level_record
	rcp_tree_delete(ctx->connections);
}

rcp_record_ref rcp_context_top_level_record(rcp_context_ref ctx){
	return ctx->top_level_record;
}

void rcp_context_add_connection(rcp_context_ref ctx, 
		rcp_connection_ref con){
	rcp_tree_node_ref node= rcp_tree_node_new(sizeof con);
	*(rcp_connection_ref*)rcp_tree_node_data(node) = con;
	rcp_tree_add(ctx->connections, node);
}

void rcp_context_remove_connection(rcp_context_ref ctx,
		rcp_connection_ref con){
	rcp_tree_node_ref node = rcp_tree_find(ctx->connections, con);
	if (node)
		rcp_tree_remove(ctx->connections, node);
	else
		rcp_error("missing connection");
}

void rcp_context_execute_command(rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd);
