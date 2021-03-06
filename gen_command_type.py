from command_specification import * 
from rcp_core.gen_type import * 

####
#files
####
cBaseFileName = 'cmd_types'
outPutCHeader = open(cBaseFileName+'.h','w')
outPutCFile = open(cBaseFileName+'.c','w')

extHeaderFileName = 'cmd_types_func'
outPutExtHeader = open(extHeaderFileName+'.h','w')

####
#templates
####
structTemplte = """
struct {c_struct_name}{{
	rcp_record_ref command;
	uint16_t loginID;
{c_fields}\
}};
"""

fieldTemplate = """\
	{c_type} {c_field_name};
"""

genFuncTemplate = """
rcp_type_ref cmd_{command_name}_type(){{
	{gen_core}
	return s_type;
}}
"""

genFuncCoreTemplate = """
	rcp_type_ref s_type = rcp_struct_type_new({param_count});
	struct rcp_type_core* t_core = (struct rcp_type_core*)s_type; 
	t_core->size = sizeof (struct {c_struct_name});
	t_core->type_id = 0x100;
	t_core->type_name = NULL;
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copied = NULL;
	t_core->compare = NULL;
	t_core->write_json = rcp_struct_write_json;

	struct rcp_type_struct_ext* s_core = 
		(struct rcp_type_struct_ext*)(t_core+1);
	s_core->name = rcp_string_new("{revert_domain_name}");
	s_core->param_count = {param_count};

	struct rcp_struct_param_core *param = 
		(struct rcp_struct_param_core*)(s_core+1);
"""

genFuncParamTemplate = """
	param->name = rcp_string_new("{c_field_name}");
	param->type = {rcp_type_name};
	param->offset = offsetof(struct {c_struct_name}, {c_field_name});
	param ++;
"""

###
#build struct def
###


for info in commandList:
	fragment = "";
	if 'parameters' in info:
		for param in info['parameters']:
			fragment += fieldTemplate.format(
					c_type = typeDict[param['type']]['cTypeName'],
					c_field_name = param['name']) 
	c_struct_name = re.findall(r'[A-Z]*[a-z]+',info['name'])
	c_struct_name = 'cmd_' + '_'.join(c_struct_name).lower()
	fragment = structTemplte.format(
			c_struct_name = c_struct_name, 
			c_fields = fragment)
	outPutCHeader.write(fragment)

###
#build type def
###

outPutCFile.write('//This file was generated by "gen_command_type.py"\n')
outPutCFile.write('#include "rcp_pch.h"\n')
outPutCFile.write('#define RCP_INTERNAL_STRUCTURE\n')
outPutCFile.write('#include "rcp_core/rcp_type_list.h"\n')
outPutCFile.write('#include "rcp_core/rcp_string.h"\n')
outPutCFile.write('#include "rcp_core/rcp_struct.h"\n')
outPutCFile.write('#include "rcp_core/rcp_type.h"\n')
outPutCFile.write('#include "cmd_list.h"\n')
outPutCFile.write('#include "cmd_types.h"\n')

for info in commandList:
	addParameter(info, "string", "command")
	addParameter(info, "uint16", "loginID")

	fragment = ""
	c_struct_name = re.findall(r'[A-Z]*[a-z]+',info['name'])
	c_struct_name = 'cmd_' + '_'.join(c_struct_name).lower()

	command_name= re.findall(r'[A-Z]*[a-z]+',info['name'])
	command_name= '_'.join(command_name).lower()
	fragment = genFuncCoreTemplate.format(
			param_count = len(info['parameters']),
			c_struct_name = c_struct_name,
			revert_domain_name = "ttt")
	for param in sorted(info['parameters'], key = lambda i:i['name']):
		if param['type'] == 'int64':
			rcp_type_name = 'rcp_int64_type'
		elif param['type'] == 'uint16':
			rcp_type_name = 'rcp_uint16_type'
		else:
			rcp_type_name = 'rcp_ref_type'
		fragment += genFuncParamTemplate.format(
				c_field_name = param['name'],
				rcp_type_name = rcp_type_name,
				c_struct_name = c_struct_name)
	fragment = genFuncTemplate.format(
			command_name = command_name,
			gen_core = fragment)
	outPutCFile.write(fragment);


outPutExtHeader.write('//This file is generated by "gen_command.py"\n')
outPutExtHeader.write('#include "def/rcp_type.h"\n')
template = "rcp_type_ref cmd_{command_name}_type();\n"
for info in commandList:
	command_name= re.findall(r'[A-Z]*[a-z]+',info['name'])
	command_name= '_'.join(command_name).lower()
	outPutExtHeader.write(template.format(
				command_name = command_name));
