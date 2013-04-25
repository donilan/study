import gdc, logging

class Token(object):
    
    def __init__(self, id = 0, key = '', secret = ''):
        try:
            self.id = int(id)
            self.key = key
            self.secret = secret
        except ValueError:
            if isinstance(id, str):
                arr = id.split(',')
                if len(arr) == 3:
                    self.id = int(arr[0])
                    self.key = arr[1]
                    self.secret = arr[2]

def toTokenList(content):
    if not isinstance(content, str) or len(content) < 1:
        return []
    return [Token(s) for s in content.split('\n')]

def toStoreString(tokens):
    return '\n'.join([','.join([str(t.id), t.key, t.secret]) for t in tokens])

def genTokenId(tokens):
    if len(tokens) > 0:
        return max([t.id for t in tokens]) + 1
    return 1

def findTokens(client):
    if isinstance(client, gdc.client.DropboxClient):
        content = client.get_file('/tokens.db')
        tokens = toTokenList(content)
        return tokens
    else:
        logging.error('client must be DropboxClient.')
    return []

def storeToken(client, key, secret):
    if isinstance(client, gdc.client.DropboxClient):
        tokens = findTokens(client)
        id = genTokenId(tokens)
        tokens.append(Token(id, key, secret))
        tokenStr = toStoreString(tokens)
        client.put_file('/tokens.db', tokenStr)
    else:
        logging.error('client must be DropboxClient.')
    return False

if __name__ == '__main__':
    tokens = toTokenList('1,a,a\n2,b,b\n3,c,c\n4,d,d')
    print toStoreString(tokens)
    print 'gen token id:', genTokenId(tokens)
