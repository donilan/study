#!/usr/bin/env python

import socket, time

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

ip = socket.gethostbyname(socket.gethostname())
ip = '0.0.0.0'
s.bind((ip, 9000))
print 'listen', ip, '9000'

while 1:
    try:
        msg, addr = s.recvfrom(1024)
        print "%s from %s msg: %s" %(time.ctime(), addr, msg)
        s.sendto('[%s] %s' % (time.ctime(), msg), addr)
        print 'recv msg and return a msg back.'
    except:
        print 'Some error here.'
        break

s.close()
