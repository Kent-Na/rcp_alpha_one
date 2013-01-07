#include "rcp_pch.h"
#include "rcp_utility.h"

#include "rcp_command.h"
#include "cmd_list.h"
#include "cmd_types.h"

#include "rcp_server.h"

#include "rcp_connection.h"
#include "rcp_type.h"
#include "rcp_record.h"
#include "rcp_caster.h"

#include "rcp_user.h"

#include "types/rcp_type_list.h"
#include "types/rcp_type_utility.h"

#include "types/rcp_old_array.h"
#include "types/rcp_dict.h"
#include "types/rcp_dict_list.h"
#include "types/rcp_array.h"
#include "types/rcp_array_list.h"
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

void cmd_util_move_context(
		rcp_context_ref old_ctx,
		rcp_context_ref new_ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec)
{
	rcp_string_ref username = (rcp_string_ref)rcp_record_data(
			rcp_connection_username(con));
	rcp_permission_t pms = rcp_context_permission(new_ctx, username);

	if (! (pms & RCP_PMS_LOGIN)){
		rcp_context_send_error(con, cmd_rec, 
				"not enough permission");
		return;
	}

	rcp_connection_retain(con);
	{
		rcp_context_remove_connection(old_ctx, con);
		rcp_connection_set_permission(con, pms);

		rcp_context_send_info(con, cmd_rec, "Login/Logout succeed.");
		
		rcp_context_send_all_con(new_ctx, con);
		rcp_context_send_all_data(new_ctx, con);	
		rcp_context_send_all_sub_ctx(new_ctx, con);	

		rcp_context_add_connection(new_ctx, con);
		//rcp_context_send_info(con, cmd_rec, "Context was initialized.");
	}
	rcp_connection_release(con);
}

void cmd_impl_login_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_login_context *cmd_recv = cmd;

	if (!cmd_recv->name){
		rcp_context_send_error(con, cmd_rec, 
				"Not enough parameters.");
		return;
	}

	rcp_dict_node_ref node = rcp_dict_find(
			ctx->sub_context, 
			rcp_record_data(cmd_recv->name));

	if (!node){
		rcp_context_send_error(con, cmd_rec, 
				"context not found");
		return;
	}

	rcp_data_ref ctx_ref = rcp_dict_node_data(
			rcp_str_ptr_dict, node);
	rcp_context_ref new_ctx = *(rcp_context_ref*)ctx_ref;

	cmd_util_move_context(ctx, new_ctx, con, cmd_rec);
}

void cmd_impl_logout_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{

	if (!ctx->parent_context){
		rcp_context_send_error(con, cmd_rec, 
				"You are in root context");
		return;
	}

	rcp_context_ref new_ctx = ctx->parent_context;
	cmd_util_move_context(ctx, new_ctx, con, cmd_rec);
}

