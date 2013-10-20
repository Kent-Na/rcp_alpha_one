#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "rcp_json_write.h"

#define RCP_INTERNAL_STRUCTURE
#include "rcp_type.h"
#include "rcp_type_list.h"
#include "rcp_tree.h"
#include "rcp_array.h"

#define RCP_ARRAY_TEMPLATE_A_A\
	{\
		sizeof(struct rcp_array_core),\
		0,\
		NULL,\
		rcp_array_init,\
		rcp_array_deinit,\
		rcp_array_copied,\
		rcp_default_compare,\
		rcp_array_write_json,\
		rcp_array_at,\
		rcp_array_replace,\
		rcp_default_merge,\
		rcp_default_unset,\
	}

#define RCP_ARRAY_TEMPLATE_A(def_name, param_name) \
	struct rcp_array_type def_name = \
	{RCP_ARRAY_TEMPLATE_A_A,{(param_name)}};

#define RCP_ARRAY_TEMPLATE_B(type_name, def_name) \
	const rcp_type_ref type_name = &def_name.core;

#define RCP_ARRAY_TEMPLATE(TYPE) \
	RCP_ARRAY_TEMPLATE_A(TYPE ## _array_def, &TYPE ## _type_def)\
	RCP_ARRAY_TEMPLATE_B(TYPE ## _array, TYPE ## _array_def)

RCP_ARRAY_TEMPLATE(rcp_int8);
RCP_ARRAY_TEMPLATE(rcp_int16);
RCP_ARRAY_TEMPLATE(rcp_int32);
RCP_ARRAY_TEMPLATE(rcp_int64);

RCP_ARRAY_TEMPLATE(rcp_uint8);
RCP_ARRAY_TEMPLATE(rcp_uint16);
RCP_ARRAY_TEMPLATE(rcp_uint32);
RCP_ARRAY_TEMPLATE(rcp_uint64);

RCP_ARRAY_TEMPLATE(rcp_double);
RCP_ARRAY_TEMPLATE(rcp_float);

RCP_ARRAY_TEMPLATE(rcp_string);

RCP_ARRAY_TEMPLATE(rcp_ref);
RCP_ARRAY_TEMPLATE(rcp_pointer);
