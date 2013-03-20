#!/usr/bin/env python

from dropbox import session,client

sess = session.DropboxSession('y2zwounslkkgd4r', '0h1ds02le9fq7ig', 'dropbox')

request_token = sess.obtain_request_token()

url = sess.build_authorize_url(request_token)

# Make the user sign in and authorize this token
print "url:", url
print "Please visit this website and press the 'Allow' button, then hit 'Enter' here."
raw_input()

access_token = sess.obtain_access_token(request_token)

#sess.token = session.OAuthToken('1tr3wd2q707dlyf', 'oh8jbht5iz8mbp5')

print access_token.key, access_token.secret

c = client.DropboxClient(sess)
print c.account_info()

