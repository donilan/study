from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app
from google.appengine.api import urlfetch
#from dropbox import session, rest, client
from gdc import session
class MainPage(webapp.RequestHandler):

    def get(self):
        sess = session.DropboxSession('vq7oaa7sagyzqaq', 'gzm2e0t4t2odqlj', 'dropbox')
        request_token = sess.obtain_request_token()
        url = sess.build_authorize_url(request_token, 'http://localhost:8080/callback')
        self.redirect(url)

class CallBack(webapp.RequestHandler):
    def get(self):
        self.response.headers['Content-Type'] = 'text/plain'
        self.response.out.write('<br />Hello, webapp World!')

application = webapp.WSGIApplication(
                                     [('/', MainPage)
                                      ,('/callback', CallBack)],
                                     debug=True)

def main():
    run_wsgi_app(application)

if __name__ == "__main__":
    main()
