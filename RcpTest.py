from RcpConnection import *

c = RcpConnection()
c.connectToDefaultServer()
c.run()
con = {'command':'loginContext'}
c.sendCommand(con)
con = {
	'command':'addRecord',
	'type':'uint32',
	'value':100,
	'recordID':100}
c.sendCommand(con)
con = {
	'command':'addRecord',
	'type':'string',
	'value':'message',
	'recordID':101}
c.sendCommand(con)
con = {'command':'kill'}
c.sendCommand(con)
c.receiveCommand()
