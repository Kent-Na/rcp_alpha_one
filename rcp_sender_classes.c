#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_type.h"
#include "rcp_record.h"
#include "rcp_sender.h"
#include "rcp_sender_classes.h"

#include "rcp_io.h"
#include "rcp_receiver.h"
#include "rcp_buffer.h"

#include "connections/con_null_terminate.h"
#include "connections/con_web_socket.h"
#include "connections/con_json.h"
#include "connections/con_dummy.h"


struct rcp_sender_cluster_core{
	struct rcp_sender_terget *tgt;

	struct rcp_sender_l0_core *json;
	struct rcp_sender_l0_core *dummy_l0;

	struct rcp_sender_l1_core *json_nt;
	struct rcp_sender_l1_core *json_ws;
	struct rcp_sender_l1_core *dummy_l1;
};

rcp_sender_cluster_ref rcp_sender_cluster_new()
{
	rcp_sender_cluster_ref cluster = malloc(sizeof *cluster);
	cluster->tgt = malloc(sizeof *cluster->tgt);
	cluster->tgt->data = NULL;
	cluster->tgt->type = NULL;

	cluster->json = rcp_sender_l0_new(&cmp_json_class, cluster->tgt);

	cluster->dummy_l0 = rcp_sender_l0_new(
			&cmp_dummy_l0_class, cluster->tgt);

	cluster->json_nt = rcp_sender_l1_new(
			&cmp_null_terminate_class, cluster->json);
		
	cluster->json_ws = rcp_sender_l1_new(
			&cmp_web_socket_class, cluster->json);

	cluster->dummy_l1 = rcp_sender_l1_new(
			&cmp_dummy_l1_class, cluster->dummy_l0);
	return cluster;
}

rcp_sender_l1_ref rcp_sender_cluster_json_nt(rcp_sender_cluster_ref cluster)
{
	return cluster->json_nt;
}

rcp_sender_l1_ref rcp_sender_cluster_json_ws(rcp_sender_cluster_ref cluster)
{
	return cluster->json_ws;
}

rcp_sender_l1_ref rcp_sender_cluster_dummy(rcp_sender_cluster_ref cluster)
{
	return cluster->dummy_l1;
}
void rcp_sender_cluster_set_data(rcp_sender_cluster_ref cluster,
		rcp_type_ref type, rcp_data_ref data)
{
	rcp_assert(cluster->tgt->data== NULL, "didn't clean up");
	cluster->tgt->type = type;
	cluster->tgt->data = data;
}
void rcp_sender_cluster_clean_up(rcp_sender_cluster_ref cluster)
{
	cluster->tgt->data = NULL;
	cluster->tgt->type = NULL;

	rcp_sender_l0_clean_up(cluster->json);
	rcp_sender_l0_clean_up(cluster->dummy_l0);
	rcp_sender_l1_clean_up(cluster->json_nt);
	rcp_sender_l1_clean_up(cluster->json_ws);
	rcp_sender_l1_clean_up(cluster->dummy_l1);
}
