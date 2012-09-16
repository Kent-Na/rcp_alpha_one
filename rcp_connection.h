
#include "def/rcp_io.h"
#include "def/rcp_sender_l1.h"
#include "def/rcp_receiver.h"
#include "def/rcp_connection.h"

#include "def/rcp_context.h"
#include "def/rcp_record.h"

#include "def/rcp_type.h"
#include "def/rcp_data.h"

#include "def/rcp_permission.h"

//rcp_connections

rcp_extern
rcp_connection_ref rcp_connection_new();

rcp_extern
rcp_connection_ref rcp_connection_retain(rcp_connection_ref con);
rcp_extern
void rcp_connection_release(rcp_connection_ref con);

///
//io set
rcp_extern
void rcp_connection_set_io(
		rcp_connection_ref con, rcp_io_ref io);
rcp_extern
void rcp_connection_set_sender(
		rcp_connection_ref con, rcp_sender_l1_ref sender);
rcp_extern
void rcp_connection_set_receiver(
		rcp_connection_ref con, rcp_receiver_ref receiver);

///
//io operation
rcp_extern
void rcp_connection_send(rcp_connection_ref con);
rcp_extern
void rcp_connection_send_data(rcp_connection_ref con,
		rcp_type_ref type, rcp_data_ref data);
rcp_extern
void rcp_connection_on_receive(rcp_connection_ref con);
rcp_extern
int rcp_connection_alive(rcp_connection_ref con);
rcp_extern
void rcp_connection_on_close(rcp_connection_ref con);

rcp_extern 
void rcp_connection_close(rcp_connection_ref con);
rcp_extern 
void rcp_connection_open(rcp_connection_ref con,
		rcp_record_ref protocol, rcp_record_ref client);
rcp_extern 
int rcp_connection_is_open(rcp_connection_ref con); 

///
//info
rcp_extern
rcp_permission_t rcp_connection_permission(rcp_connection_ref con);

rcp_extern
void rcp_connection_set_permission(
		rcp_connection_ref con, rcp_permission_t pms);

rcp_extern
rcp_context_ref rcp_connection_context(
		rcp_connection_ref con);
rcp_extern
void rcp_connection_set_username(
		rcp_connection_ref con, rcp_record_ref username);
rcp_extern
rcp_record_ref rcp_connection_username(
		rcp_connection_ref con);
