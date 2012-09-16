#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"
#include "def/rcp_io.h"

#include "connections/con_file.h"
#include "rcp_context.h"

void rcp_context_store_into_file(rcp_context_ref ctx)
{
	rcp_io_ref io = con_file_io_new_wr(RCP_INIT_FILE);
	rcp_context_store_into_io(ctx, io);
}

void rcp_context_load_from_file(rcp_context_ref ctx)
{
	rcp_io_ref io = con_file_io_new_rd(RCP_INIT_FILE);
	rcp_context_load_from_io(ctx, io);
}
