#include "../rcp_pch.h"
#include "../rcp_defines.h"
#include "../rcp_io.h"

//for set fd
#include "../rcp_event.h"

#include "con_pgsql_lo.h"

#define CON_PGSQL_LO_READ 1
#define CON_PGSQL_LO_WRITE 2

struct con_pgsql_lo{
	PGconn* con;
	Oid oid;
	int fd;
	int flag; //read or write
};

struct rcp_io_class con_pgsql_lo_class = {
	sizeof (struct con_pgsql_lo),
	con_pgsql_lo_init,
	con_pgsql_lo_release,
	con_pgsql_lo_send,
	con_pgsql_lo_receive,
	con_pgsql_lo_close,
	con_pgsql_lo_alive,
	con_pgsql_lo_on_close,
};

void con_pgsql_lo_init(rcp_io_ref io){
	struct con_pgsql_lo *st = rcp_io_data(io);
	st->oid = InvalidOid;
	st->con = NULL;
	st->fd = -1;
}

static
rcp_io_ref con_pgsql_lo_new_with_flag(PGconn* con, Oid oid, int flag)
{
	rcp_io_ref io = rcp_io_new(&con_pgsql_lo_class);
	struct con_pgsql_lo *st = rcp_io_data(io);

	st->con = con;
	st->oid = oid;
	PQclear(PQexec(con, "begin"));
	if (flag == CON_PGSQL_LO_READ)
		st->fd = lo_open(st->con, st->oid, INV_READ);
	if (flag == CON_PGSQL_LO_WRITE)
		st->fd = lo_open(st->con, st->oid, INV_WRITE);
	st->flag = flag;

	return io;
}

rcp_io_ref con_pgsql_lo_new_rd(PGconn* con, Oid oid)
{
	return con_pgsql_lo_new_with_flag(con, oid, CON_PGSQL_LO_READ);
}
rcp_io_ref con_pgsql_lo_new_wr(PGconn* con, Oid oid)
{
	return con_pgsql_lo_new_with_flag(con, oid, CON_PGSQL_LO_WRITE);
}

void con_pgsql_lo_release(rcp_io_ref io)
{
	rcp_io_close(io);
}

size_t con_pgsql_lo_send(rcp_io_ref io, const void *data, size_t len)
{
	struct con_pgsql_lo *st= rcp_io_data(io);
	if (st->flag==CON_PGSQL_LO_READ) return len;
	if (st->fd == -1)
		rcp_error("No connection");
	
	int r_len;
	r_len = lo_write(st->con, st->fd, data, len);

	if (r_len <= 0){
		rcp_io_close(io);
		return 0;
	}

	return r_len;
}

size_t con_pgsql_lo_receive(
		rcp_io_ref io, void *data, size_t len)
{
	struct con_pgsql_lo *st = rcp_io_data(io);
	if (st->flag==CON_PGSQL_LO_WRITE) return len;
	if (st->fd == -1)
		rcp_error("No connection");

	int r_len;
	r_len = lo_read(st->con, st->fd, data, len);

	if (r_len <= 0){
		rcp_io_close(io);
		return 0;
	}

	return r_len;
}

void con_pgsql_lo_close(
		rcp_io_ref io)
{
	struct con_pgsql_lo *st = rcp_io_data(io);
	if (st->fd != -1){
		lo_close(st->con, st->fd);
		PQclear(PQexec(st->con, "end"));
		st->fd = -1;
	}
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
	rcp_io_close(io);
}
