
rcp_extern size_t rcp_context_size;

typedef struct rcp_context_core* rcp_context_ref;

rcp_extern void rcp_context_init(rcp_context_ref ctx);

//connection management
rcp_extern void rcp_context_add_connection(rcp_context_ref ctx, 
		rcp_connection_ref con);

rcp_extern void rcp_context_remove_connection(rcp_context_ref ctx,
		rcp_connection_ref con);

//data management
rcp_extern rcp_record_ref rcp_context_top_level_record(rcp_context_ref ctx);

rcp_extern void rcp_context_execute_command_rec(
		rcp_connection_ref con, rcp_record_ref cmd);

void rcp_context_page_in(rcp_context_ref ctx);
//rcp_extern void rcp_context_command_set(rcp_context_ref ctx, )

//rcp_extern void rcp_context_execute_command(rcp_context_ref ctx,
//		rcp_connection_ref con,
//		rcp_record_ref cmd);
