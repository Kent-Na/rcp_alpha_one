#include "def/rcp_sender_cluster.h"
#include "def/rcp_sender_l1.h"

rcp_sender_cluster_ref rcp_sender_cluster_new();
rcp_sender_l1_ref rcp_sender_cluster_json_nt(
		rcp_sender_cluster_ref cluster);
rcp_sender_l1_ref rcp_sender_cluster_json_ws(
		rcp_sender_cluster_ref cluster);
rcp_sender_l1_ref rcp_sender_cluster_dummy(
		rcp_sender_cluster_ref cluster);
void rcp_sender_cluster_set_data(rcp_sender_cluster_ref cluster,
		rcp_type_ref type, rcp_data_ref data);
void rcp_sender_cluster_clean_up(rcp_sender_cluster_ref cluster);

