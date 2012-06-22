from RcpConnection import *

c = RcpConnection()
c.connectToDefaultServer()
c.run()
con = {'command':'loginContext'}
c.sendCommand(con)
con = {
	'command':'sendValue',
	'type':'uint32',
	'value':100.1234}
c.sendCommand(con)
con = {
	'command':'sendValue',
	'type':'string',
	'value':'message'}
c.sendCommand(con)
con = {
	'command':'setValue',
	'value':[0,1,2,3,4,5],
	'path':102}
#c.sendCommand(con)
con = {'command':'dump'}
#c.sendCommand(con)
c.receiveCommand()
c.receiveCommand()
c.receiveCommand()
c.receiveCommand()
