from RcpConnection import *

c = RcpConnection()
c.connectToDefaultServer()
c.run()
con = {
	'command':'createUser',
	'username':'cz',
	'password':'nk'
}
c.sendCommand(con)
con = {
	'command':'loginUser',
	'username':'cz',
	'password':'nk'
}
c.sendCommand(con)
"""
con = {
	'command':'loginUser',
	'username':'black',
	'password':'bbbox'
	}
c.sendCommand(con)
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
	'command':'appendValue',
	'value':'test',
	'path':102}
#c.sendCommand(con)
con = {
	'command':'addPermission',
	'username':'black'}
c.sendCommand(con)
con = {
	'command':'removePermission'}
c.sendCommand(con)
con = {'command':'dump'}
c.sendCommand(con)
c.receiveCommand()
c.receiveCommand()
c.receiveCommand()
c.receiveCommand()
"""
