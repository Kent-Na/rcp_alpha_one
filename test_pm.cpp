#include "rcp_pch.h"
#include "rcp_utility.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_struct.h"

#include "rcp_type_utility.h"

struct pm_task{
	rcp_record_ref name;
	uint8_t progress;
};
/*
struct {
	struct rcp_type_core t_core
	struct rcp_structure_core s_core;
	struct rcp_structure_parameter param[2];
} pm_task_structure = {
	{
		sizeof(struct rcp_struct),
		RCP_TYPE_MAP,
		"pm_task",
		rcp_struct_type_init,//init
		rcp_struct_type_deinit,//free
		NULL,//copy
		NULL,//comp
	}
	{
		"com.tuna-cat.pm.task",
		2
	},
	{
		{
			"name",
			rcp_ref_type,
			offsetof(struct pm_task, name)
		},
		{
			"progress",
			rcp_uint8_type,
			offsetof(struct pm_task, progress)
		}
	}
};
*/


int test_struct(void){

	rcp_struct_type_ref s_type = rcp_struct_type_new(2);
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct pm_task);
	t_core->type_id = 0x100;
	t_core->type_name = "pm_task";
	t_core->init = rcp_struct_type_init;
	t_core->deinit = rcp_struct_type_deinit;
	t_core->copy = NULL;
	t_core->compare = NULL;

	struct rcp_structure_core* s_core = 
		(struct rcp_structure_core)(t_core+1) 
	s_core->name = "com.tuna-cat.pm.task";
	s_core->parameter_count = 2;

	struct rcp_struct_parameter *param = 
		(struct rcp_structure_parameter)(s_core+1) 
	param->name = rcp_string_new("name");
	param->type = rcp_ref_type;
	param->offset = offsetof(struct pm_task, name);

	param ++;
	param->name = rcp_string_new("progress");
	param->type = rcp_uint8_type;
	param->offset = offsetof(struct pm_task, progress);

	rcp_record_ref rec = rcp_record_new(s_type);
	struct pm_task* task = rcp_record_data(rec);
	task->name = rcp_string_new("set command");
	task->progress = 255;

	rcp_map map = rcp_struct_to_map(rec);
	rec = rcp_map_to_struct(map);
}
