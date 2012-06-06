
struct con_buffer{
	//char buffer[RCP_PROTOCOL_JSON_BUFFER_SIZE];
	//allocated mem info
	void *front;
	void *back;
	//active data info
	void *begin;
	void *end;
};

void con_buffer_init(struct con_buffer *buffer, size_t len);

void con_buffer_deinit(struct con_buffer *buffer);

void con_buffer_verify(struct con_buffer *buffer);

void *con_buffer_data(struct con_buffer *buffer);

void *con_buffer_data_end(struct con_buffer *buffer);

size_t con_buffer_data_size(struct con_buffer *buffer);

void *con_buffer_space(struct con_buffer *buffer);

size_t con_buffer_space_size(struct con_buffer *buffer);

void *con_buffer_supplied(struct con_buffer *buffer, size_t len);

void *con_buffer_consume(struct con_buffer *buffer, size_t len);
void *con_buffer_consumed(struct con_buffer *buffer, size_t len);
void *con_buffer_consumed_at(struct con_buffer *buffer, void* ptr);

void con_buffer_cleanup(struct con_buffer *buffer);

