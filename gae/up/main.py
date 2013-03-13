from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app
from google.appengine.api import urlfetch
#from dropbox import session, rest, client

class MainPage(webapp.RequestHandler):

    def get(self):
        headers = {'Content-type': 'application/x-www-form-urlencoded'}
        
        body = 'oauth_version=1.0&oauth_nonce=83308840&oauth_timestamp=1363163829&oauth_signature=gzm2e0t4t2odqlj%26&oauth_consumer_key=vq7oaa7sagyzqaq&oauth_signature_method=PLAINTEXT'
        result = urlfetch.fetch(url='https://api.dropbox.com/1/oauth/request_token', method=urlfetch.POST, headers=headers, payload=body)
        self.response.headers['Content-Type'] = 'text/plain'
        self.response.out.write(result.content)
        self.response.out.write('<br />Hello, webapp World!')
        


class CallBack(webapp.RequestHandler):
    def get(self):
        import ssl, pkg_resources
        print ssl.__package__

application = webapp.WSGIApplication(
                                     [('/', MainPage)
                                      ,('/callback', CallBack)],
                                     debug=True)

def main():
    run_wsgi_app(application)

if __name__ == "__main__":
    main()
