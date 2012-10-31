from command_specification import * 
from types.gen_type import * 

####
#files
####
outPutFile= open('Commander.py','w')


###
#build main func
###

template = """
	def {name}(self,{arg_list}):
		command = {{}}
		command["command"]="{name}"
		{param_list}
		self.tgt.sendCommand(command)
"""

param_template = """
		command["{name}"]={name}\
"""

fragment = """
import json
class Commander:
"""

for info in commandList:
	param_list = "" 
	arg_list = ""
	if "parameters" in info:
		for param_info in info["parameters"]:
			param_list += param_template.format(**param_info)
		arg_list = ','.join(map(lambda x:x['name'],info["parameters"]))

	fragment += template.format(param_list=param_list,
			name=info['name'], arg_list=arg_list)

outPutFile.write(fragment)

