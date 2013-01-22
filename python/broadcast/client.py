#!/usr/bin/env python

import socket, time

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

while 1:
    s.sendto('Hello world', ('<broadcast>', 9000))
    print 'sended.'
    time.sleep(1)
s.close()
