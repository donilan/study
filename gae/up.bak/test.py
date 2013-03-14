#!/usr/bin/env python

from dropbox import session,client

sess = session.DropboxSession('vq7oaa7sagyzqaq', 'gzm2e0t4t2odqlj', 'dropbox')
#sess.obtain_request_token()
sess.token = session.OAuthToken('1tr3wd2q707dlyf', 'oh8jbht5iz8mbp5')
c = client.DropboxClient(sess)
print c.account_info()

