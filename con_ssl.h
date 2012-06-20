
struct con_ssl{
	int fd;
	BIO* bio;
	SSL* ssl;
	struct rcp_epoll_action *unit;
};

void con_ssl_init(rcp_io_ref io);
void con_ssl_release(rcp_io_ref io);
void con_ssl_set_fd(rcp_io_ref io, struct rcp_epoll_action *unit,
		int epfd, int fd);
size_t con_ssl_send(
		rcp_io_ref io, const void *data, size_t len);
size_t con_ssl_receive(
		rcp_io_ref io, void *data, size_t len);
int con_ssl_alive(
		rcp_io_ref io);
void con_ssl_on_close(rcp_io_ref io);

extern struct rcp_io_class con_ssl_class;
