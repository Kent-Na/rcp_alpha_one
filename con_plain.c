#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"

//for set fd
#include "rcp_epoll.h"
#include "rcp_connection.h"
#include "rcp_connection_builder.h"

#include "con_plain.h"

void con_plain_init(rcp_connection_ref con){
	struct con_plain *st = rcp_connection_l1(con);
	st->unit = NULL;
	st->fd = -1;
}

void con_plain_set_fd(int epfd, int fd, rcp_connection_ref con)
{
	struct con_plain *st = rcp_connection_l1(con);
	st->unit = malloc(sizeof (struct rcp_epoll_action));
	st->unit->action = rcp_connection_epoll_action;	
	st->unit->userdata.ptr = con;

	st->fd = fd;

	struct epoll_event ev;
	ev.events = EPOLLIN|EPOLLPRI|EPOLLRDHUP|EPOLLERR|EPOLLHUP;
	ev.data.ptr = st->unit;
	int err = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

void con_plain_release(rcp_connection_ref con)
{
	struct con_plain *st = rcp_connection_l1(con);
	int fd = st->fd;
	close(fd);
	free(st->unit);
}

size_t con_plain_send(rcp_connection_ref con, const void *data, size_t len)
{
	struct con_plain *st= rcp_connection_l1(con);
	int fd = st->fd;
	if (fd == -1)
		rcp_error("No connection");
	
	ssize_t r_len;
#ifdef MSG_NOSIGNAL
	r_len = send(fd, data, len, MSG_NOSIGNAL);
#else
	r_len = write(fd, data, len);
#endif

	if (r_len <= 0)
		rcp_error("Connection closed");

	return r_len;
}

size_t con_plain_receive(
		rcp_connection_ref con, const void *data, size_t len)
{
	struct con_plain *st = rcp_connection_l1(con);
	int fd = st->fd;
	if (fd == -1)
		rcp_error("No connection");

	ssize_t r_len;
	r_len = read(fd, data, len);

	if (r_len <= 0){
		rcp_info("Connection closed");
		close(fd);
		fd = -1;
	}

	return r_len;
}
