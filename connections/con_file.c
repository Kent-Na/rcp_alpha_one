#include "../rcp_pch.h"
#include "../rcp_defines.h"
#include "../rcp_io.h"

//for set fd
#include "../rcp_event.h"

#include "con_file.h"

#define CON_FILE_READ (1)
#define CON_FILE_WRITE (2)

struct con_file{
	int fd;
	int flag;
};

struct rcp_io_class con_file_class = {
	sizeof (struct con_file),
	con_file_init,
	con_file_release,
	con_file_send,
	con_file_receive,
	con_file_close,
	con_file_alive,
	con_file_on_close,
};

void con_file_init(rcp_io_ref io){
	struct con_file *st = rcp_io_data(io);
	st->fd = -1;
	st->flag = 0;
}

void con_file_release(rcp_io_ref io)
{
	rcp_io_close(io);
}

rcp_io_ref con_file_io_new_wr(const char* path)
{
	rcp_io_ref io = rcp_io_new(&con_file_class);
	struct con_file *st = rcp_io_data(io);
#ifdef __FreeBSD__
	st->fd = open(path, O_WRONLY | O_TRUNC | O_CREAT);
#else
	st->fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR|S_IWUSR);
#endif
	st->flag = CON_FILE_WRITE;
	return io;
}
rcp_io_ref con_file_io_new_rd(const char* path)
{
	rcp_io_ref io = rcp_io_new(&con_file_class);
	struct con_file *st = rcp_io_data(io);
	st->fd = open(path, O_RDONLY, 0);
	st->flag = CON_FILE_READ;
	return io;
}
size_t con_file_send(rcp_io_ref io, const void *data, size_t len)
{
	struct con_file *st= rcp_io_data(io);
	if (st->flag == CON_FILE_READ) return len;
	int fd = st->fd;
	if (fd == -1)
		rcp_error("No connection");
	
	ssize_t r_len;
	r_len = write(fd, data, len);

	if (r_len <= 0){
		rcp_io_close(io);
		return 0;
	}

	return r_len;
}

size_t con_file_receive(
		rcp_io_ref io, void *data, size_t len)
{
	struct con_file *st = rcp_io_data(io);
	if (st->flag == CON_FILE_WRITE) return len;

	int fd = st->fd;
	if (fd == -1)
		rcp_error("No connection");

	ssize_t r_len;
	r_len = read(fd, data, len);

	if (r_len <= 0){
		rcp_io_close(io);
		return 0;
	}

	return r_len;
}

void con_file_close(
		rcp_io_ref io)
{
	struct con_file *st = rcp_io_data(io);
	close(st->fd);
	st->fd = -1;
}
int con_file_alive(
		rcp_io_ref io)
{
	struct con_file *st = rcp_io_data(io);
	return st->fd != -1;	
}

void con_file_on_close(
		rcp_io_ref io)
{
	rcp_io_close(io);
}
