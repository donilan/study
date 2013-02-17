import socket, logging, threading, re, errno
from smart import Context, CommandExecutor
import smart.const

class Connections(object):
    
    def __init__(self, log=logging.getLogger('Connections')):
        self._conns = dict()
        self._alias = dict()
        self.log = log

    def alias(self, name, alias):
        self._alias[alias] = name

    def add(self, name, conn):
        self._conns[name] = conn

    def remove(self, name):
        del self._conns[name]

    def get(self, name, _default=None):
        host = self._alias.get(name, None)
        if host:
            self.log.debug('Found host [%s] alias [%s]' % (host, name))
            name = host
        return self._conns.get(name, _default)

    def send(self, name, data):
        con = self.get(name)
        if con:
            self.log.debug('Send data [%s] to [%s]' % (data, name))
            con.sendall(data)
            return con.recv(1024)
        else:
            self.log.debug('Connection not found for [%s]' % (name))

    def items(self):
        return self._conns.items()


class SmartServer(object):
    
    def __init__(self, hostname=smart.const.LISTEN
                 , cmdPort = smart.const.CMD_PORT
                 , conns = Connections()
                 , log = logging.getLogger('SmartServer')):
        self.cmdPort = cmdPort
        self.hostname = hostname
        self.conns = Connections()
        self.log = log
        self.stopEvent = threading.Event()

    def run(self):
        cmdSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        cmdSocket.bind((self.hostname, self.cmdPort))
        cmdSocket.listen(1)
        self.log.info('Server %s listening on %d starting...' 
                      % (self.hostname, self.cmdPort))

        while True:
            try:
                conn, addr = cmdSocket.accept()
            except KeyboardInterrupt:
                break
            addr = "%s:%d" % addr
            self.log.info('Connectd by %s' % (addr))
            self.conns.add(addr, conn)
            t = threading.Thread(target=self.handleClient, args=(conn, addr))
            t.start()


        self.stopEvent.set()
        cmdSocket.close()
        self.log.info('User quit.')
        
    def handleClient(self, conn, addr):
        while not self.stopEvent.is_set():
            try:
                data = conn.recv(1024)
            except socket.error, v:
                self.log.info('Host [%s] closed.' % addr)
                self.conns.remove(addr)
                break
            if not data: 
                self.log.info('Host [%s] closed.' % addr)
                self.conns.remove(addr)
                break
            data = data.replace('\n', '')
            if not data:
                return ''
            for cmd in re.sub(r'([^\\]);', r'\1;;;', data).split(';;;'):
                context = Context(cmd)
                context.client = addr
                context.connections = self.conns
                result = self.executeCommand(context)
                if result:
                    conn.sendall(result+'\n')
                else:
                    conn.sendall('Success but no result!\n')

    def executeCommand(self, context):
        self.log.debug("[%s] execute command: '%s' with param '%s' to [%s]" 
                       % (context.client, context.cmdName
                          , context.param, context.target))
        if context.target == 'smart':
            return CommandExecutor(context).run()
        elif context.target == 'all':
            for host, con in self.conns.items():
                if host != context.client:
                    return self.conns.send(host, context.orgCmd)
        else:
            return self.conns.send(context.target, context.orgCmd)


