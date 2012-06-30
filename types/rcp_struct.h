//rcp_structure_type memory_placement

//rcp_type_core
//rcp_structure_core
//rcp_structure_parameter (x parameter count);

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_type_struct_ext{
	rcp_string_ref name;
	uint16_t param_count;
};

struct rcp_struct_param_core{
	rcp_string_ref name;
	rcp_type_ref type;
	size_t offset;
};
typedef struct rcp_struct_param_core *rcp_struct_param_ref;
#else
typedef struct rcp_struct_param_opaque *rcp_struct_param_ref;
#endif

typedef rcp_type_ref rcp_struct_type_ref;
typedef rcp_data_ref rcp_struct_ref;

rcp_extern void rcp_struct_init(rcp_type_ref type, rcp_data_ref data);
rcp_extern void rcp_struct_deinit(rcp_type_ref type, rcp_data_ref data);
rcp_extern void rcp_struct_copy(rcp_type_ref type, 
		rcp_data_ref l, rcp_data_ref r);

rcp_extern rcp_struct_type_ref rcp_struct_type_new(uint16_t param_count);

rcp_data_ref rcp_struct_data(rcp_struct_ref st, rcp_struct_param_ref param);

rcp_struct_param_ref rcp_struct_type_begin(rcp_struct_type_ref type);
rcp_string_ref rcp_struct_param_name(rcp_struct_param_ref param);
rcp_type_ref rcp_struct_param_type(rcp_struct_param_ref param);
rcp_struct_param_ref rcp_struct_param_next(
		rcp_struct_type_ref type, rcp_struct_param_ref param);