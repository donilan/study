#!/usr/bin/env python

class token(object):
    
    def __init__(self, id = 0, key = '', secret = ''):
        self.id = id
        self.key = key
        self.secret = secret
        if isinstance(id, str):
            arr = id.split(',')
            if len(arr) == 3:
                self.id = arr[0]
                self.key = arr[1]
                self.secret = arr[2]


def store(tokens):
#    tokensTmp = read()
    f = open('tokens.db', 'w')
    for t in tokens:
        f.write('%s,%s,%s\n' % (t.id, t.key, t.secret))
    f.close()

def read():
    f = open('tokens.db', 'r')
    result = []
    
    for text in f.read().split('\n'):
        result.append(token(text))
    f.close()
    return result

if __name__ == '__main__':
    tokens = [
        token(1, 'key1', 'secret1'),
        token(2, 'key2', 'secret2'),
        token(3, 'key3', 'secret3'),
        token(4, 'key4', 'secret4'),
        ]
    store(tokens)
    tokens2 = read()
    for t in tokens2:
        print t.id, t.key, t.secret
