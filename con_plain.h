
struct con_plain{
	int fd;
	struct rcp_epoll_action *unit;
};

void con_plain_init(rcp_connection_ref con);
void con_plain_release(rcp_connection_ref con);
size_t con_plain_send(
		rcp_connection_ref con, const void *data, size_t len);
size_t con_plain_receive(
		rcp_connection_ref con, const void *data, size_t len);
int con_plain_alive(
		rcp_connection_ref con);

#define CON_PLAIN_CLASS_PART \
{\
	con_plain_init,\
	con_plain_release,\
	con_plain_send,\
	con_plain_receive,\
	con_plain_alive\
}

