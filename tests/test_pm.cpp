#include "../rcp_pch.h"
#include "../rcp_utility.h"

#include "../rcp_tree.h"

#include "../types/rcp_type_utility.h"
#include "../rcp_record.h"
#include "../types/rcp_type_list.h"
#include "../types/rcp_string.h"
#include "../types/rcp_map.h"

#define RCP_INTERNAL_STRUCTURE

#include "../rcp_type.h"
#include "../types/rcp_struct.h"

struct pm_task{
	rcp_record_ref name;
	uint8_t progress;
};

int test_struct(void){

	rcp_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct pm_task);
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("com.tuna-cat.pm.task");
	s_core->param_count = 2;

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);
	param->name = rcp_string_new("name");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct pm_task, name);

	param ++;
	param->name = rcp_string_new("progress");
	param->type = rcp_uint8_type;
	param->offset = offsetof(struct pm_task, progress);

	rcp_record_ref rec = rcp_record_new(s_type);
	struct pm_task* task = (struct pm_task*)rcp_record_data(rec);
	task->name = rcp_string_new_rec("set command");
	task->progress = 32;

	rcp_map_ref map = rcp_map_new(rcp_string_type, rcp_ref_type);
	rcp_struct_to_map(s_type, (rcp_struct_ref)task, map);

	struct pm_task cpy;
	cpy.name = NULL;
	cpy.progress = 0;
	rcp_map_to_struct(map, s_type, (rcp_struct_ref)&cpy);

	if (task->name != cpy.name)
		rcp_error("name");
	if (task->progress != cpy.progress)
		rcp_error("pgs");

	rcp_info("struct done");
	return 0;
}
