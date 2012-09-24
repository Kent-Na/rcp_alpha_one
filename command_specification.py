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

def requirePermission(command, name):
	if not "permission" in command:
		command["permission"] = []
	command["permission"].append(name)

###
#command def
##

commandList = []
#Connection

command = {
	"name":"open",
	"shortDescription":"Client must send this when connect to server.",
	"longDescription":"",
	}
addParameter(command, "string", "protocol")
addParameter(command, "string", "client")

commandList.append(command)

commandList.append({
	"name":"kill",
	"shortDescription":"Stop listening new connection.",
	"longDescription":"",
	})

commandList.append({
	"name":"dump",
	"shortDescription":"Backup all context's data to database.",
	"longDescription":"",
	})

commandList.append({
	"name":"load",
	"shortDescription":"Don't use this.",
	"longDescription":"",
	})

commandList.append({
	"name":"close",
	"shortDescription":"Request server to close connection.",
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
	"shortDescription":"Create new user account.",
	"longDescription":"",
	}
addParameter(command, "string", "username")
addParameter(command, "string", "password")

commandList.append(command)

command = {
	"name":"deleteUser",
	"shortDescription":"Not yet implement.",
	"longDescription":"",
	}

addParameter(command, "string", "username")
addParameter(command, "string", "password")

commandList.append(command)
	
command = {
	"name":"loginUser",
	"shortDescription":"Login as specified username/password pair.",
	"longDescription":"",
	}

addParameter(command, "string", "username")
addParameter(command, "string", "password")
commandList.append(command)

command = {
	"name":"addUser",
	"shortDescription":"Send from server when new user are logged in.",
	"longDescription":"",
	}

addParameter(command, "string", "username")
requirePermission(command, "server")
commandList.append(command)

command = {
	"name":"removeUser",
	"shortDescription":"Send from server when new user are logged out.",
	"longDescription":"",
	}

addParameter(command, "string", "username")
requirePermission(command, "server")
commandList.append(command)

#Permission
command = {
	"name":"setPermission",
	"shortDescription":"Set users permission to specified mode list.",
	"longDescription":"",
	}

addParameter(command, "string", "username")
addParameter(command, "array", "mode")
requirePermission(command, "permission")

commandList.append(command)

command = {
	"name":"unsetPermission",
	"shortDescription":"Make user to use default permission.",
	"longDescription":"",
	}

addParameter(command, "string", "username")
requirePermission(command, "permission")

commandList.append(command)

###
#Context
##

##createContext
command = {
	"name":"addContext",
	"shortDescription":"Add new sub context to current context.",
	"longDescription":"",
	}

addParameter(command, "string", "name")
requirePermission(command, "context")
commandList.append(command)

command = {
	"name":"removeContext",
	"shortDescription":"Not yet implemented.",
	"longDescription":"",
	}

addParameter(command, "string", "name")
requirePermission(command, "context")
commandList.append(command)

command = {
	"name":"loginContext",
	"shortDescription":"Login to specified sub context.",
	"longDescription":"",
	}

addParameter(command, "string", "name")
#This command doesn't require "login" permission at current context.
commandList.append(command)

command = {
	"name":"logoutContext",
	"shortDescription":"Login to parent context.",
	"longDescription":"",
	}

commandList.append(command)

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
requirePermission(command, "write")
commandList.append(command)

command = {
	"name":"unsetValue",
	"shortDescription":"Delete value from container like a dict",
	"longDescription":"",
	}
addParameter(command, "ref", "path")
requirePermission(command, "write")
commandList.append(command)

command = {
	"name":"appendValue",
	"shortDescription":"Add value into container like an array or a map",
	"longDescription":"",
	}

addParameter(command, "ref", "path")
addParameter(command, "ref", "value")
addParameter(command, "string", "type")
requirePermission(command, "write")
commandList.append(command)

#Type
commandList.append({
	"name":"createStruct",
	"shortDescription":"Not yet implemented.",
	"longDescription":"",
	})

commandList.append({
	"name":"addStruct",
	"shortDescription":"Not yet implemented.",
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
	"name":"fatal",
	"shortDescription":"Sent from server when unable to continue.",
	"longDescription":"",
	}

addParameter(command, "string", "cause")
addParameter(command, "string", "description")
requirePermission(command, "server")
commandList.append(command)

command = {
	"name":"error",
	"shortDescription":"Sent from server when fail to execute command.",
	"longDescription":"",
	}

addParameter(command, "string", "cause")
addParameter(command, "string", "description")
requirePermission(command, "server")
commandList.append(command)

command = {
	"name":"caution",
	"shortDescription":"Sent from server when some probrem happend.",
	"longDescription":"",
	}
addParameter(command, "string", "cause")
addParameter(command, "string", "description")
requirePermission(command, "server")
commandList.append(command)

command = {
	"name":"info",
	"shortDescription":"Sent from server when someting happened.",
	"longDescription":"",
	}
addParameter(command, "string", "cause")
addParameter(command, "string", "description")
requirePermission(command, "server")
commandList.append(command)
