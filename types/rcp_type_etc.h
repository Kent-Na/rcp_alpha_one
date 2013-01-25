
void rcp_bool8_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);
void rcp_bool32_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);

void rcp_ref_init(rcp_type_ref type, rcp_data_ref data);
void rcp_ref_deinit(rcp_type_ref type, rcp_data_ref data);
void rcp_ref_copy(
		rcp_type_ref type, rcp_data_ref src, rcp_data_ref dst);
void rcp_ref_set(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref key_type, rcp_data_ref key_data,
		rcp_type_ref data_type, rcp_data_ref data_data);
void rcp_ref_unset(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref key_type, rcp_data_ref key_data);
void rcp_ref_append(rcp_type_ref type, rcp_data_ref dst,
		rcp_type_ref data_type, rcp_data_ref data_data);
void rcp_ref_at(rcp_type_ref *io_type, rcp_data_ref *io_data,
		rcp_type_ref key_type, rcp_data_ref key_data);
