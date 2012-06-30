
struct con_plain{
	int fd;
	struct rcp_epoll_action *unit;
};

void con_plain_init(rcp_io_ref io);
void con_plain_release(rcp_io_ref io);
size_t con_plain_send(
		rcp_io_ref io, const void *data, size_t len);
size_t con_plain_receive(
		rcp_io_ref io, void *data, size_t len);
int con_plain_alive(rcp_io_ref io);
void con_plain_on_close(
		rcp_io_ref io);

void con_plain_set_fd(rcp_io_ref io, struct rcp_epoll_action *unit,
		int epfd, int fd);

extern struct rcp_io_class con_plain_class;