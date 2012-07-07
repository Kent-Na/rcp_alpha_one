#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_type.h"
#include "rcp_record.h"
#include "rcp_io.h"
#include "rcp_receiver.h"


void rcp_receiver_core_init(
		void *state, struct rcp_receiver_class* klass)
{
	struct rcp_receiver_core *st = state;
	st->klass = klass;
}

struct rcp_receiver_class *rcp_receiver_class(rcp_receiver_ref con)
{
	struct rcp_receiver_core *core = (void*)con;
	struct rcp_receiver_class *klass = core->klass;
	return klass;
}

void *rcp_receiver_l2(rcp_receiver_ref con)
{
	return con + 1;
}
void *rcp_receiver_l3(rcp_receiver_ref con)
{
	struct rcp_receiver_core *core = (void*)con;
	struct rcp_receiver_class *klass = core->klass;
	size_t l2_data_offset = sizeof (struct rcp_receiver_core);
	size_t l3_data_offset = l2_data_offset + klass->layer2_data_size;
	return (void*)con + l3_data_offset;
}
rcp_receiver_ref rcp_receiver_new(struct rcp_receiver_class* klass)
{
	size_t l2_data_offset = sizeof (struct rcp_receiver_core);
	size_t l3_data_offset = l2_data_offset + klass->layer2_data_size;
	size_t total_size = l3_data_offset + klass->layer3_data_size;

	rcp_receiver_ref con = malloc(total_size);

	struct rcp_receiver_core *core = (void*)con;

	rcp_receiver_core_init(core, klass);
	if (klass ->l2.init)
		klass->l2.init(con);
	if (klass ->l3.init)
		klass->l3.init(con);

	return (rcp_receiver_ref) con;
};

void rcp_receiver_delete(rcp_receiver_ref con)
{
	if (!con)
		return;
	struct rcp_receiver_core *core = (void*)con;
	struct rcp_receiver_class *klass = core->klass;

	if (klass ->l2.release)
		klass->l2.release(con);
	if (klass ->l3.release)
		klass->l3.release(con);
	free(con);
}

void rcp_receiver_on_receive(rcp_receiver_ref con, rcp_io_ref io)
{
	struct rcp_receiver_core *core = (void*)con;
	struct rcp_receiver_class *klass = core->klass;

	klass->l2.on_receive(con, io);
}

rcp_record_ref rcp_receiver_next_command(rcp_receiver_ref con)
{
	struct rcp_receiver_core *core = (void*)con;
	struct rcp_receiver_class *klass = core->klass;


	void *cmd_begin;
	void *cmd_end;
	rcp_err err;
	while (1){
		err = klass->l2.next_command(con, &cmd_begin, &cmd_end);
		if (err){
			klass->l2.clean_space(con);
			return NULL;
		}
		rcp_record_ref rec = 
			klass->l3.execute_command(con, cmd_begin, cmd_end);
		if (rec)
			return rec;
	}

}
