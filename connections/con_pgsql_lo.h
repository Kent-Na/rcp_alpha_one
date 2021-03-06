rcp_io_ref con_pgsql_lo_new_rd(PGconn* con, Oid oid);
rcp_io_ref con_pgsql_lo_new_wr(PGconn* con, Oid oid);
void con_pgsql_lo_init(rcp_io_ref io);
void con_pgsql_lo_release(rcp_io_ref io);
size_t con_pgsql_lo_send(
		rcp_io_ref io, const void *data, size_t len);
size_t con_pgsql_lo_receive(
		rcp_io_ref io, void *data, size_t len);
void con_pgsql_lo_close(rcp_io_ref io);
int con_pgsql_lo_alive(rcp_io_ref io);
void con_pgsql_lo_on_close(rcp_io_ref io);

extern struct rcp_io_class con_pgsql_lo_class;
