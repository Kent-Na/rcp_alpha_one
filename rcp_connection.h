
//Layer 1
//plain socket or ssl socket
//
//close,send,receive
//
//hold basic connection state
//ie. userid, loginid ...

typedef int rcp_err;

typedef void *rcp_connection_ref;

struct rcp_connection_core{
	struct rcp_connection_class *klass;
	uint32_t loginID;
	uint32_t userID;
};

struct rcp_connection_layer1_class_part{
	void (*init)(
			rcp_connection_ref con);
	void (*release)(
			rcp_connection_ref con);
	size_t (*send)(
			rcp_connection_ref con, const void *data, size_t len);
	size_t (*receive)(
			rcp_connection_ref con, const void *data, size_t len);
	int (*alive)(
			rcp_connection_ref con);
};

//layer 2 (command separation)
//raw or web socket or null terminated

struct rcp_connection_layer2_class_part{
	void (*init)(rcp_connection_ref con);
	void (*release)(rcp_connection_ref con);

	//sending
	void (*send)(rcp_connection_ref con, void *data, size_t len);

	//receiving
	void (*on_receive)(rcp_connection_ref con);
	rcp_err (*next_command)(
			rcp_connection_ref con, void **cmd_begin, void **cmd_end);
	void (*clean_space)(rcp_connection_ref con);
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
rcp_extern
int rcp_connection_alive(rcp_connection_ref con);

struct rcp_connection_class *rcp_connection_class(rcp_connection_ref con);

void *rcp_connection_l1(rcp_connection_ref con);
void *rcp_connection_l2(rcp_connection_ref con);
void *rcp_connection_l3(rcp_connection_ref con);

