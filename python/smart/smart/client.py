import logging, socket, threading
from smart import Context, CommandExecutor
import smart.const

class SmartClient(object):
    
    def __init__(self, host='java.ii2d.com', 
                 port = smart.const.CMD_PORT, 
                 buffSize = smart.const.BUFF_SIZE,
                 recvTimeout = smart.const.RECV_TIMEOUT,
                 alias = None,
                 log=logging.getLogger('SmartClient')):
        self.host = host
        self.port = port
        self.alias = alias
        self.recvTimeout = recvTimeout
        self.log = log
        self.buffSize = buffSize
        self.stopEvent = threading.Event()
        self.socket = socket.socket(socket.AF_INET
                                    , socket.SOCK_STREAM)
        self.socket.connect((self.host, self.port))
        self.socket.settimeout(self.recvTimeout)
        self.name = '%s:%s' % self.socket.getsockname()
        self.log.info('Connecting server [%s:%d] on [%s]...' 
                      % (self.host, self.port, self.name))
        t = threading.Thread(target=self.recvServerData)
        t.start()


    def recvServerData(self):
        self.log.info('Thread start for recv data.')
        while not self.stopEvent.is_set():
            try:
                data = self.socket.recv(self.buffSize)
            except socket.timeout:
                continue
            if not data:
                self.log.error('Server close connection.')
                break

            if data.startswith(self.name) or (self.alias is not None and data.startswith(self.alias)):
                context = Context(data)
                context.client = self
                result = CommandExecutor(context).run()
                if not result:
                    result = 'Execute success but no message.'
                    self.sendLog(result)
            else:
                self.log.info(data)

    def send(self, cmd):
        self.socket.sendall(cmd)

    def sendLog(self, msg):
        self.send('%s log info %s' % (smart.const.SERVER_ALIAS, msg))
        
    def close(self):
        self.log.info('Client closing...')
        self.socket.close()
        self.stopEvent.set()
