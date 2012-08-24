#include "rcp_pch.h"
#include "rcp_utility.h"
#include "def/rcp_io.h"

#include "types/rcp_array.h"
#include "types/rcp_type_list.h"
#include "rcp_record.h"
#include "rcp_receiver.h"
#include "rcp_sender_classes.h"
#include "rcp_receiver_classes.h"
#include "rcp_connection.h"
#include "connections/con_file.h"
#include "rcp_server.h"

#define RCP_INTERNAL_STRUCTURE
#include "rcp_context.h"

void rcp_context_page_out(rcp_context_ref ctx)
{
	rcp_connection_ref con = rcp_connection_new();
	rcp_io_ref io = con_file_io_new_wr("contexts/file");
	rcp_connection_set_io(con, io);
	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_l1_ref sender = rcp_sender_cluster_json_nt(cls);
	rcp_connection_set_sender(con, sender);

	rcp_context_send_all_data(ctx, con);	

	rcp_connection_delete(con);
}

void rcp_context_page_in(rcp_context_ref ctx)
{
	rcp_connection_ref con = rcp_connection_new();
	rcp_io_ref io = con_file_io_new_rd("contexts/file");
	rcp_connection_set_io(con, io);
	rcp_receiver_ref receiver = rcp_receiver_new(&con_nt_json_class);
	rcp_connection_set_receiver(con, receiver);
	rcp_connection_set_context(con, ctx);

	while (rcp_connection_alive(con)){
		rcp_connection_on_receive(con);
	}

	rcp_connection_delete(con);
}
