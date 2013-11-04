
typeList = [
	'type',
	'data',
	'record',
	
	'tree',

	'string',

	'dict',
	'dict_type',
	'dict_node',

	'array',
	'array_iterater',

	'struct',
	'struct_param',
]

template = """
#ifndef RCP_DEF_{capitalName}
#define RCP_DEF_{capitalName}
typedef struct rcp_{name}_core *rcp_{name}_ref;
#endif
"""

for name in typeList:
	with open('rcp_{name}.h'.format(name = name),'w') as f:
		f.write(template.format(name = name, capitalName = name.upper()))

