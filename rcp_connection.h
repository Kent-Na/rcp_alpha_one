
#include "def/rcp_io.h"
#include "def/rcp_sender_l1.h"
#include "def/rcp_receiver.h"
#include "def/rcp_connection.h"

#include "def/rcp_context.h"
#include "def/rcp_record.h"

//rcp_connections

rcp_extern
rcp_connection_ref rcp_connection_new();
rcp_extern
void rcp_connection_delete(rcp_connection_ref con);

rcp_extern
void rcp_connection_set_io(
		rcp_connection_ref con, rcp_io_ref io);
rcp_extern
void rcp_connection_set_sender(
		rcp_connection_ref con, rcp_sender_l1_ref sender);
rcp_extern
void rcp_connection_set_receiver(
		rcp_connection_ref con, rcp_receiver_ref receiver);

rcp_extern
void rcp_connection_send(rcp_connection_ref con);
rcp_extern
void rcp_connection_on_receive(rcp_connection_ref con);
rcp_extern
int rcp_connection_alive(rcp_connection_ref con);
rcp_extern
void rcp_connection_on_close(rcp_connection_ref con);

rcp_extern
void rcp_connection_set_context(
		rcp_connection_ref con, rcp_context_ref ctx);
rcp_extern
rcp_context_ref rcp_connection_context(
		rcp_connection_ref con);
rcp_extern
void rcp_connection_set_username(
		rcp_connection_ref con, rcp_record_ref username);
rcp_extern
rcp_record_ref rcp_connection_username(
		rcp_connection_ref con);
