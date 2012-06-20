
typedef struct rcp_connection_core *rcp_connection_ref;

struct rcp_connection_core{
	rcp_io_ref io;
	rcp_sender_ref sender;
	rcp_receiver_ref receiver;

	//user data
	uint32_t loginID;
	uint32_t userID;
};

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
		rcp_connection_ref con, rcp_sender_ref sender);
rcp_extern
void rcp_connection_set_receiver(
		rcp_connection_ref con, rcp_receiver_ref receiver);

rcp_extern
void rcp_connection_send(rcp_connection_ref con);
rcp_extern
void rcp_connection_send_rec(rcp_connection_ref con, rcp_receiver_ref rec);
rcp_extern
void rcp_connection_on_receive(rcp_connection_ref con);
rcp_extern
int rcp_connection_alive(rcp_connection_ref con);
rcp_extern
void rcp_connection_on_close(rcp_connection_ref con);
