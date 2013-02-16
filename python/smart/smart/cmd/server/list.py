from smart import ConnectionHolder

def execute(addr, cmd):
    result = '\n'.join('client: %s' % k for k, v in ConnectionHolder.connections())
    ConnectionHolder.send(addr, result)
        
