#!/usr/bin/env python

import socket, logging, thread

#logging.basicConfig(level=logging.DEBUG, format='%(name)s: %(message)s')

def recvData(conn, addr):
    while 1:
        data = conn.recv(1024)
        if not data: break
        for k, v in conns.items():
            if k != addr:
                v.sendall("%s: %s\n" %(addr, data))


ADDR = ('', 19374)
conns = dict()
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(ADDR)
s.listen(1)
while 1:
    conn, addr = s.accept()
    addr = "%s:%d" % addr
    print 'Connectd by', addr
    conns[addr] = conn
    thread.start_new_thread(recvData, (conn, addr))


