
CONNS = dict()

def add(name, conn):
    CONNS[name] = conn

def remove(name):
    del CONNS[name]

def get(name, _default=None):
    return CONNS.get(name, _default)

def send(name, data):
    con = get(name)
    if con:
        con.sendall(data)

def connections():
    return CONNS.items()
