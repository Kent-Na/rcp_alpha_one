#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "../rcp_json_write.h"

#define RCP_INTERNAL_STRUCTURE
#include "../rcp_type.h"
#include "rcp_type_list.h"
#include "../rcp_tree.h"
#include "rcp_dict.h"


struct rcp_dict_core{
	struct rcp_type_core core;
	struct rcp_type_dict_ext ext;
};

#define RCP_DICT_TEMPLATEA \
	{\
		sizeof(struct rcp_tree_core),\
		0,\
		NULL,\
		rcp_dict_init,\
		rcp_dict_deinit,\
		NULL,\
		NULL,\
		rcp_dict_write_json,\
	}

#define RCP_DICT_TEMPLATEB(key,data) \
	{RCP_DICT_TEMPLATEA,{(key),(data)}}

#define RCP_DICT_TEMPLATE(name,key,data) \
	struct rcp_dict_core name = RCP_DICT_TEMPLATEB(key,data);


RCP_DICT_TEMPLATE(rcp_uint32_ptr_dict_def, 
		&rcp_uint32_type_def, &rcp_pointer_type_def);
RCP_DICT_TEMPLATE(rcp_str_ref_dict_def, 
		&rcp_string_type_def, &rcp_ref_type_def);
RCP_DICT_TEMPLATE(rcp_str_uint64_dict_def, 
		&rcp_string_type_def, &rcp_uint64_type_def);
RCP_DICT_TEMPLATE(rcp_str_ptr_dict_def, 
		&rcp_string_type_def, &rcp_pointer_type_def);

const rcp_type_ref rcp_uint32_ptr_dict = 
	&rcp_uint32_ptr_dict_def.core;
const rcp_type_ref rcp_str_uint64_dict = 
	&rcp_str_uint64_dict_def.core;
const rcp_type_ref rcp_str_ref_dict = 
	&rcp_str_ref_dict_def.core;
const rcp_type_ref rcp_str_ptr_dict = 
	&rcp_str_ptr_dict_def.core;
