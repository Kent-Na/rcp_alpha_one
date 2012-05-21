#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_tree.h"
#include "rcp_epoll.h"
#include "rcp_connection.h"
#include "rcp_string.h"
#include "rcp_struct.h"
#include "rcp_context.h"

struct rcp_context_core{
	//rcp_tree_ref structs;
	rcp_tree_ref records;
	rcp_tree_ref connections;
};

size_t rcp_context_size = sizeof (struct rcp_context_core);

void rcp_context_init(rcp_context_ref ctx){
	ctx->records = rcp_tree_new(rcp_type_uint32.compare);
	ctx->connections = rcp_tree_new(rcp_type_pointer.compare);
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
