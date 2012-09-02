
struct con_plain{
	int fd;
	rcp_event_action_ref unit;
};

void con_plain_init(rcp_io_ref io);
void con_plain_release(rcp_io_ref io);
size_t con_plain_send(
		rcp_io_ref io, const void *data, size_t len);
size_t con_plain_receive(
		rcp_io_ref io, void *data, size_t len);
void con_plain_close(
		rcp_io_ref io);
int con_plain_alive(rcp_io_ref io);
void con_plain_on_close(
		rcp_io_ref io);

void con_plain_set_fd(rcp_io_ref io, rcp_event_action_ref unit,
		int epfd, int fd);

extern struct rcp_io_class con_plain_class;
