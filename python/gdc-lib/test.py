#!/usr/bin/env python

from gdc import session

sess = session.DropboxSession('vq7oaa7sagyzqaq', 'gzm2e0t4t2odqlj', 'dropbox')
print sess.build_access_headers()
