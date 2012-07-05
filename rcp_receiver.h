#include "def/rcp_receiver.h"
#include "def/rcp_record.h"


//Layer 1
//plain socket or ssl socket
//
//close,send,receive
//
//hold basic connection state
//ie. userid, loginid ...

struct rcp_receiver_core{
	struct rcp_receiver_class *klass;
};

//layer 2 (command separation)
//raw or web socket or null terminated

struct rcp_receiver_layer2_class_part{
	void (*init)(rcp_receiver_ref con);
	void (*release)(rcp_receiver_ref con);

	void (*on_receive)(rcp_receiver_ref con, rcp_io_ref io);
	rcp_err (*next_command)(
			rcp_receiver_ref con, void **cmd_begin, void **cmd_end);
	void (*clean_space)(rcp_receiver_ref con);
};

//layer 3 ()
//json or binaly
//
//execute command

struct rcp_receiver_layer3_class_part{
	void (*init)(void *state);
	void (*release)(void* state);
	rcp_record_ref (*execute_command)(rcp_receiver_ref con, 
			const char *begin, const char *end);
	//rcp_receiver_execute_command execute_command;
};

struct rcp_receiver_class{
	size_t layer2_data_size;
	struct rcp_receiver_layer2_class_part l2;
	size_t layer3_data_size;
	struct rcp_receiver_layer3_class_part l3;
};


//rcp_receivers

rcp_extern
rcp_receiver_ref rcp_receiver_new(struct rcp_receiver_class* klass);
rcp_extern
void rcp_receiver_delete(rcp_receiver_ref con);
rcp_extern
void rcp_receiver_on_receive(rcp_receiver_ref con, rcp_io_ref io);
rcp_record_ref rcp_receiver_next_command(rcp_receiver_ref con);

struct rcp_receiver_class *rcp_receiver_class(rcp_receiver_ref con);

void *rcp_receiver_l2(rcp_receiver_ref con);
void *rcp_receiver_l3(rcp_receiver_ref con);

