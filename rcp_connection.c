#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"

#include "rcp_connection.h"
#include "rcp_io.h"
#include "rcp_sender.h"
#include "rcp_receiver.h"
#include "rcp_context.h"

#include "rcp_server.h"
#include "rcp_sender_classes.h"

#include "rcp_record.h"

struct rcp_connection_core{
	rcp_io_ref io;
	rcp_sender_l1_ref sender;
	rcp_receiver_ref receiver;

	//statuses
	rcp_record_ref username;
	uint32_t login_id;
	rcp_context_ref ctx;

	uint32_t permission;
};

rcp_connection_ref rcp_connection_new()
{
	rcp_connection_ref con = malloc(sizeof *con);	
	con->io = NULL;
	con->sender = NULL;
	con->receiver = NULL;

	con->username = NULL;
	con->login_id = 0;
	con->ctx = NULL;
	return con;
}
void rcp_connection_delete(rcp_connection_ref con)
{
	if (!con)
		return;
	rcp_io_delete(con->io);
	rcp_receiver_delete(con->receiver);

	rcp_record_release(con->username);
}
void rcp_connection_set_io(
		rcp_connection_ref con, rcp_io_ref io)
{
#ifdef RCP_SELF_TEST
	if (con->io){
		rcp_error("connection have already io");
		return;
	}
#endif
	con->io = io;
}
void rcp_connection_set_sender(
		rcp_connection_ref con, rcp_sender_l1_ref sender)
{
#ifdef RCP_SELF_TEST
	if (con->sender){
		rcp_error("connection have already sender");
		return;
	}
#endif
	con->sender = sender;
}
void rcp_connection_set_receiver(
		rcp_connection_ref con, rcp_receiver_ref receiver)
{
#ifdef RCP_SELF_TEST
	if (con->receiver){
		rcp_error("connection have already receiver");
		return;
	}
#endif
	con->receiver = receiver;
}
void rcp_connection_send(rcp_connection_ref con)
{
	const uint8_t *begin, *end;
	rcp_sender_result(con->sender, &begin, &end);
	rcp_io_send(con->io, begin, end-begin);
}
void rcp_connection_send_rec(rcp_connection_ref con, rcp_record_ref rec)
{
	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_cluster_set_rec(cls, rec);
	rcp_connection_send(con);
	rcp_sender_cluster_clean_up(cls);
}

extern void rcp_context_remove_connection(
		rcp_context_ref ctx, rcp_connection_ref con);

void rcp_connection_on_receive(rcp_connection_ref con)
{
	rcp_receiver_on_receive(con->receiver, con->io);
	rcp_record_ref rec = rcp_receiver_next_command(con->receiver);
	while (rec){
		rcp_context_execute_command_rec(con->ctx, con, rec);
		rec = rcp_receiver_next_command(con->receiver);
	}

	if (con->ctx && !rcp_io_alive(con->io))
		rcp_context_remove_connection(con->ctx, con);
}
int rcp_connection_alive(rcp_connection_ref con)
{
	return rcp_io_alive(con->io);
}
void rcp_connection_on_close(rcp_connection_ref con)
{
	rcp_io_on_close(con->io);
}

rcp_extern
void rcp_connection_set_context(
		rcp_connection_ref con, rcp_context_ref ctx)
{
	rcp_assert(con->ctx == NULL,"double ctx login");
	con->ctx = ctx;
}

rcp_extern
void rcp_connection_set_username(
		rcp_connection_ref con, rcp_record_ref username)
{
	rcp_assert(con->username == NULL,"double login");
	con->username = username;
	rcp_record_retain(username);
}
rcp_extern
rcp_record_ref rcp_connection_username(
		rcp_connection_ref con)
{
	return con->username;
}
