import socket, logging, threading, re, errno, time
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

    def sendLog(self, target, msg, origin = smart.const.SERVER_ALIAS, logType='info'):
        self.send(Context('%s log %s %s for %s' %(target, logType, msg, origin)))

    def send(self, context):
        con = self.get(context.target)
        if con:
            self.log.debug('Send data [%s] to [%s]' % (context.cmd(), context.target))
            con.sendall(context.cmd())
        else:
            self.log.debug('Connection not found for [%s]' % (context.target))

    def items(self):
        return self._conns.items()


class SmartServer(object):
    
    def __init__(self, hostname=smart.const.LISTEN
                 , cmdPort = smart.const.CMD_PORT
                 , buffSize = smart.const.BUFF_SIZE
                 , recvTimeout = smart.const.RECV_TIMEOUT
                 , conns = Connections()
                 , log = logging.getLogger('SmartServer')):
        self.cmdPort = cmdPort
        self.hostname = hostname
        self.conns = Connections()
        self.log = log
        self.recvTimeout = recvTimeout
        self.buffSize = buffSize
        self.stopEvent = threading.Event()
        self.cmdSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def run(self):
        
        for i in range(1, 100):
            try:
                self.cmdSocket.bind((self.hostname, self.cmdPort))
            except socket.error, v:
                if i > 90:
                    exit(1)
                self.log.error('Server bind error. Retry after %ds.' % (5))
                time.sleep(5)
                continue
            else:
                break
            

        self.cmdSocket.listen(1)
        self.log.info('Server %s listening on %d starting...' 
                      % (self.hostname, self.cmdPort))

        while True:
            try:
                conn, addr = self.cmdSocket.accept()
            except socket.error:
                self.log.error('Socket error.')
                break
            except KeyboardInterrupt:
                self.log.info('User close by keyboard interrupt.')
                break
            addr = "%s:%d" % addr
            self.log.info('Connectd by %s' % (addr))
            self.conns.add(addr, conn)
            t = threading.Thread(target=self.handleClient, args=(conn, addr))
            t.start()

        self.stopEvent.set()

    def __del__(self):
        self.log.info('User call __del__ to close.')
        for host, con in self.conns.items():
            con.close()
        self.cmdSocket.close()
        self.stopEvent.set()
        
    def handleClient(self, conn, addr):
        conn.settimeout(self.recvTimeout)
        while not self.stopEvent.is_set():
            try:
                data = conn.recv(1024)
            except socket.timeout:
                continue
            except socket.error, v:
                break
            if not data: 
                break
            data = data.replace('\n', '')
            if not data:
                continue
            for cmd in re.sub(r'([^\\]);', r'\1;;;', data).split(';;;'):
                cmd = self.__checkOrigin(cmd, addr)
                context = Context(cmd)
                context.connections = self.conns
                result = self.executeCommand(context)
                if result:
                    self.conns.sendLog(context.origin, result)
        self.log.info('Host [%s] closed.' % addr)
        self.conns.remove(addr)
        conn.close()

    def __checkOrigin(self, cmd, origin):
        originRe = '\\s+for\\s+(me|%s)\\s*$' % origin
        cmd = re.sub('\\\\(;|\\\\)', '\\1', cmd)
        if re.search(originRe, cmd):
            self.log.debug('In check origin match self.')
            return re.sub(originRe, ' for %s' % (origin), cmd)
        elif re.search('for\\s+\\w+\\s*$', cmd):
            self.log.debug('In check origin match other.')
            return cmd
        else:
            self.log.debug('In check origin Not match.')
            return ' '.join([cmd, 'for', origin])

    def executeCommand(self, context):
        self.log.debug("[%s] execute command: '%s' with param '%s' to [%s]" 
                       % (context.origin, context.cmdName
                          , context.param, context.target))
        if context.target == 'smart':
            return CommandExecutor(context).run()
        elif context.target == 'all':
            for host, con in self.conns.items():
                if host != context.origin:
                    context.target = host
                    return self.conns.send(context)
        else:
            return self.conns.send(context)


