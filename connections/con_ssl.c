#include "../rcp_pch.h"
#include "../rcp_defines.h"
#include "../rcp_utility.h"
#include "../rcp_io.h"
#include "../rcp_event.h"

#include "con_ssl.h"
#include "../rcp_ssl.h"

struct rcp_io_class con_ssl_class = {
	sizeof (struct con_ssl),
	con_ssl_init,
	con_ssl_release,
	con_ssl_send,
	con_ssl_receive,
	con_ssl_alive,
	con_ssl_on_close,
};

void con_ssl_init(rcp_io_ref io){
	struct con_ssl *st = rcp_io_data(io);
	st->unit = NULL;
	st->fd = -1;
	st->bio = NULL;
	st->ssl = NULL;
}

void con_ssl_set_fd(rcp_io_ref io, rcp_event_action_ref unit,
		int epfd, int fd)
{
	struct con_ssl *st = rcp_io_data(io);
	st->unit = unit;

	st->fd = -1;
	st->bio = NULL;
	st->ssl = NULL;

	int err;
	st->fd = fd;
	//accept(listenSocket, NULL, NULL);
	st->bio = BIO_new_socket(st->fd, BIO_NOCLOSE);
	if (st->bio == NULL){
		printf("e3\n");
	}
	st->ssl = SSL_new(rcp_ssl_ctx());
	SSL_set_bio(st->ssl,st->bio,st->bio);
	if (st->ssl == NULL){
		printf("e4\n");
	}
	err = SSL_accept(st->ssl);
	if (err != 1){
		err = SSL_get_error(st->ssl,err);
		printf("e6 %i\n",err);
		printf("%s\n",strerror(errno));
	}

	rcp_event_add_fd(epfd, fd, unit);
}

void con_ssl_release(rcp_io_ref io)
{
	struct con_ssl *st = rcp_io_data(io);
	int fd = st->fd;
	SSL_free(st->ssl);
	BIO_free(st->bio);
	close(fd);
	st->fd = -1;
	free(st->unit);
}

size_t con_ssl_send(rcp_io_ref io, const void *data, size_t len)
{
	struct con_ssl *st= rcp_io_data(io);
	int fd = st->fd;
	if (fd == -1)
		rcp_error("No connection");
	
	ssize_t r_len;
	r_len = SSL_write(st->ssl, data, len);

	if (r_len <= 0){
		rcp_error("Connection closed");
		close(fd);
		st->fd = -1;
		if (con_ssl_alive(io))
			rcp_error("zombe conn");
		return 0;
	}

	return r_len;
}

size_t con_ssl_receive(
		rcp_io_ref io, void *data, size_t len)
{
	struct con_ssl *st = rcp_io_data(io);
	int fd = st->fd;
	if (fd == -1)
		rcp_error("No connection");

	ssize_t r_len;
	r_len = SSL_read(st->ssl, (void*)data, len);

	if (r_len <= 0){
		rcp_info("Connection closed");
		close(fd);
		st->fd = -1;
		if (con_ssl_alive(io))
			rcp_error("zombe conn");
		return 0;
	}

	return r_len;
}

int con_ssl_alive(
		rcp_io_ref io)
{
	struct con_ssl *st = rcp_io_data(io);
	int fd = st->fd;
	return fd != -1;	
}
void con_ssl_on_close(rcp_io_ref io)
{
	return;
}
