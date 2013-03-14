from google.appengine.api import urlfetch
import logging
import urllib, pickle

class DropboxClient(object):
    def __init__(self, session):
        self.session = session

    def account_info(self):
        headers, params = self.session.build_access_headers()
        url = self.session.build_url(self.session.API_HOST, '/account/info', params)
        result = urlfetch.fetch(url=url, method=urlfetch.GET, headers=headers)
        return result.content
