import socket, logging, thread, re, errno
from smart import Context, CommandExecutor

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
    
    def __init__(self, hostname='0.0.0.0'
                 , port=19374, conns=Connections()
                 , log = logging.getLogger('SmartServer')):
        self.port = port
        self.hostname = hostname
        self.conns = Connections()
        self.log = log

    def run(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((self.hostname, self.port))
        s.listen(1)
        self.log.info('Server %s listening on %d starting...' 
                      % (self.hostname, self.port))
        while True:
            conn, addr = s.accept()
            addr = "%s:%d" % addr
            self.log.info('Connectd by %s' % (addr))
            self.conns.add(addr, conn)
            thread.start_new_thread(self.handleClient, (conn, addr))
        
    def handleClient(self, conn, addr):
        while True:
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


