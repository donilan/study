#!/usr/bin/env python
import socket, sqlite3
from xml.dom.minidom import parseString
from xml.parsers.expat import ExpatError

SUCCESS = True
FAIL = False
conn = sqlite3.connect('auth.db')

def init():
    c = conn.cursor()
    c.execute('''
CREATE TABLE IF NOT EXISTS AUTH(
  USERNAME VARCHAR(255) NOT NULL,
  PASSWORD VARCHAR(255) NOT NULL,
  MAC VARCHAR(255)
)
''')
    c.execute('''
CREATE TABLE IF NOT EXISTS AUTH_LOG(
  USERNAME VARCHAR(255),
  PASSWORD VARCHAR(255),
  MAC VARCHAR(255),
  IP VARCHAR(255),
  AUTH_DATE DATE DEFAULT (DATETIME('now', 'localtime'))
)
''')
    conn.commit()

def authLog(fn):
    def new(param):
        if(param):
            c = conn.cursor()
            c.execute("INSERT INTO AUTH_LOG(USERNAME, PASSWORD, MAC, IP) VALUES (?,?,?,?)", param)
            conn.commit()
        return fn(param)
    return new

def parseData(xmlStr):
    print xmlStr
    try:
        dom = parseString(xmlStr)
        if dom is None:
            return []
    except ExpatError:
        print 'Not well-formed'
        return []
    usernameTag = dom.getElementsByTagName('username')
    passwordTag = dom.getElementsByTagName('password')
    macTag = dom.getElementsByTagName('mac')
    if(usernameTag and passwordTag and macTag \
           and usernameTag[0].childNodes \
           and passwordTag[0].childNodes \
           and macTag[0].childNodes):
        return [usernameTag[0].childNodes[0].data,
                passwordTag[0].childNodes[0].data,
                macTag[0].childNodes[0].data]
    return []

@authLog
def auth(param):
    if param:
        c = conn.cursor()
        c.execute("SELECT * FROM AUTH WHERE USERNAME = ? AND PASSWORD = ?", [param[0], param[1]])
        user = c.fetchone()
        if user is not None:
            if user[2] == 'dev':
                return SUCCESS
            if user[2] is not None and user[2] != param[2]:
                return FAIL
            if user[2] is None:
                print 'update Mac address.'
                c.execute('UPDATE AUTH SET MAC = ? WHERE USERNAME = ? AND PASSWORD = ?', \
                              [param[2], param[0], param[1]])
                conn.commit()
            return SUCCESS
    return FAIL

if __name__ == '__main__':
    init()
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('0.0.0.0', 8002))
    sock.listen(5)
    while True:
        connection,address = sock.accept()
        try:
            connection.settimeout(5)
            buff = connection.recv(1024)
            param = parseData(buff)
            if(param):
                param.append(address[0])
            if auth(param):
                print 'success'
                connection.send('succ')
            else:
                print 'fail'
        except socket.timeout:
            print 'time out'
        connection.close()
