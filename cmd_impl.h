//This file was generated by program.
#include "def/rcp_context.h"
#include "def/rcp_connection.h"
#include "def/rcp_record.h"
#include "def/rcp_type.h"

void cmd_impl_open(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_kill(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_dump(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_load(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_close(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_ping(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_pong(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_create_user(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_delete_user(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_login_user(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_add_user(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_remove_user(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_add_permission(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_remove_permission(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_add_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_remove_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_login_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_logout_context(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_update_name(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_send_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_set_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_unset_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_append_value(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_create_struct(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_add_struct(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_add_type(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_error(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_caution(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);

void cmd_impl_info(
		rcp_context_ref ctx,
		rcp_connection_ref con,
		rcp_record_ref cmd_rec,
		rcp_type_ref cmd_type,
		void* cmd);