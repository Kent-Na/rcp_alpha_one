#include "../rcp_pch.h"
#include "../rcp_defines.h"
#include "../rcp_utility.h"
#include "../rcp_io.h"

//for set fd
#include "../rcp_event.h"

#include "con_pgsql_lo.h"

struct rcp_io_class con_pgsql_lo_class = {
	sizeof (struct con_pgsql_lo),
	con_pgsql_lo_init,
	con_pgsql_lo_release,
	con_pgsql_lo_send,
	con_pgsql_lo_receive,
	con_pgsql_lo_alive,
	con_pgsql_lo_on_close,
};

void con_pgsql_lo_init(rcp_io_ref io){
	struct con_pgsql_lo *st = rcp_io_data(io);
	st->oid = InvalidOid;
	st->con = NULL;
	st->fd = -1;
}

rcp_io_ref con_pgsql_lo_new(PGconn* con, Oid oid)
{
	rcp_io_ref io = rcp_io_new(&con_pgsql_lo_class);
	struct con_pgsql_lo *st = rcp_io_data(io);

	st->con = con;
	st->oid = oid;
	st->fd = lo_open(st->con, st->oid, INV_READ|INV_WRITE);

	return io;
}

void con_pgsql_lo_release(rcp_io_ref io)
{
	struct con_pgsql_lo *st = rcp_io_data(io);
	if (st->fd != -1)
		lo_close(st->con, st->fd);
}

size_t con_pgsql_lo_send(rcp_io_ref io, const void *data, size_t len)
{
	struct con_pgsql_lo *st= rcp_io_data(io);
	if (st->fd == -1)
		rcp_error("No connection");
	
	int r_len;
	r_len = lo_write(st->con, st->fd, data, len);

	if (r_len <= 0){
		rcp_error("Connection closed");
		lo_close(st->con, st->fd);
		st->fd = -1;
		if (con_pgsql_lo_alive(io))
			rcp_error("zombie conn");
		return 0;
	}

	return r_len;
}

size_t con_pgsql_lo_receive(
		rcp_io_ref io, void *data, size_t len)
{
	struct con_pgsql_lo *st = rcp_io_data(io);
	if (st->fd == -1)
		rcp_error("No connection");

	int r_len;
	r_len = lo_read(st->con, st->fd, data, len);

	if (r_len <= 0){
		rcp_info("Connection closed");
		lo_close(st->con, st->fd);
		st->fd = -1;
		if (con_pgsql_lo_alive(io))
			rcp_error("zombie conn");
		return 0;
	}

	return r_len;
}

int con_pgsql_lo_alive(
		rcp_io_ref io)
{
	struct con_pgsql_lo *st = rcp_io_data(io);
	int fd = st->fd;
	return fd != -1;	
}

void con_pgsql_lo_on_close(
		rcp_io_ref io)
{
	struct con_pgsql_lo *st = rcp_io_data(io);
	lo_close(st->con, st->fd);
	st->fd = -1;
}
