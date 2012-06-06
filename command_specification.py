import re

###
##generater constants
##

cBaseFileName = 'rcp_command_list'
outPutCHeader = open(cBaseFileName+'.h','w')
outPutCFile = open(cBaseFileName+'.c','w')

###
##helper func
##
def addParameter(command, cType, name, pType = "default"):
	if not "parameters" in command:
		command["parameters"] = []
	parameter = {
		"type":cType,
		"name":name,
	}
	command["parameters"].append(parameter)


###
#command def
##

commandList = []
#Connection

command = {
	"name":"protocol",
	"shortDescription":"",
	"longDescription":"",
	}
addParameter(command, "const char*", "version")
addParameter(command, "const char*", "client")

commandList.append(command)

commandList.append({
	"name":"kill",
	"shortDescription":"kill server process.",
	"longDescription":"",
	})

commandList.append({
	"name":"close",
	"shortDescription":"",
	"longDescription":"",
	})

commandList.append({
	"name":"ping",
	"shortDescription":"Test the connection is alive or not",
	"longDescription":"",
	})

commandList.append({
	"name":"pong",
	"shortDescription":"Responce of command \"ping\"",
	"longDescription":"",
	})

#User
command = {
	"name":"createUser",
	"shortDescription":"",
	"longDescription":"",
	"returnParameters":["userID"],
	}
addParameter(command, "const char*", "username")
addParameter(command, "const char*", "password")

commandList.append(command)

command = {
	"name":"deleteUser",
	"shortDescription":"",
	"longDescription":"",
	}

addParameter(command, "const char*", "username")
addParameter(command, "const char*", "password")

commandList.append(command)
	
commandList.append({
	"name":"loginUser",
	"shortDescription":"Login as specified username/password pair.",
	"longDescription":"",
	"possibleErrors":["Incorrect name or password"],
	"returnParameters":["userID","loginID"],
	})

addParameter(command, "const char*", "username")
addParameter(command, "const char*", "password")

command = {
	"name":"addUser",
	"shortDescription":"Add specified user to loged in document.",
	"longDescription":"",
	}

addParameter(command, "const char*", "username")
commandList.append(command)

#removeUser
command = {
	"name":"removeUser",
	"shortDescription":"",
	"longDescription":"",
	}

addParameter(command, "const char*", "username")
commandList.append(command)

#updateUserPermission
command = {
	"name":"updateUserPermission",
	"shortDescription":"",
	"parameters":[]
	}

addParameter(command, "const char*", "username")
addParameter(command, "const char*", "mode")

commandList.append(command)

###
#Context
##

##createContext
command = {
	"name":"createContext",
	"shortDescription":"",
	"longDescription":"",
	"possibleErrors":["Permission denied"],
	"returnParameters":["contextID"],
	}

#addParameter(command, "uint32_t", "contextID")
commandList.append(command)

command = {
	"name":"deleteContext",
	"shortDescription":"",
	"longDescription":"",
	"possibleErrors":["Permission denied"],
	}

addParameter(command, "uint32_t", "contextID")
commandList.append(command)

command = {
	"name":"loginContext",
	"shortDescription":"",
	"longDescription":"",
	"possibleErrors":["Context not found","Permission denied"],
	}

addParameter(command, "uint32_t", "contextID")
commandList.append(command)

commandList.append({
	"name":"*updateName",
	"shortDescription":"",
	"longDescription":"",
	})

commandList.append({
	"name":"*updateUserList",
	"shortDescription":"",
	"longDescription":"",
	})

#Record
command = {
	"name":"setValue",
	"shortDescription":"set value to specified variable",
	"longDescription":"",
	}

addParameter(command, "ref", "path")
addParameter(command, "ref", "value")
addParameter(command, "string", "type")
commandList.append(command)


command = {
	"name":"unsetValue",
	"shortDescription":"delete value from container like an array or a map",
	"longDescription":"",
#"parameters":["recordID","struct","values"],
	}
addParameter(command, "ref", "path")
commandList.append(command)

command = {
	"name":"appendValue",
	"shortDescription":"add value into container like an array or a map",
	"longDescription":"",
	}

addParameter(command, "ref", "path")
addParameter(command, "ref", "value")
addParameter(command, "string", "type")
commandList.append(command)

commandList.append({
	"name":"addRecord",
	"shortDescription":"",
	"longDescription":"",
#"parameters":["recordID","struct","values"],
	})

#addParameter(command, "uint32_t", "recordID")
#addParameter(command, "any", "value")
#addParameter(command, "uint32_t", "recordID")

commandList.append({
	"name":"updateRecord",
	"shortDescription":"",
	"longDescription":"",
#"parameters":["recordID","values"],
	})

commandList.append({
	"name":"removeRecord",
	"shortDescription":"",
	"longDescription":"",
#"parameters":["recordID"],
	})

#RecordID
commandList.append({
	"name":"reserveRecordID",
	"shortDescription":"Reserve record id for furser use.",
	"longDescription":"",
#"parameters":["recordIDs"],
	})

commandList.append({
	"name":"releaseRecordID",
	"shortDescription":"",
	"longDescription":"",
#"parameters":["recordIDs"],
	})

#Struct
commandList.append({
	"name":"createStruct",
	"shortDescription":"",
	"longDescription":"",
	})

commandList.append({
	"name":"addStruct",
	"shortDescription":"",
	"longDescription":"",
	})

#Error
command = {
	"name":"error",
	"shortDescription":"Send from server when fail to execute command.",
	"longDescription":"",
	}

addParameter(command, "const char*", "cause")
addParameter(command, "const char*", "reason")
commandList.append(command)

commandList.append({
	"name":"caution",
	"shortDescription":"",
	"longDescription":"",
	})

commandList.append({
	"name":"note",
	"shortDescription":"",
	"longDescription":"Send from server when someting important but not faital thing happened.",
	})

def printAsText(commandInfo):
	baseText ="""\
{name}
	{shortDescription}\
"""
	print(baseText.format(**commandInfo))

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
	print(baseText.format(params=params,**commandInfo))

sCommandList = sorted(commandList, key = lambda info : info['name'])
for info in sCommandList:
	printAsLongText(info);
###
#meta
###
def convertToMNames(info):
	macroName = '_'.join(re.findall(r'[A-Z]*[a-z]+',info['name'])).upper()
	return {
		'cmdName':info['name'],
		'defName':'RCP_COMMAND_'+macroName,
		'defStrName':'RCP_COMMAND_STR_'+macroName,
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

outPutCHeader.write("#define RCP_COMMAND_INVALID {}\n".format(i))
outPutCHeader.write("#define RCP_COMMAND_COUNT {}\n".format(i))

#implement
outPutCFile.write("//This file was generated by program.\n")
outPutCFile.write('#include "rcp_pch.h"\n')
outPutCFile.write('#include "{}"\n'.format(cBaseFileName+'.h'))
outPutCFile.write("const char* rcp_command_str_table[]={\n")
for info in sCommandList:
	printAsCTable(info)
outPutCFile.write("};\n")
