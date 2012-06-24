#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_type.h"
#include "rcp_sender.h"
#include "rcp_sender_classes.h"

#include "rcp_io.h"
#include "rcp_receiver.h"
#include "rcp_buffer.h"

#include "con_null_terminate.h"
#include "con_web_socket.h"
#include "con_json.h"


struct rcp_sender_cluster{
	struct rcp_sender_terget *tgt;

	struct rcp_sender_l0_core *json;

	struct rcp_sender_l1_core *json_nt;
	struct rcp_sender_l1_core *json_ws;
};

rcp_sender_cluster_ref rcp_sender_cluster_new()
{
	rcp_sender_cluster_ref cluster = malloc(sizeof *cluster);
	cluster->tgt = malloc(sizeof *cluster->tgt);
	cluster->tgt->rec = NULL;

	cluster->json = rcp_sender_l0_new(&cmp_json_class, cluster->tgt);

	cluster->json_nt = rcp_sender_l1_new(
			&cmp_null_terminate_class, cluster->json);
		
	cluster->json_ws = rcp_sender_l1_new(
			&cmp_web_socket_class, cluster->json);
	return cluster;
}

rcp_sender_ref rcp_sender_cluster_json_nt(rcp_sender_cluster_ref cluster)
{
	return cluster->json_nt;
}

rcp_sender_ref rcp_sender_cluster_json_ws(rcp_sender_cluster_ref cluster)
{
	return cluster->json_ws;
}
void rcp_sender_cluster_set_rec(rcp_sender_cluster_ref cluster,
		rcp_record_ref rec)
{
	rcp_assert(cluster->tgt->rec == NULL, "didn't clean up");
	cluster->tgt->rec = rec;
}
void rcp_sender_cluster_clean_up(rcp_sender_cluster_ref cluster)
{
	cluster->tgt->rec = NULL;

	rcp_sender_l0_clean_up(cluster->json);
	rcp_sender_l1_clean_up(cluster->json_nt);
	rcp_sender_l1_clean_up(cluster->json_ws);
}
