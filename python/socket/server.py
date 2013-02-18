#!/usr/bin/env python

import socket, thread

close = False

def accept(sock):
    print 'starting...'
    while True:
        conn, addr = sock.accept()
        if close:
            print 'Executing close...'
            sock.close()
            break
        print 'accept'


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
listenAddr = ('127.0.0.1', 8081)
s.bind(listenAddr)
s.listen(1)

thread.start_new_thread(accept, (s,))
print s.getsockname()
try:
    while True:
        raw_input('')

except KeyboardInterrupt:
    close = True
    socket.socket(socket.AF_INET, socket.SOCK_STREAM).connect(listenAddr)
    


print 'shutdown'
