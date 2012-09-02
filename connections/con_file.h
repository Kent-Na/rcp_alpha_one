
struct con_file{
	int fd;
};

void con_file_init(rcp_io_ref io);
void con_file_release(rcp_io_ref io);
size_t con_file_send(
		rcp_io_ref io, const void *data, size_t len);
size_t con_file_receive(
		rcp_io_ref io, void *data, size_t len);
void con_file_close(
		rcp_io_ref io);
int con_file_alive(rcp_io_ref io);
void con_file_on_close(
		rcp_io_ref io);

rcp_io_ref con_file_io_new_rd(const char* path);
rcp_io_ref con_file_io_new_wr(const char* path);

extern struct rcp_io_class con_file_class;
