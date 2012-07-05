#include "def/rcp_sender_cluster.h"
#include "def/rcp_sender_l1.h"
#include "def/rcp_record.h"

rcp_sender_cluster_ref rcp_sender_cluster_new();
rcp_sender_l1_ref rcp_sender_cluster_json_nt(
		rcp_sender_cluster_ref cluster);
rcp_sender_l1_ref rcp_sender_cluster_json_ws(
		rcp_sender_cluster_ref cluster);
void rcp_sender_cluster_set_rec(rcp_sender_cluster_ref cluster,
		rcp_record_ref rec);
void rcp_sender_cluster_clean_up(rcp_sender_cluster_ref cluster);

