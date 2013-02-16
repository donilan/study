import thread, socket, time

port = 18080
addr = (socket.gethostname(), port)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print 'Listening on', socket.gethostname(), 'port', port

s.setblocking(0)
clients = {}

while True:
    pass
