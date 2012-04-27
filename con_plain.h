
struct con_plain{
	int fd;
	struct rcp_epoll_action *unit;
};

void con_plain_init(void *state);
void con_plain_release(void *state);
ssize_t con_plain_send(void *state, void *data, size_t len);
ssize_t con_plain_receive(void *state, void *data, size_t len);

#define CON_PLAIN_CLASS_PART \
{\
	con_plain_init,\
	con_plain_release,\
	con_plain_send,\
	con_plain_receive\
}

