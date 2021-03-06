from command_specification import * 

###
##generater constants
##

cBaseFileName = 'cmd_list'
outPutCHeader = open(cBaseFileName+'.h','w')
outPutCFile = open(cBaseFileName+'.c','w')

def printAsText(commandInfo):
	baseText ="""\
{name}
	{shortDescription}\
"""
	#print(baseText.format(**commandInfo))

def printAsLongText(commandInfo):
	params = ""
	if 'parameters' in commandInfo:
		pNames = map(lambda param : param['name'], commandInfo['parameters'])
#params = ' '.join(commandInfo['parameters'])
		params = ' '.join(pNames)
	
	baseText ="""\
{name} [{params}]
	{shortDescription}\
"""
	#print(baseText.format(params=params,**commandInfo))

sCommandList = sorted(commandList, key = lambda info : info['name'])
for info in commandList:
	printAsLongText(info);
###
#meta
###
def convertToMNames(info):
	macroName = '_'.join(re.findall(r'[A-Z]*[a-z]+',info['name'])).upper()
	return {
		'cmdName':info['name'],
		'defName':'CMD_'+macroName,
		'defStrName':'CMD_STR_'+macroName,
	}

def printAsCMacro(info,idx):
	tmp = convertToMNames(info)
	tmp['defIdx']=idx
	outPutCHeader.write("#define {defName}\t((rcp_command_type_t){defIdx})\n".format(** tmp))
	outPutCHeader.write("#define {defStrName}\t(\"{cmdName}\")\n".format(** tmp))
	
def printAsCTable(info):
	tmp = convertToMNames(info)
	outPutCFile.write("\t{defStrName},\n".format(** tmp))

#header
outPutCHeader.write("//This file was generated by program.\n")
outPutCHeader.write("typedef uint8_t rcp_command_type_t ;\n")
outPutCHeader.write("extern const char* rcp_command_str_table[];\n")
i = 0;
for info in sCommandList:
	printAsCMacro(info,i)
	i=i+1

outPutCHeader.write("#define CMD_INVALID {}\n".format(i))
outPutCHeader.write("#define CMD_COUNT {}\n".format(i))

#implement
outPutCFile.write("//This file was generated by program.\n")
outPutCFile.write('#include "rcp_pch.h"\n')
outPutCFile.write('#include "{}"\n'.format(cBaseFileName+'.h'))
outPutCFile.write("const char* rcp_command_str_table[]={\n")
for info in sCommandList:
	printAsCTable(info)
outPutCFile.write("};\n")
