#!/usr/bin/env python

from dropbox import session

sess = session.DropboxSession('vq7oaa7sagyzqaq', 'gzm2e0t4t2odqlj', 'dropbox')
sess.obtain_request_token()
print sess
