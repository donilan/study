#!/usr/bin/env python

import socket, thread

def recvData(s):
    while 1:
        print s.recv(1024)


ADDR = ('127.0.0.1', 19374)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(ADDR)
thread.start_new_thread(recvData, (s,))
while 1:
    msg = raw_input('')
    s.sendall(msg)
    
s.close()
    