void cmd_impl_reset_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{

	if (!ctx->parent_context){
		rcp_context_send_error(con, cmd_rec, 
				"You are in root context");
		return;
	}

	cmd_util_move_context(ctx, ctx, con, cmd_rec);
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
		rcp_context_send_error(con, cmd_rec, 
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
	rcp_context_send_info(con, cmd_rec, "Add context succeed.");
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
		rcp_context_send_error(con, cmd_rec, 
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
		rcp_context_send_error(con, cmd_rec, 
				"fail!!");
		return;
	}
	rcp_context_send_info(con, cmd_rec, "Succeed.");
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
		rcp_context_send_error(con, cmd_rec, 
				"you are already user logined");
		return;
	}

	if (!(cmd_recv->username && cmd_recv->password)){
		rcp_context_send_error(con, cmd_rec, 
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
		rcp_context_send_error(con, cmd_rec, 
				"Inncorrect username and password pair.");
		return;
	}

	rcp_permission_t pms = rcp_context_permission(ctx, username);
	if (! (pms & RCP_PMS_LOGIN)){
		rcp_context_send_error(con, cmd_rec, 
				"not enough context login permission");
		return;
	}

	rcp_connection_set_permission(con, pms);
	rcp_connection_set_username(con, cmd_recv->username);
	rcp_context_send_info(con, 
			cmd_rec, "Loggin succeed.");

	{
		struct cmd_remove_user cmd;
		rcp_type_ref type_cmd = rcp_command_type(CMD_REMOVE_USER);
		rcp_init(type_cmd, (rcp_data_ref)&cmd);
		cmd.command = rcp_string_new_rec(CMD_STR_REMOVE_USER);
		cmd.loginID = rcp_connection_login_id(con);
		rcp_context_send_data(ctx,
				type_cmd, (rcp_data_ref)&cmd);
		rcp_deinit(type_cmd, (rcp_data_ref)&cmd);
	}
	{
		struct cmd_add_user cmd;
		rcp_type_ref type_cmd = rcp_command_type(CMD_ADD_USER);
		rcp_init(type_cmd, (rcp_data_ref)&cmd);
		cmd.command = rcp_string_new_rec(CMD_STR_ADD_USER);
		cmd.loginID = rcp_connection_login_id(con);
		cmd.username = rcp_record_retain(cmd_recv->username);
		rcp_context_send_data(ctx,
				type_cmd, (rcp_data_ref)&cmd);
		rcp_deinit(type_cmd, (rcp_data_ref)&cmd);
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


void cmd_impl_set_permission(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_set_permission *cmd_recv = cmd;
	rcp_string_ref username = NULL;
	if (!rcp_record_is_null(cmd_recv->username)){
		if (rcp_record_type(cmd_recv->username) == rcp_string_type)
			username = (rcp_string_ref)rcp_record_data(cmd_recv->username);
		else
			return;
	}
	uint64_t new = rcp_permission_from_array(cmd_recv->mode);
	rcp_context_set_permission(ctx, username, new);
}

void cmd_impl_unset_permission(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_unset_permission *cmd_recv = cmd;
	rcp_string_ref username = NULL;
	if (rcp_record_type(cmd_recv->username) == rcp_string_type)
		username = (rcp_string_ref)rcp_record_data(cmd_recv->username);
	else if (!rcp_record_is_null(cmd_recv->username))
		return;
	rcp_context_unset_permission(ctx, username);
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

void cmd_value_at_path(rcp_context_ref ctx, rcp_record_ref path, 
		rcp_type_ref *o_type, rcp_data_ref *o_data)
{

	*o_type = rcp_ref_type;
	*o_data = (rcp_data_ref)&ctx->top_level_record;
	if (rcp_record_is_null(path)){
		return;
	}

	if (rcp_record_type(path) == rcp_ref_array){
		*o_type = rcp_ref_type;
		*o_data = (rcp_data_ref)&ctx->top_level_record;
		rcp_array_ref path_array =(rcp_array_ref)rcp_record_data(path);
		rcp_data_at(o_type, o_data, path_array);
		return;
	}

	{
		rcp_at(o_type, o_data, 
				rcp_record_type(path), rcp_record_data(path));
		return;
	}
}

void cmd_impl_set_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_set_value *cmd_st = cmd;

	rcp_assert(cmd_st->value,"null value");

	if (rcp_record_cast(ctx, cmd_st->type, cmd_st->value)){
		rcp_context_send_error(con, cmd_rec, "type err.");
		return;
	}

	rcp_type_ref o_type;
	rcp_data_ref o_data;

	cmd_value_at_path(ctx, cmd_st->path, &o_type, &o_data);

	if (o_data){
		//value already existed
		rcp_assert(o_type == rcp_ref_type, "can not copy");

		rcp_copy(rcp_ref_type, (rcp_data_ref)&cmd_st->value, o_data);
		rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)cmd_st);
		return;
	}

	rcp_context_send_error(con, cmd_rec, "path err.");
}

void cmd_impl_unset_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}

void cmd_impl_append_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_append_value *cmd_st = cmd;

	rcp_assert(cmd_st->value,"null value");

	if (rcp_record_cast(ctx, cmd_st->type, cmd_st->value)){
		rcp_context_send_error(con, cmd_rec, "type err.");
		return;
	}

	rcp_type_ref o_type;
	rcp_data_ref o_data;

	cmd_value_at_path(ctx, cmd_st->path, &o_type, &o_data);

	if (o_data){
		//value already existed
		rcp_assert(o_type == rcp_ref_type, "can not copy");
		rcp_append(o_type, o_data,
				rcp_ref_type, (rcp_data_ref)&cmd_st->value);
		rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)cmd_st);
		return;
	}

	rcp_context_send_error(con, cmd_rec, "path err.");
}

void cmd_impl_replace_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	struct cmd_replace_value *cmd_st = cmd;

	rcp_assert(cmd_st->value,"null value");

	rcp_type_ref o_type;
	rcp_data_ref o_data;

	cmd_value_at_path(ctx, cmd_st->path, &o_type, &o_data);
	if (o_type == rcp_ref_type){
		o_type = rcp_record_type(*(rcp_record_ref*)o_data);
		o_data = rcp_record_data(*(rcp_record_ref*)o_data);
	}

	if (o_data){
		if (rcp_record_type(cmd_st->value) != o_type)
			return;
		uint8_t err = rcp_array_replace(o_type, o_data, 
			cmd_st->begin, cmd_st->end, rcp_record_data(cmd_st->value));
		if (!err)
			rcp_context_send_data(ctx, cmd_type, (rcp_data_ref)cmd_st);
		return;
	}

	rcp_context_send_error(con, cmd_rec, "path err.");
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
	struct cmd_open *cmd_st = cmd;

	if (!(cmd_st->protocol)){
		rcp_context_send_fatal(con, cmd_rec, 
			"Invalid parameter.");
		return;
	}
	rcp_string_ref protocol = (rcp_string_ref)rcp_record_data(
			cmd_st->protocol);
	if (strcmp(rcp_string_c_str(protocol), "alpha1")){
		rcp_context_send_fatal(con, cmd_rec, 
			"Invalid protocol.");
		return;
	}

	rcp_connection_open(con, cmd_st->protocol, cmd_st->client);
	rcp_login_root_context(con);
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

void cmd_impl_fatal(
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

extern void cmd_impl_update_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd)
{
	rcp_context_send_caution(con, cmd_rec, 
			"Not yet implemented.");
}
