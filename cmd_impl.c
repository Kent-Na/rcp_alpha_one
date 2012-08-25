#include "rcp_pch.h"
#include "rcp_utility.h"

#include "cmd_types.h"

#include "rcp_server.h"

#include "rcp_connection.h"
#include "rcp_type.h"
#include "rcp_record.h"
#include "rcp_caster.h"

#include "rcp_user.h"

#include "types/rcp_type_list.h"

#include "types/rcp_array.h"
#include "types/rcp_dict.h"
#include "types/rcp_dict_list.h"
#include "types/rcp_string.h"
#include "types/rcp_alias.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_context.h"

void cmd_impl_kill(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_listen_end();
}

///
//context
//
void cmd_impl_login_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_login_context *cmd_recv = cmd;

	if (!cmd_recv->name){
		rcp_context_send_caution(con, cmd_rec, 
				"Not enough parameters.");
		return;
	}

	rcp_dict_node_ref node;
	node = rcp_dict_find(ctx->sub_context, 
			rcp_record_data(cmd_recv->name));

	if (!node){
		rcp_context_send_caution(con, cmd_rec, 
				"context not found");
		return;
	}

	rcp_data_ref ctx_ref = rcp_dict_node_data(
			rcp_str_ptr_dict, node);
	rcp_context_ref new_ctx = *(rcp_context_ref*)ctx_ref;

	rcp_string_ref username = (rcp_string_ref)rcp_record_data(
			rcp_connection_username(con));
	uint64_t pms = rcp_context_permission(new_ctx, username);

	if (! (pms & RCP_PMS_LOGIN)){
		rcp_context_send_caution(con, cmd_rec, 
				"not enough permission");
		return;
	}
	rcp_context_remove_connection(ctx, con);

	rcp_context_send_info(con, cmd_rec, "Loggin succeed.");
	
	rcp_context_send_all_con(new_ctx, con);
	rcp_context_send_all_data(new_ctx, con);	
	rcp_context_send_all_sub_ctx(new_ctx, con);	
	rcp_context_add_connection(new_ctx, con);
}

void cmd_impl_add_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_add_context *cmd_recv = cmd;

	if (!cmd_recv->name){
		rcp_context_send_caution(con, cmd_rec, 
				"Not enough parameters.");
	}

	rcp_context_ref new_ctx = rcp_context_new();
	rcp_context_set_state_flag(new_ctx, RCP_CTX_PIRTIALY_LOADED);
	rcp_context_set_state_flag(new_ctx, RCP_CTX_FULLY_LOADED);
	rcp_context_add_context(ctx,
			(rcp_string_ref)rcp_record_data(cmd_recv->name),
			new_ctx);
	rcp_context_create_db_info(ctx,
			(rcp_string_ref)rcp_record_data(cmd_recv->name),
			new_ctx);
	rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)cmd_recv);
}

///
//user
//
void cmd_impl_create_user(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_create_user *cmd_recv = cmd;

	if (!(cmd_recv->username && cmd_recv->password)){
		rcp_context_send_caution(con, cmd_rec, 
				"Not enough parameters.");
		return;
	}

	rcp_string_ref username = 
		(rcp_string_ref)rcp_record_data(cmd_recv->username);
	rcp_string_ref password = 
		(rcp_string_ref)rcp_record_data(cmd_recv->password);

	int r = rcp_user_create(rcp_string_c_str(username),
							rcp_string_c_str(password));
	if (r != 0){
		rcp_context_send_caution(con, cmd_rec, 
				"fail!!");
	}
}

void cmd_impl_login_user(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_login_user *cmd_recv = cmd;

	if (rcp_connection_username(con)){
		rcp_context_send_caution(con, cmd_rec, 
				"you are already user logined");
		return;
	}

	if (!(cmd_recv->username && cmd_recv->password)){
		rcp_context_send_caution(con, cmd_rec, 
				"Not enough parameters.");
		return;
	}

	rcp_string_ref username = 
		(rcp_string_ref)rcp_record_data(cmd_recv->username);
	rcp_string_ref password = 
		(rcp_string_ref)rcp_record_data(cmd_recv->password);

	int r = rcp_user_autenticate(rcp_string_c_str(username),
								rcp_string_c_str(password));
	if (r != 1){
		rcp_context_send_caution(con, cmd_rec, 
				"Inncorrect username and password pair.");
	}
	else{
		rcp_connection_set_username(con, cmd_recv->username);
		rcp_context_send_info(con, 
				cmd_rec, "Loggin succeed.");
	}
}

void cmd_impl_dump(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_page_out_r(ctx);
	//rcp_context_page_out(ctx);
}

void cmd_impl_load(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_page_in_r(ctx);
	//rcp_context_page_in(ctx);
}

