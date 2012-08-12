import re

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
	"name":"open",
	"shortDescription":"",
	"longDescription":"",
	}
addParameter(command, "string", "version")
addParameter(command, "string", "client")

commandList.append(command)

commandList.append({
	"name":"kill",
	"shortDescription":"kill server process.",
	"longDescription":"",
	})

commandList.append({
	"name":"dump",
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
addParameter(command, "string", "username")
addParameter(command, "string", "password")

commandList.append(command)

command = {
	"name":"deleteUser",
	"shortDescription":"",
	"longDescription":"",
	}

addParameter(command, "string", "username")
addParameter(command, "string", "password")

commandList.append(command)
	
command = {
	"name":"loginUser",
	"shortDescription":"Login as specified username/password pair.",
	"longDescription":"",
	"possibleErrors":["Incorrect name or password"],
	"returnParameters":["userID","loginID"],
	}

addParameter(command, "string", "username")
addParameter(command, "string", "password")
commandList.append(command)

command = {
	"name":"addUser",
	"shortDescription":"Add specified user to loged in document.",
	"longDescription":"",
	}

addParameter(command, "string", "username")
commandList.append(command)

#removeUser
command = {
	"name":"removeUser",
	"shortDescription":"",
	"longDescription":"",
	}

addParameter(command, "string", "username")
commandList.append(command)

#Permission
command = {
	"name":"addPermission",
	"shortDescription":"",
	"parameters":[]
	}

addParameter(command, "string", "username")
addParameter(command, "string", "mode")

commandList.append(command)

command = {
	"name":"removePermission",
	"shortDescription":"",
	"parameters":[]
	}

addParameter(command, "string", "username")
addParameter(command, "string", "mode")

commandList.append(command)

###
#Context
##

##createContext
command = {
	"name":"addContext",
	"shortDescription":"",
	"longDescription":"",
	"possibleErrors":["Permission denied"],
	"returnParameters":["contextID"],
	}

addParameter(command, "string", "name")
commandList.append(command)

command = {
	"name":"removeContext",
	"shortDescription":"",
	"longDescription":"",
	"possibleErrors":["Permission denied"],
	}

addParameter(command, "string", "name")
commandList.append(command)

command = {
	"name":"loginContext",
	"shortDescription":"",
	"longDescription":"",
	"possibleErrors":["Context not found","Permission denied"],
	}

addParameter(command, "string", "name")
commandList.append(command)

commandList.append({
	"name":"*updateName",
	"shortDescription":"",
	"longDescription":"",
	})

#Value
command = {
	"name":"sendValue",
	"shortDescription":"Send value to all users in the context.",
	"longDescription":"",
	}

addParameter(command, "ref", "value")
addParameter(command, "string", "type")
commandList.append(command)

command = {
	"name":"setValue",
	"shortDescription":"Set value to specified variable.",
	"longDescription":"Old values will be replaced.",
	}

addParameter(command, "ref", "path")
addParameter(command, "ref", "value")
addParameter(command, "string", "type")
commandList.append(command)

command = {
	"name":"unsetValue",
	"shortDescription":"Delete value from container like an array or a map",
	"longDescription":"",
	}
addParameter(command, "ref", "path")
commandList.append(command)

command = {
	"name":"appendValue",
	"shortDescription":"Add value into container like an array or a map",
	"longDescription":"",
	}

addParameter(command, "ref", "path")
addParameter(command, "ref", "value")
addParameter(command, "string", "type")
commandList.append(command)

#Type
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

command = {
	"name":"addType",
	"shortDescription":"create new type and add to current context",
	"longDescription":"current implement only support dict(string,ref).",
	}

addParameter(command, "string", "name")
commandList.append(command)

#Error
command = {
	"name":"error",
	"shortDescription":"Send from server when fail to execute command.",
	"longDescription":"",
	}

addParameter(command, "string", "cause")
addParameter(command, "string", "reason")
commandList.append(command)

command = {
	"name":"caution",
	"shortDescription":"",
	"longDescription":"",
	}
addParameter(command, "string", "cause")
addParameter(command, "string", "reason")
commandList.append(command)

command = {
	"name":"info",
	"shortDescription":"",
	"longDescription":"Send from server when someting important but not faital thing happened.",
	}
addParameter(command, "string", "info")
addParameter(command, "string", "cause")
commandList.append(command)
