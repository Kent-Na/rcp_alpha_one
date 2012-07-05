#include "def/rcp_context.h"
#include "def/rcp_connection.h"
#include "def/rcp_record.h"

rcp_extern rcp_context_ref rcp_context_new();
rcp_extern void rcp_context_init(rcp_context_ref ctx);

//connection management
rcp_extern void rcp_context_add_connection(rcp_context_ref ctx, 
		rcp_connection_ref con);

rcp_extern void rcp_context_remove_connection(rcp_context_ref ctx,
		rcp_connection_ref con);

//data management
rcp_extern rcp_record_ref rcp_context_top_level_record(rcp_context_ref ctx);

rcp_extern void rcp_context_execute_command_rec(
		rcp_context_ref ctx,
		rcp_connection_ref con, rcp_record_ref cmd);

void rcp_context_page_in(rcp_context_ref ctx);