void cmd_impl_add_permission(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_add_permission *cmd_recv = cmd;
	rcp_string_ref username = (rcp_string_ref)rcp_record_data(
			cmd_recv->username);
	uint64_t old = rcp_context_permission(ctx, username);
	uint64_t new = old;
		new |= RCP_PMS_LOGIN;
		new |= RCP_PMS_READ;
		new |= RCP_PMS_WRITE;
	rcp_context_set_permission(ctx, username, new);
}

void cmd_impl_remove_permission(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_remove_permission *cmd_recv = cmd;
	rcp_string_ref username = (rcp_string_ref)rcp_record_data(
			cmd_recv->username);
	uint64_t old = rcp_context_permission(ctx, username);
	uint64_t new = old;
	new &= ~RCP_PMS_LOGIN;
	new &= ~RCP_PMS_READ;
	new &= ~RCP_PMS_WRITE;
	rcp_context_set_permission(ctx, username, new);
}

void cmd_impl_send_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_send_value *cmd_st = cmd;
	rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)cmd_st);
}

void cmd_impl_unset_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_record_release(ctx->top_level_record);
	ctx->top_level_record = 
		rcp_array_new_rec(rcp_ref_type);
}

int rcp_record_cast(
		rcp_context_ref ctx,
		rcp_record_ref type_rec,
		rcp_record_ref target_rec)
{
	if (! type_rec)
		return 0;

	if (rcp_record_type(type_rec) == rcp_null_type)
		return 0;

	if (rcp_record_type(type_rec) != rcp_string_type)
		return -1;

	rcp_dict_node_ref node = rcp_dict_find(ctx->types,
			rcp_record_data(type_rec));

	if (!node)
		return -1;

	rcp_type_ref dst_type = *(rcp_type_ref*)
		rcp_dict_node_data(rcp_str_ptr_dict, node);
	
	rcp_record_change_type(target_rec, dst_type);

	if (rcp_record_type(target_rec) != dst_type)
		return -1;

	return 0;
}

void cmd_impl_set_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_set_value *cmd_st = cmd;

	if (! cmd_st->value)
		return;

	if (rcp_record_cast(ctx, cmd_st->type, cmd_st->value)){
		rcp_context_send_caution(con, cmd_rec, 
				"type err.");
		return;
	}

	if ((!(cmd_st->path)) || 
			rcp_record_type(cmd_st->path) == rcp_null_type ){
		rcp_record_release(ctx->top_level_record);
		ctx->top_level_record = 
			rcp_record_retain(cmd_st->value);
		return;
	}

	rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);

	rcp_set(rcp_record_type(tlo_rec), rcp_record_data(tlo_rec),
			rcp_ref_type, (rcp_data_ref)&cmd_st->path,
			rcp_ref_type, (rcp_data_ref)&cmd_st->value);

	rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)cmd_st);
}

void cmd_impl_append_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_append_value *cmd_st = cmd;

	if (! cmd_st->value)
		return;

	rcp_record_ref tlo_rec = rcp_context_top_level_record(ctx);

	if (! tlo_rec)
		return;

	rcp_append(rcp_record_type(tlo_rec), rcp_record_data(tlo_rec),
			rcp_ref_type, (rcp_data_ref)&cmd_st->value);

	rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)cmd_st);
}

void cmd_impl_add_type(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_add_type *cmd_st = cmd;

	if (cmd_st->name && rcp_record_type(cmd_st->name) != rcp_string_type)
		return;

	rcp_type_ref new_type = rcp_alias_type_new(rcp_str_ref_dict);
	rcp_dict_node_ref node = rcp_dict_node_new(rcp_str_ptr_dict);
	rcp_copy(rcp_string_type,
			rcp_record_data(cmd_st->name),
			rcp_dict_node_key(rcp_str_ptr_dict, node));
	rcp_copy(rcp_pointer_type,
			(rcp_data_ref)&new_type,
			rcp_dict_node_data(rcp_str_ptr_dict, node));
	rcp_dict_set_node(ctx->types, node);

	rcp_data_ref name = rcp_new(rcp_string_type);
	rcp_copy(rcp_string_type,
			rcp_record_data(cmd_st->name),
			name);
	rcp_type_set_name(new_type, (rcp_string_ref)name);
}

void cmd_impl_open(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}

void cmd_impl_close(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}

void cmd_impl_ping(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}

void cmd_impl_pong(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}


void cmd_impl_delete_user(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}

void cmd_impl_add_user(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}

void cmd_impl_remove_user(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}


void cmd_impl_remove_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}


void cmd_impl_update_name(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}


void cmd_impl_create_struct(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}

void cmd_impl_add_struct(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}

void cmd_impl_error(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}

void cmd_impl_caution(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}

extern void cmd_impl_info(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}
