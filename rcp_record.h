#include "def/rcp_type.h"
#include "def/rcp_data.h"
#include "def/rcp_record.h"

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_record_core{
	rcp_type_ref type;
	uint64_t owner_id;
	uint32_t ref_count;
	uint32_t frags;
};
#endif


#define RCP_REC_FRAG_ACTIVE (1<<0)
#define RCP_REC_FRAG_OWNED (1<<1)

rcp_extern rcp_record_ref rcp_record_new(rcp_type_ref type);
rcp_extern rcp_record_ref rcp_record_retain(rcp_record_ref rec);
rcp_extern void rcp_record_release(rcp_record_ref rec);
rcp_extern void rcp_record_init(rcp_record_ref type);
rcp_extern void rcp_record_deinit(rcp_record_ref type);
rcp_extern rcp_type_ref rcp_record_type(rcp_record_ref rec);
rcp_extern rcp_data_ref rcp_record_data(rcp_record_ref rec);
