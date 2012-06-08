#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_connection.h"


void rcp_connection_core_init(
		void *state, struct rcp_connection_class* klass)
{
	struct rcp_connection_core *st = state;
	st->klass = klass;
	st->loginID = 0;
	st->userID = 0;
}

struct rcp_connection_class *rcp_connection_class(rcp_connection_ref con)
{
	struct rcp_connection_core *core = (void*)con;
	struct rcp_connection_class *klass = core->klass;
	return klass;
}

void *rcp_connection_l1(rcp_connection_ref con)
{
	size_t l1_data_offset = sizeof (struct rcp_connection_core);
	return con + l1_data_offset;
}
void *rcp_connection_l2(rcp_connection_ref con)
{
	struct rcp_connection_core *core = (void*)con;
	struct rcp_connection_class *klass = core->klass;
	size_t l1_data_offset = sizeof (struct rcp_connection_core);
	size_t l2_data_offset = l1_data_offset + klass->layer1_data_size;
	return con + l2_data_offset;
}
void *rcp_connection_l3(rcp_connection_ref con)
{
	struct rcp_connection_core *core = (void*)con;
	struct rcp_connection_class *klass = core->klass;
	size_t l1_data_offset = sizeof (struct rcp_connection_core);
	size_t l2_data_offset = l1_data_offset + klass->layer1_data_size;
	size_t l3_data_offset = l2_data_offset + klass->layer2_data_size;
	return con + l3_data_offset;
}
rcp_connection_ref rcp_connection_new(struct rcp_connection_class* klass)
{
	size_t l1_data_offset = sizeof (struct rcp_connection_core);
	size_t l2_data_offset = l1_data_offset + klass->layer1_data_size;
	size_t l3_data_offset = l2_data_offset + klass->layer2_data_size;
	size_t total_size = l3_data_offset + klass->layer3_data_size;

	rcp_connection_ref con = malloc(total_size);

	struct rcp_connection_core *core = (void*)con;

	rcp_connection_core_init(core, klass);
	if (klass ->l1.init)
		klass->l1.init(con);
	if (klass ->l2.init)
		klass->l2.init(con);
	if (klass ->l3.init)
		klass->l3.init(con);

	return (rcp_connection_ref) con;
};

void rcp_connection_free(rcp_connection_ref con)
{
	struct rcp_connection_core *core = (void*)con;
	struct rcp_connection_class *klass = core->klass;

	if (klass ->l1.release)
		klass->l1.release(con);
	if (klass ->l2.release)
		klass->l2.release(con);
	if (klass ->l3.release)
		klass->l3.release(con);
	free(con);
}

void rcp_connection_send(rcp_connection_ref con, void *data, size_t len)
{
	struct rcp_connection_core *core = (void*)con;
	struct rcp_connection_class *klass = core->klass;
	klass->l2.send(con, data, len);
}

void rcp_connection_on_receive(rcp_connection_ref con)
{
	struct rcp_connection_core *core = (void*)con;
	struct rcp_connection_class *klass = core->klass;

	klass->l2.on_receive(con);

	void *cmd_begin;
	void *cmd_end;
	rcp_err err;
	while (1){
		err = klass->l2.next_command(con, &cmd_begin, &cmd_end);
		if (err)
			break;
		klass->l3.execute_command(con, cmd_begin, cmd_end);
	}

	klass->l2.clean_space(con);
}

