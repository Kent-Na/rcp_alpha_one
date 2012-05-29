
//Layer 1
//plain socket or ssl socket
//
//close,send,receive
//
//hold basic connection state
//ie. userid, loginid ...

typedef int rcp_err;

typedef void *rcp_connection_ref;

struct rcp_connection_layer1_class_part{
	void (*init)(void *state);
	void (*release)(void* state);
	ssize_t (*send)(void* state, void *data, size_t len);
	ssize_t (*receive)(void* state, void *data, size_t len);
};

//layer 2 (command separation)
//raw or web socket or null terminated

struct rcp_connection_layer2_class_part{
	void (*init)(void *state);
	void (*release)(void* state);
	//sending
	void* (*generate_header)(void *state, size_t len);	
	size_t (*last_header_size)(const void *state);
	void* (*generate_footer)(void *state);	
	size_t (*last_footer_size)(const void *state);
	//receiving
	size_t (*space_len)(const void *state);
	void* (*space_ptr)(const void *state);
	void (*data_added)(void *state, size_t len);
	rcp_err (*next_command)(void *state, void **cmd_begin, void **cmd_end);
	void (*clean_space)(void *state);
};

//layer 3 ()
//json or binaly
//
//execute command

struct rcp_connection_layer3_class_part{
	void (*init)(void *state);
	void (*release)(void* state);
	void (*execute_command)(rcp_connection_ref con, 
			const char *begin, const char *end);
	//rcp_connection_execute_command execute_command;
};

struct rcp_connection_class{
	size_t layer1_data_size;
	struct rcp_connection_layer1_class_part l1;
	size_t layer2_data_size;
	struct rcp_connection_layer2_class_part l2;
	size_t layer3_data_size;
	struct rcp_connection_layer3_class_part l3;
};


//rcp_connections

rcp_extern
rcp_connection_ref rcp_connection_new(struct rcp_connection_class* klass);
rcp_extern
void rcp_connection_free(rcp_connection_ref con);
rcp_extern
void rcp_connection_send(rcp_connection_ref con, void *data, size_t len);
rcp_extern
void rcp_connection_on_receive(rcp_connection_ref con);

//epoll
void rcp_connection_epoll_action(int epfd, struct epoll_event *ev,
		epoll_data_t data);
rcp_connection_ref rcp_connection_plain_json_new(int epfd, int fd);
rcp_epoll_action_ref rcp_listener_plain_json_new(int epfd);
void rcp_listener_plain_json_release(rcp_epoll_action_ref unit);
void rcp_listener_plain_json_epoll_event(
		int epfd, struct epoll_event *ev, epoll_data_t userdata);
