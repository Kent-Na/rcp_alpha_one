from RcpConnection import *

c = RcpConnection();
c.connectToDefaultServer();
con = {'command':'loginContext'}
c.sendCommand(con);
con = {'command':'addRecord', 'type':'uint32', 'value':100}
c.sendCommand(con);
con = {'command':'addRecord', 'type':'string', 'value':'message'}
c.sendCommand(con);
con = {'command':'kill'}
c.sendCommand(con);
c.close();
