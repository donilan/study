#!/usr/bin/env python

import socket, logging, thread, re

from smart import ConnectionHolder, Executor

logging.basicConfig(level=logging.DEBUG, format='%(name)s: %(message)s')
PORT = 19374
LOG = logging.getLogger()

def waitingClient(conn, addr):
    while True:
        data = conn.recv(1024)
        if not data: break
        for cmd in re.sub(r'([^\\]);', r'\1;;;', data).split(';;;'):
            LOG.debug(cmd)
            executeCommand(addr, cmd)

def executeCommand(addr, cmd):
    if cmd:
        who = cmd.split(' ')[0]
        cmd = cmd[len(who):].strip()
        if who == 'smart':
            Executor.serverExecute(addr, cmd)
            return
        conn = ConnectionHolder.get(who, None)
        if conn:
            conn.sendall(cmd)
        else:
            LOG.warning('client [%s] not found.' % who)
        
if __name__ == '__main__':
    ADDR = ('', PORT)
    conns = dict()
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(ADDR)
    s.listen(1)
    LOG.info('Server starting...')
    while True:
        conn, addr = s.accept()
        addr = "%s:%d" % addr
        print 'Connectd by', addr
        ConnectionHolder.add(addr, conn)
        thread.start_new_thread(waitingClient, (conn, addr))



