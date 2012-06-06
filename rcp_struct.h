//rcp_structure_type memory_placement

//rcp_type_core
//rcp_structure_core
//rcp_structure_parameter (x parameter count);

#ifdef RCP_INTERNAL_STRUCTURE
struct rcp_struct_core{
	rcp_string_ref name;
	uint16_t parameter_count;
};

struct rcp_struct_parameter{
	rcp_string_ref name;
	rcp_type_ref type;
	size_t offset;
};
#endif

typedef struct rcp_struct_parameter *rcp_struct_parameter_ref;
typedef rcp_record_ref rcp_struct_ref;

void rcp_struct_type_init(rcp_type_ref type, rcp_data_ref data);
void rcp_struct_type_deinit(rcp_type_ref type, rcp_data_ref data);
void rcp_struct_type_copy(rcp_type_ref type, 
		rcp_data_ref l, rcp_data_ref r);
