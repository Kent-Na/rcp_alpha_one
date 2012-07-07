from command_specification import * 
from gen_type import * 

####
#files
####
cBaseFileName = 'cmd_types'
outPutCHeader = open(cBaseFileName+'.h','w')
outPutCFile = open(cBaseFileName+'.c','w')

####
#templates
####
structTemplte = """
struct {c_struct_name}{{
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
	t_core->type_name = "{rcp_type_name}";
	t_core->init = rcp_struct_init;
	t_core->deinit = rcp_struct_deinit;
	t_core->copy = NULL;
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
	param->type = rcp_ref_type;
	param->offset = offsetof(struct {c_struct_name}, {c_field_name});
	param ++;
"""

###
#build struct def
###

outPutCHeader.write('#include "def/rcp_type.h"\n')
outPutCHeader.write('#include "def/rcp_record.h"\n')

for info in commandList:
	addParameter(info, "string", "command")

for info in commandList:
	if not 'parameters' in info:
		continue
	fragment = "";
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
	print(fragment)

outPutCHeader.write("""
rcp_type_ref rcp_command_type(rcp_command_type_t command_id);
""")
outPutCHeader.write("""
void rcp_command_type_table_init();
""")
###
#build type def
###

outPutCFile.write('//This file is generated by "gen_command.py"\n')
outPutCFile.write('#include "rcp_pch.h"\n')
outPutCFile.write('#include "rcp_utility.h"\n')
outPutCFile.write('#include "rcp_tree.h"\n')
outPutCFile.write('#include "rcp_command_list.h"\n')
outPutCFile.write('#include "rcp_json_write.h"\n')
outPutCFile.write('#define RCP_INTERNAL_STRUCTURE\n')
outPutCFile.write('#include "types/rcp_type_list.h"\n')
outPutCFile.write('#include "types/rcp_string.h"\n')
outPutCFile.write('#include "types/rcp_struct.h"\n')
outPutCFile.write('#include "rcp_type.h"\n')
outPutCFile.write('#include "cmd_types.h"\n')

for info in commandList:
	if not 'parameters' in info:
		continue
	fragment = ""
	c_struct_name = re.findall(r'[A-Z]*[a-z]+',info['name'])
	c_struct_name = 'cmd_' + '_'.join(c_struct_name).lower()

	command_name= re.findall(r'[A-Z]*[a-z]+',info['name'])
	command_name= '_'.join(command_name).lower()
	fragment = genFuncCoreTemplate.format(
			param_count = len(info['parameters']),
			c_struct_name = c_struct_name,
			rcp_type_name = "ttt",
			revert_domain_name = "ttt")
	for param in sorted(info['parameters'], key = lambda i:i['name']):
		fragment += genFuncParamTemplate.format(
				c_field_name = param['name'],
				c_struct_name = c_struct_name)
	fragment = genFuncTemplate.format(
			command_name = command_name,
			gen_core = fragment)
	print(fragment)
	outPutCFile.write(fragment);

###
#build main func
###

outPutCFile.write("rcp_type_ref rcp_command_type_table[{}];\n".format(
			len(commandList)))
typeTableTemplate = """
void rcp_command_type_table_init(){{
{values}
}}
"""

template = """
	rcp_command_type_table[CMD_{command_id}] = 
		cmd_{func_name}_type();\
"""

fragment = ""
for info in commandList:
	macroName = '_'.join(re.findall(r'[A-Z]*[a-z]+',info['name'])).upper()
	command_name= re.findall(r'[A-Z]*[a-z]+',info['name'])
	command_name= '_'.join(command_name).lower()
	fragment += template.format(
			command_id = macroName,
			func_name = command_name
			)
outPutCFile.write(typeTableTemplate.format(values = fragment))


outPutCFile.write("""
rcp_type_ref rcp_command_type(rcp_command_type_t command_id){
	return rcp_command_type_table[command_id];
}
""")
