#include "rcp_pch.h"
#include "rcp_utility.h"
#include "def/rcp_io.h"

#include "types/rcp_array.h"
#include "types/rcp_type_list.h"
#include "rcp_record.h"
#include "rcp_receiver.h"
#include "rcp_sender_classes.h"
#include "rcp_receiver_classes.h"
#include "rcp_connection.h"
#include "connections/con_pgsql_lo.h"
#include "rcp_server.h"

#define RCP_INTERNAL_STRUCTURE
#include "rcp_context.h"

void rcp_context_load_structure(rcp_context_ref ctx)
{

}

void rcp_context_page_out(rcp_context_ref ctx)
{
	if (! (ctx->state & RCP_CTX_FULLY_LOADED))
		return;

	rcp_connection_ref con = rcp_connection_new();

	PGconn* db_con = rcp_db_connection();
	Oid oid = 16391;
		//lo_creat(db_con, INV_READ|INV_WRITE);
	PQclear(PQexec(db_con, "begin"));
	rcp_io_ref io = con_pgsql_lo_new(db_con, oid);

	rcp_connection_set_io(con, io);
	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_l1_ref sender = rcp_sender_cluster_json_nt(cls);
	rcp_connection_set_sender(con, sender);

	rcp_context_send_all_data(ctx, con);

	rcp_connection_delete(con);
	PQclear(PQexec(db_con, "end"));

	ctx->state &= ~RCP_CTX_FULLY_LOADED;
	rcp_record_release(ctx->top_level_record);
	ctx->top_level_record = NULL;
}

void rcp_context_page_in(rcp_context_ref ctx)
{
	if (ctx->state & RCP_CTX_FULLY_LOADED)
		return;

	ctx->top_level_record = 
		rcp_array_new_rec(rcp_ref_type);

	rcp_connection_ref con = rcp_connection_new();

	PGconn* db_con = rcp_db_connection();
	Oid oid = 16391;
	//Oid oid = lo_creat(db_con, INV_READ|INV_WRITE);
	PQclear(PQexec(db_con, "begin"));
	rcp_io_ref io = con_pgsql_lo_new(db_con, oid);

	rcp_connection_set_io(con, io);
	rcp_receiver_ref receiver = rcp_receiver_new(&con_nt_json_class);
	rcp_connection_set_receiver(con, receiver);
	rcp_connection_set_context(con, ctx);

	while (rcp_connection_alive(con)){
		rcp_connection_on_receive(con);
	}
	rcp_connection_delete(con);

	PQclear(PQexec(db_con, "end"));

	ctx->state |= RCP_CTX_FULLY_LOADED;
}
