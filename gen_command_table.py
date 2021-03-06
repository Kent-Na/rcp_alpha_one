from command_specification import * 
from types.gen_type import * 

####
#files
####
cBaseFileName = 'cmd_table'
outPutCHeader = open(cBaseFileName+'.h','w')
outPutCFile = open(cBaseFileName+'.c','w')

outPutCHeader.write("//This file was generated by program.\n")
outPutCHeader.write("void rcp_command_table_init();")

###
#build main func
###

outPutCFile.write("//This file was generated by program.\n")
outPutCFile.write('#include "rcp_pch.h"\n')
outPutCFile.write('#include "cmd_impl.h"\n')
outPutCFile.write('#include "cmd_list.h"\n')
outPutCFile.write('#include "cmd_types_func.h"\n')
outPutCFile.write('#include "rcp_command.h"\n')
outPutCFile.write("struct rcp_command_core rcp_command_table[{}];\n".format(
			len(commandList)))

commandTableTemplate = """
void rcp_command_table_init(){{
{values}
}}
"""

template = """
	rcp_command_table[CMD_{command_id}].cmd = 
		CMD_{command_id};
	rcp_command_table[CMD_{command_id}].cmd_str = 
		"{name}";
	rcp_command_table[CMD_{command_id}].cmd_pms = 
		{pms};
	rcp_command_table[CMD_{command_id}].cmd_type= 
		cmd_{func_name}_type();
	rcp_command_table[CMD_{command_id}].cmd_impl= 
		cmd_impl_{func_name};
"""

permissionNumDict = {
	"login":1<<0,
	"read":1<<1,
	"write":1<<2,
	"permission":1<<3,
	"context":1<<4,
	"server":1<<31,
}

fragment = ""
for info in commandList:
	macroName = '_'.join(re.findall(r'[A-Z]*[a-z]+',info['name'])).upper()
	command_name= re.findall(r'[A-Z]*[a-z]+',info['name'])
	command_name= '_'.join(command_name).lower()
	permissionNum = 0
	if 'permission' in info:
		permissionNum = sum(map(
				lambda x:permissionNumDict[x],info['permission']))
	fragment += template.format(
			command_id = macroName,
			func_name = command_name,
			pms = permissionNum,
			**info
			)
outPutCFile.write(commandTableTemplate.format(values = fragment))

