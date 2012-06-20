#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"
#include "rcp_type.h"
#include "rcp_sender.h"

#define CMP_DONE (1)
#define CMP_NOT_YET (2)

struct rcp_sender_l0_core{
	struct rcp_sender_l0_class *klass;
	struct rcp_sender_terget *tgt;
	uint32_t state;
};

struct rcp_sender_l1_core{
	struct rcp_sender_l1_class *klass;
	rcp_sender_l0_ref l0;
	uint32_t state;
};

rcp_sender_l0_ref rcp_sender_l0_new(
		struct rcp_sender_l0_class *klass,
		struct rcp_sender_terget *tgt)
{
	rcp_sender_l0_ref l0;
	l0 = malloc(sizeof *l0 + klass->size);
	l0->klass = klass;
	l0->tgt = tgt; 
	l0->state = CMP_NOT_YET;
	l0->klass->init(l0 + 1);
	return l0;
}

void rcp_sender_l0_clean_up(rcp_sender_l0_ref l0)
{
	l0->state = CMP_NOT_YET;
	l0->klass->clean_up(l0 + 1);
}
rcp_sender_l1_ref rcp_sender_l1_new(
		struct rcp_sender_l1_class *klass,
		rcp_sender_l0_ref l0)
{
	rcp_sender_l1_ref l1;
	l1 = malloc(sizeof *l1 + klass->size);
	l1->klass = klass;
	l1->l0 = l0; 
	l1->state = CMP_NOT_YET;
	l1->klass->init(l1 + 1);
	return l1;
}

void rcp_sender_l1_clean_up(rcp_sender_l1_ref l1)
{
	l1->state = CMP_NOT_YET;
	l1->klass->clean_up(l1 + 1);
}
void rcp_sender_result(rcp_sender_ref cmp, 
		const uint8_t **begin, const uint8_t **end)
{
	struct rcp_sender_l0_core *l0 = cmp->l0;
	struct rcp_sender_l1_core *l1 = cmp;
	if (l0->state != CMP_DONE){
		l0->klass->build(l0+1, l0->tgt->rec);
		l0->state = CMP_DONE;
	}
	if (l1->state != CMP_DONE){
		const uint8_t *l0_begin, *l0_end;
		l0->klass->result(l0+1, &l0_begin, &l0_end);
		l1->klass->build(l1+1, l0_begin, l0_end);
		l1->state = CMP_DONE;
	}
	l1->klass->result(l1+1, begin, end);
}
