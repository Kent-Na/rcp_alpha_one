typedef struct rcp_record_core *rcp_record_ref;

struct rcp_record_core{
	rcp_type_ref type;
	//rcp_structure_ref structure;
	//uint32_t record_id;
};

rcp_extern rcp_record_ref rcp_record_new(rcp_type_ref type);
rcp_extern rcp_type_ref rcp_record_type(rcp_record_ref rec);
rcp_extern void* rcp_record_data(rcp_record_ref rec);

struct rcp_structure_core{
	rcp_string_ref name;
	//bool contain ref
	size_t size;
	uint16_t parameter_count;
};

struct rcp_structure_parameter{
	//rcp_string_ref name;
	//type;
	//size_t size;
	rcp_type_ref type;
	size_t offset;
};

typedef void *rcp_structure_ref;

/*
rcp_record_ref rcp_record_new(rcp_structure_ref st);

rcp_record_ref rcp_record_clone(rcp_record_ref src_record);

struct rcp_structure_parameter* rcp_parameter_from_str(
		rcp_structure_ref st, const char* str);

struct pm_task{
	const char* name;
	uint8_t progress;
};

struct {
	struct rcp_structure_core core;
	struct rcp_structure_parameter param[2];
} pm_task_structure = {
	{
		"com.tuna-cat.pm.task",
		2
	},
	{
		{
			"name",
			sizeof (rcp_string_ref),
			offsetof(struct pm_task, name)
		},
		{
			"progress",
			sizeof (uint8_t),
			offsetof(struct pm_task, name)
		}
	}
};
*/
