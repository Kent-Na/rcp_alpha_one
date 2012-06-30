#include "../rcp_pch.h"
#include "../rcp_defines.h"
#include "../rcp_utility.h"
#include "../rcp_io.h"

//for set fd
#include "../rcp_epoll.h"

#include "con_plain.h"

struct rcp_io_class con_plain_class = {
	sizeof (struct con_plain),
	con_plain_init,
	con_plain_release,
	con_plain_send,
	con_plain_receive,
	con_plain_alive,
	con_plain_on_close,
};

void con_plain_init(rcp_io_ref io){
	struct con_plain *st = rcp_io_data(io);
	st->unit = NULL;
	st->fd = -1;
}

void con_plain_release(rcp_io_ref io)
{
	struct con_plain *st = rcp_io_data(io);
	int fd = st->fd;
	close(fd);
	st->fd = -1;
	free(st->unit);
}

size_t con_plain_send(rcp_io_ref io, const void *data, size_t len)
{
	struct con_plain *st= rcp_io_data(io);
	int fd = st->fd;
	if (fd == -1)
		rcp_error("No connection");
	
	ssize_t r_len;
#ifdef MSG_NOSIGNAL
	r_len = send(fd, data, len, MSG_NOSIGNAL);
#else
	r_len = write(fd, data, len);
#endif

	if (r_len <= 0){
		rcp_error("Connection closed");
		close(fd);
		st->fd = -1;
		if (con_plain_alive(io))
			rcp_error("zombie conn");
		return 0;
	}

	return r_len;
}

size_t con_plain_receive(
		rcp_io_ref io, void *data, size_t len)
{
	struct con_plain *st = rcp_io_data(io);
	int fd = st->fd;
	if (fd == -1)
		rcp_error("No connection");

	ssize_t r_len;
	r_len = read(fd, data, len);

	if (r_len <= 0){
		rcp_info("Connection closed");
		close(fd);
		st->fd = -1;
		if (con_plain_alive(io))
			rcp_error("zombie conn");
		return 0;
	}

	return r_len;
}

int con_plain_alive(
		rcp_io_ref io)
{
	struct con_plain *st = rcp_io_data(io);
	int fd = st->fd;
	return fd != -1;	
}

void con_plain_on_close(
		rcp_io_ref io)
{
	struct con_plain *st = rcp_io_data(io);
	st->fd = -1;
}

void con_plain_set_fd(rcp_io_ref io, struct rcp_epoll_action *unit,
		int epfd, int fd)
{
	struct con_plain *st = rcp_io_data(io);
	st->unit = unit;
	st->fd = fd;

	struct epoll_event ev;
	ev.events = EPOLLIN|EPOLLPRI|EPOLLRDHUP|EPOLLERR|EPOLLHUP;
	ev.data.ptr = st->unit;
	int err = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
	if (err)
		rcp_error("epoll ctl");
}
