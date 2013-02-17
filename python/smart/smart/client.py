import logging, socket, threading
from smart import Context, CommandExecutor
import smart.const

class SmartClient(object):
    
    def __init__(self, host='java.ii2d.com', 
                 port = smart.const.CMD_PORT, 
                 buffSize = smart.const.BUFF_SIZE,
                 alias = None,
                 log=logging.getLogger('SmartClient')):
        self.host = host
        self.port = port
        self.alias = alias
        self.log = log
        self.buffSize = buffSize
        self.stopEvent = threading.Event()
        self.socket = socket.socket(socket.AF_INET
                                    , socket.SOCK_STREAM)

    def recvServerData(self):
        self.log.info('Thread start for recv data.')
        while not self.stopEvent.is_set():
            data = self.socket.recv(self.buffSize)
            if not data:
                self.log.error('Server close connection.')
                break
            if data.startswith(self.name) or (self.alias is not None and data.startswith(self.alias)):
                context = Context(data)
                result = CommandExecutor(context).run()
                if not result:
                    result = 'Execute success but no message.'
                    self.socket.sendall('smart log info %s' % result)
            else:
                self.log.info(data)

    def run(self):
        self.socket.connect((self.host, self.port))
        self.name = '%s:%s' % self.socket.getsockname()
        self.log.info('Connecting server [%s:%d] on [%s]...' 
                      % (self.host, self.port, self.name))
        t = threading.Thread(target=self.recvServerData)
        t.start()
        while True:
            try:
                msg = raw_input('')
            except KeyboardInterrupt:
                break
            if msg == 'quit': 
                break
            self.socket.sendall(msg)
        self.log.info('User quit')
        self.socket.close()
        self.stopEvent.set()
