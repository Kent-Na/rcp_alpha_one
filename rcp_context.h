#include "def/rcp_context.h"
#include "def/rcp_connection.h"
#include "def/rcp_record.h"
#include "def/rcp_string.h"
#include "def/rcp_tree.h"
#include "def/rcp_dict.h"
#include "def/rcp_array.h"
#include "def/rcp_data.h"
#include "def/rcp_type.h"

typedef uint64_t rcp_context_id_t;
typedef uint64_t rcp_context_state_t;

#define RCP_CTX_PIRTIALY_LOADED	(1<<0)
#define RCP_CTX_FULLY_LOADED	(1<<1)
#define RCP_CTX_DIRTY			(1<<2)

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_context_core{
	rcp_context_state_t state;
	rcp_context_id_t id;
	rcp_context_ref parent_context;

	rcp_record_ref top_level_record;
	rcp_tree_ref connections;

	//string - uint64
	rcp_dict_ref permissions;

	rcp_permission_t base_permission;

	//string - ptr
	rcp_dict_ref types;

	//connections that closed but not send "removeUser" command. 
	rcp_array_ref dead;

	//string - ptr
	rcp_dict_ref sub_context;
};
#endif

rcp_extern rcp_context_ref rcp_context_new();
rcp_extern void rcp_context_init(rcp_context_ref ctx);

//connection management
rcp_extern void rcp_context_add_connection(rcp_context_ref ctx, 
		rcp_connection_ref con);

rcp_extern void rcp_context_remove_connection(rcp_context_ref ctx,
		rcp_connection_ref con);

void rcp_context_test_and_kill(
		rcp_context_ref ctx, rcp_connection_ref con);

//data management

rcp_extern rcp_context_state_t rcp_context_state_flag(
		rcp_context_ref ctx);
rcp_extern void rcp_context_set_state_flag(
		rcp_context_ref ctx,
		rcp_context_state_t state);
rcp_extern void rcp_context_unset_state_flag(
		rcp_context_ref ctx,
		rcp_context_state_t state);

rcp_extern rcp_record_ref rcp_context_top_level_record(rcp_context_ref ctx);

void rcp_context_add_context(rcp_context_ref ctx, 
		rcp_string_ref name, rcp_context_ref new_ctx);
rcp_extern void rcp_context_execute_command_rec(
		rcp_context_ref ctx,
		rcp_connection_ref con, rcp_record_ref cmd);

void rcp_context_load_from_io(rcp_context_ref ctx, rcp_io_ref io);
void rcp_context_store_into_io(rcp_context_ref ctx, rcp_io_ref io);
//ext imp
void rcp_context_load_sub_contexts(rcp_context_ref ctx);
void rcp_context_load(rcp_context_ref ctx);
void rcp_context_store(rcp_context_ref ctx);
void rcp_context_page_in(rcp_context_ref ctx);
void rcp_context_page_out(rcp_context_ref ctx);

void rcp_page_out_r(rcp_context_ref ctx);

void rcp_page_in_r(rcp_context_ref ctx);

void rcp_context_load_from_file(rcp_context_ref);
void rcp_context_store_as_file(rcp_context_ref);

void rcp_context_create_db_info(
		rcp_context_ref ctx,
		rcp_string_ref name, rcp_context_ref new_ctx);

rcp_extern void rcp_login_root_context(
		rcp_connection_ref con);

void rcp_context_send_data(rcp_context_ref ctx, 
		rcp_type_ref type, rcp_data_ref data);
void rcp_context_send_all_con(
		rcp_context_ref ctx, rcp_connection_ref con);
void rcp_context_send_all_data(
		rcp_context_ref ctx, rcp_connection_ref con);
void rcp_context_send_all_sub_ctx(
		rcp_context_ref ctx, rcp_connection_ref con);
void rcp_context_send_all_permission(
		rcp_context_ref ctx, rcp_connection_ref con);
void rcp_context_send_error(rcp_connection_ref con, 
		rcp_record_ref cause, const char* reason);
void rcp_context_send_caution(rcp_connection_ref con, 
		rcp_record_ref cause, const char* reason);
void rcp_context_send_info(rcp_connection_ref con, 
		rcp_record_ref cause, const char* info);

int64_t rcp_context_permission(
		rcp_context_ref ctx, rcp_string_ref username);

void rcp_context_set_permission(
		rcp_context_ref ctx, rcp_string_ref username, int64_t pms);
void rcp_context_unset_permission(
		rcp_context_ref ctx, rcp_string_ref username);
