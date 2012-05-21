import socket
import random
import json
import thread


class RcpDocumentInfo:
	def __init__(self):
		self.name = ""
		self.lastUpdateTime = ""

class RcpConnection:
	def __init__(self):
		self.mainSocket = None
		self.typeTable = {}
		self.objects = {}
		self.documentList = []
		self.documentIDTable = {}
		self.userList = []
		self.userIDTable = {}
		self.remain = ""
#will be deprecated
		self.objectsArray = []
		self.outputDebug = True 
		
	def connectToDefaultServer(self):
#host = "www.tuna-cat.com"
#port = 2189
#host = '10.0.1.17'
		host = 'localhost'
		port = 4000
		self.mainSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.mainSocket.connect((host,port))
	
	def close():
		self.mainSocket.close()

	def run(self):
		thread.start_new_thread(self.mainLoop,())
	
	def mainLoop(self):
		while True:
			self.receiveCommand()

#class must implement "executeCommand" method
	def bindClassToType(self,classObject,typeName):
		self.typeTable[typeName]=classObject

	def generateObjectID(self):
		while True:
			newID = random.uniform(0,4096*4096-1)	
			if newID in objects:
				return newID

	def executeCommand(self,command):
		if not "command" in command:
			return
		if command["command"]=="createUser":
			userID = command["userID"]
			if userID == 0:
				self.didFailRegist()
			else:
				self.didSucceedRegist()
		if command["command"] == "document":
			dinfo = RcpDocumentInfo()
			dinfo.name = command["name"]
			dinfo.lastUpdateTime = command["lastUpdateTime"]
			self.documentIDTable[command["documentID"]] = dinfo
			self.didUpdateContents()
		if command["command"] == "user":
			self.userIDTable[command["userID"]] = command["name"]	
			self.didUpdateContents()
		if command["command"] == "updateDocumentList":
			self.documentList = command["documentIDs"]
			for ID in self.documentList:
				if not ID in self.documentIDTable:
					sendCommand = {}
					sendCommand["command"] = "document"
					sendCommand["documentID"] = ID
					self.sendCommand(sendCommand)
			self.didUpdateContents()
		if command["command"] == "updateUserList":
			self.userList = command["userIDs"]
			for ID in self.userList:
				if not ID in self.userIDTable:
					sendCommand = {}
					sendCommand["command"] = "user"
					sendCommand["userID"] = ID
					self.sendCommand(sendCommand)
			self.didUpdateContents()
		if command["command"] == "addObject":
			type = command["type"]
			objectID = command["objectID"]
			if not type in self.typeTable:
				print ("fatal error")
				return
				exit()
			newObject = self.typeTable[type](objectID)
			newObject.executeCommand(command)
			self.objects[objectID] = newObject
#will be deprecated
			self.objectsArray.append(newObject)
			self.didUpdateContents()
		if command["command"]=="updateObject":
			objectID = command["objectID"]
			if not objectID in self.objects:
				print ("fatal error")
				return
				exit()
			object = self.objects[objectID]
			object.executeCommand(command)
			self.didUpdateContents()
		if command["command"]=="removeObject":
#will be deprecated
			self.objectsArray.append(self.objects[objectID])
			del self.objects[objectID]
			self.didUpdateContents()
			

#input: command dictionaly
	def sendCommand(self,command):
		commandString = json.dumps(command)
		if self.outputDebug:
			print("->"+commandString);
		commandString += "\0"
		self.mainSocket.send(commandString)

	def receiveCommand(self):
		new = self.mainSocket.recv(1024)
		if new == "":
			if self.outputDebug:
				print("connection closed")
			self.didLostConnection()
		rawdat = self.remain + new
		strings = rawdat.split("\0");
		for element in strings[:-1]:
			if self.outputDebug:
				print("<-"+element)
			command = json.loads(element)
			self.executeCommand(command)
		self.remain = strings[-1]

#input: command dictionaly
	def executeAndSendCommand(self,command):
		self.executeCommand()
		self.sendCommand()

	def userNameList(self):
		names = []
		for ID in self.userList:
			if ID in self.userIDTable:
				names.append(self.userIDTable[ID])
			else:
				names.append(str(ID))
		return names

	def objectForID(self,objectID):
		return objects[objectID]	

#This method will be deplecated.
	def objectAtIndex(send):
		return None

#command sending
	def protocol(self):
		command = {}
		command["command"]="protocol"
		command["protocol"]="RCP-ALPHA"
		command["client"]="Python-RCP"
		self.sendCommand(command)

	def createUser(self,username,password):
		command = {}
		command["command"]="createUser"
		command["name"]=username
		command["password"]=password
		self.sendCommand(command)

	def loginUser(self,username,password):
		command = {}
		command["command"]="loginUser"
		command["name"]=username
		command["password"]=password
		self.sendCommand(command)

	def createDocument(self):
		command = {}
		command["command"]="createDocument"
		self.sendCommand(command)

	def loginDocument(self,documentID):
		command = {}
		command["command"]="loginDocument"
		command["documentID"]=documentID
		self.sendCommand(command)

	def loginDocumentListDocument(self):
		command = {}
		command["command"]="loginDocumentListDocument"
		self.sendCommand(command)
	
	def updateName(self,name):
		command = {}
		command["command"] = "updateName"
		command["name"] = name
		self.sendCommand(command)

#events (these are never called except "didUpdateContents" because did not implemented...)
	def didConnectToServer(send):
		pass
	def didLostConnection(send):
		pass

#implemented
	def didSucceedRegist(self):
		pass
#implemented
	def didFailRegist(self):
		pass

	def didSucceedLogin(send):
		pass
	def didFailLogin(send):
		pass

	def didReceiveUserList(send):
		pass
	def didExecuteCommand(send):
		pass

	def willUpdateContents(send):
		pass
#implemented
	def didUpdateContents(self):
		pass
	def willAddObject(send):
		pass
	def willRemoveObject(send):
		pass
	def willRemoveAllObjects(send):
		pass
