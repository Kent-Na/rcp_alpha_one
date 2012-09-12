#include "rcp_pch.h"
#include "rcp_utility.h"
#include "def/rcp_io.h"

#include "types/rcp_string.h"
#include "types/rcp_array.h"
#include "types/rcp_type_list.h"
#include "rcp_record.h"
#include "rcp_receiver.h"
#include "rcp_sender_classes.h"
#include "rcp_receiver_classes.h"
#include "rcp_connection.h"
#include "connections/con_pgsql_lo.h"
#include "rcp_server.h"
#include "rcp_user.h"

#define RCP_INTERNAL_STRUCTURE
#include "rcp_context.h"

void rcp_context_create_db_info(
		rcp_context_ref ctx, 
		rcp_string_ref name, rcp_context_ref new_ctx)
{
	PGconn* con = rcp_db_connection();

	if (! con){
		rcp_error("connection to database");
		return;
	}

	const char* sql_template = 
		"INSERT INTO contexts (name, parent_id)"
		"VALUES ($1::text,$2::bigint) RETURNING id;";

	const size_t param_num = 2;
	const void* values[param_num];
	int lengths[param_num];
	int binary[param_num];

	values[0] = rcp_string_c_str(name);
	values[1] = &ctx->id;	

	lengths[0] = rcp_string_c_str_len(name);
	lengths[1] = sizeof ctx->id;
	
	binary[0] = 0;
	binary[1] = 1;

	PGresult* result = PQexecParams(
			con,
			sql_template,
			param_num,
			NULL,
			(const char*const*)values,
			lengths,
			binary,
			1);

	void* val = PQgetvalue(result, 0, 0);
	rcp_context_id_t id = *(rcp_context_id_t*)val;
	new_ctx->id = id;

	PQclear(result);
	return;
}

void rcp_context_load_sub_contexts(rcp_context_ref ctx)
{
	if (ctx->state & RCP_CTX_PIRTIALY_LOADED)
		return;
	rcp_context_id_t id = ctx->id;

	PGconn* con = rcp_db_connection();

	if (! con){
		rcp_error("connection to database");
		return;
	}

	const char* sql_template = 
		"SELECT name,id FROM contexts WHERE parent_id = $1::bigint;";
	const size_t param_num = 1;
	void* values[param_num];
	int lengths[param_num];
	int binary[param_num];

	values[0] = &id;
	lengths[0] = sizeof id;
	binary[0] = 1;

	PGresult* result = PQexecParams(
			con,
			sql_template,
			param_num,
			NULL,
			(const char*const*)values,
			lengths,
			binary,
			1);

	int i;
	for (i = 0; i<PQntuples(result); i++){
		void* val;
		val = PQgetvalue(result, i, 0);
		rcp_string_ref name = rcp_string_new(val);

		val = PQgetvalue(result, i, 1);
		rcp_context_id_t id = *(rcp_context_id_t*)val;

		rcp_context_ref new_ctx = rcp_context_new();
		new_ctx->id = id;
		rcp_context_add_context(ctx, name, new_ctx);

		rcp_string_delete(name);
	}

	PQclear(result);
	rcp_context_set_state_flag(ctx, RCP_CTX_PIRTIALY_LOADED);
	return;	
}

Oid rcp_context_assign_new_contents_oid(rcp_context_ref ctx)
{
	rcp_context_id_t id = ctx->id;

	PGconn* con = rcp_db_connection();

	if (! con){
		rcp_error("connection to database");
		return InvalidOid;
	}

	Oid oid = lo_creat(con, INV_READ|INV_WRITE);
	uint32_t be_oid = htobe32(oid);

	const char* sql_template = 
		"UPDATE contexts SET contents = $1::oid WHERE id = $2::bigint;";
	const size_t param_num = 2;
	const void* values[param_num];
	int lengths[param_num];
	int binary[param_num];

	values[0] = &be_oid;
	lengths[0] = sizeof be_oid;
	binary[0] = 1;

	values[1] = &id;
	lengths[1] = sizeof id;
	binary[1] = 1;

	PGresult* result = PQexecParams(
			con,
			sql_template,
			param_num,
			NULL,
			(const char*const*)values,
			lengths,
			binary,
			1);

	PQclear(result);

	return oid;	
}

Oid rcp_context_contents_oid(rcp_context_ref ctx)
{
	rcp_context_id_t id = ctx->id;

	PGconn* con = rcp_db_connection();

	if (! con){
		rcp_error("connection to database");
		return InvalidOid;
	}

	const char* sql_template = 
		"SELECT contents FROM contexts WHERE id = $1::bigint;";
	const size_t param_num = 1;
	void* values[param_num];
	int lengths[param_num];
	int binary[param_num];

	values[0] = &id;
	lengths[0] = sizeof id;
	binary[0] = 1;

	PGresult* result = PQexecParams(
			con,
			sql_template,
			param_num,
			NULL,
			(const char*const*)values,
			lengths,
			binary,
			1);

	if (PQntuples(result) != 1){
		rcp_error("context record is not found");
		PQclear(result);
		return InvalidOid;
	}

	void* val = PQgetvalue(result, 0, 0);
	Oid oid = be32toh(*(Oid*)val);
	PQclear(result);


	return oid;	
}

void rcp_context_page_out(rcp_context_ref ctx)
{
	if (! (ctx->state & RCP_CTX_FULLY_LOADED))
		return;

	rcp_connection_ref con = rcp_connection_new();

	PGconn* db_con = rcp_db_connection();
	Oid oid = rcp_context_contents_oid(ctx);
	if (oid == InvalidOid)
		oid = rcp_context_assign_new_contents_oid(ctx);
	if (oid == InvalidOid)
		return;
	PQclear(PQexec(db_con, "begin"));
	rcp_io_ref io = con_pgsql_lo_new(db_con, oid);

	rcp_connection_set_io(con, io);
	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_l1_ref sender = rcp_sender_cluster_json_nt(cls);
	rcp_connection_set_sender(con, sender);

	rcp_context_send_all_data(ctx, con);
	rcp_context_send_all_permission(ctx, con);

	rcp_connection_delete(con);
	PQclear(PQexec(db_con, "end"));
}

void rcp_context_page_in(rcp_context_ref ctx)
{
	if (ctx->state & RCP_CTX_FULLY_LOADED)
		return;

	rcp_connection_ref con = rcp_connection_new();

	PGconn* db_con = rcp_db_connection();
	Oid oid = rcp_context_contents_oid(ctx);
	if (oid == InvalidOid)
		return;
	//Oid oid = lo_creat(db_con, INV_READ|INV_WRITE);
	PQclear(PQexec(db_con, "begin"));
	rcp_io_ref io = con_pgsql_lo_new(db_con, oid);

	rcp_connection_set_io(con, io);
	rcp_receiver_ref receiver = rcp_receiver_new(&con_nt_json_class);
	rcp_connection_set_receiver(con, receiver);
	rcp_connection_set_context(con, ctx);
	rcp_record_ref protocol_ver = rcp_string_new_rec("alpha1");
	rcp_connection_open(con, protocol_ver, NULL);
	rcp_record_release(protocol_ver);
	rcp_connection_set_permission(con, RCP_PMS_WRITE|RCP_PMS_PMS);

	while (rcp_connection_alive(con)){
		rcp_connection_on_receive(con);
	}
	rcp_connection_delete(con);

	PQclear(PQexec(db_con, "end"));

	rcp_context_set_state_flag(ctx, RCP_CTX_FULLY_LOADED);
}
