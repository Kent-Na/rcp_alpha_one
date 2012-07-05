
#include "../def/rcp_io.h"
#include "../def/rcp_type.h"
#include "../def/rcp_receiver.h"

struct con_null_terminate{
	struct rcp_buffer buffer;
};

void con_null_terminate_init(rcp_receiver_ref con);
void con_null_terminate_deinit(rcp_receiver_ref con);

void con_null_terminate_on_receive(rcp_receiver_ref con, rcp_io_ref io);
rcp_err con_null_terminate_next_command(
		rcp_receiver_ref con, void **command_begin, void **command_end);
void con_null_terminate_clean_space(rcp_receiver_ref con);

#define CON_NULL_TERMINAE_CLASS_PART \
{\
	con_null_terminate_init,\
	con_null_terminate_deinit,\
	con_null_terminate_on_receive,\
	con_null_terminate_next_command,\
	con_null_terminate_clean_space\
}

extern struct rcp_sender_l1_class cmp_null_terminate_class;
