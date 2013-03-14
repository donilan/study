from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app
from gdc import session, client
import settings
class MainPage(webapp.RequestHandler):

    def get(self):
        sess = session.DropboxSession('vq7oaa7sagyzqaq', 'gzm2e0t4t2odqlj', 'dropbox')
        request_token = sess.obtain_request_token()
        url = sess.build_authorize_url(request_token, 'http://localhost:8080/callback')
        settings.sessions.append(sess)
        self.redirect(url)

class CallBack(webapp.RequestHandler):
    def get(self):
        self.response.headers['Content-Type'] = 'text/plain'
        token = settings.sessions[-1].obtain_access_token()
        c = client.DropboxClient(settings.sessions[-1])
        self.response.out.write(self.request.GET['uid'])
        self.response.out.write(' / ')
        self.response.out.write(c.account_info())
        self.response.out.write(' / ')
        self.response.out.write(token.key)

class AccountInfo(webapp.RequestHandler):
    def get(self):

        self.response.headers['Content-Type'] = 'text/plain'
        self.response.out.write(c.account_info())

application = webapp.WSGIApplication(
                                     [('/', MainPage)
                                      ,('/callback', CallBack)
                                      ,('/accountInfo', AccountInfo)
                                      ],
                                     debug=True)

def main():
    run_wsgi_app(application)

if __name__ == "__main__":
    main()
