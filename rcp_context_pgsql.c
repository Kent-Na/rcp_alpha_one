#include "rcp_pch.h"

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
		PQclear(result);
		return InvalidOid;
	}

	void* val = PQgetvalue(result, 0, 0);
	Oid oid = be32toh(*(Oid*)val);
	PQclear(result);


	return oid;	
}

void rcp_context_store(rcp_context_ref ctx)
{
	PGconn* db_con = rcp_db_connection();
	Oid oid = rcp_context_contents_oid(ctx);
	if (oid == InvalidOid)
		oid = rcp_context_assign_new_contents_oid(ctx);
	if (oid == InvalidOid){
		rcp_error("no oid");
		return;
	}
	rcp_io_ref io = con_pgsql_lo_new_wr(db_con, oid);
	rcp_context_store_into_io(ctx, io);
}

void rcp_context_load(rcp_context_ref ctx)
{
	if (ctx->state & RCP_CTX_FULLY_LOADED)
		return;

	PGconn* db_con = rcp_db_connection();
	Oid oid = rcp_context_contents_oid(ctx);
	if (oid == InvalidOid)
		return;
	rcp_io_ref io = con_pgsql_lo_new_rd(db_con, oid);
	rcp_context_load_from_io(ctx, io);
}
