from smart import ConnectionHolder

def execute(addr, cmd):
    ConnectionHolder.send(addr, 'Now your name is: %s.' % cmd)
