import urllib, time, random, logging
from google.appengine.api import urlfetch

try:
    from urlparse import parse_qs
except ImportError:
    # fall back for Python 2.5
    from cgi import parse_qs

class OAuthToken(object):
    
    def __init__(self, key, secret):
        self.key = key
        self.secret = secret

class DropboxSession(object):
    API_VERSION = 1
    API_HOST = "api.dropbox.com"
    WEB_HOST = "www.dropbox.com"
    API_CONTENT_HOST = "api-content.dropbox.com"

    def __init__(self, consumer_key, consumer_secret, access_type):
        assert access_type in ['dropbox', 'app_folder'], "expected access_type of 'dropbox' or 'app_folder'"
        self.consumer_creds = OAuthToken(consumer_key, consumer_secret)
        self.token = None
        self.request_token = None
        self.root = 'sandbox' if access_type == 'app_folder' else 'dropbox'

    def build_path(self, target, params=None):
        target_path = urllib.quote(target)
        params = params or {}
        params = params.copy()
        if params:
            return "/%d%s?%s" % (self.API_VERSION, target_path, urllib.urlencode(params))
        else:
            return "/%d%s" %(self.API_VERSION, target_path)

    def build_url(self, host, target, params=None):
        return "https://%s%s" % (host, self.build_path(target, params))
    
    def build_authorize_url(self, request_token, oauth_callback=None):
        params = {'oauth_token': request_token.key,}
        if oauth_callback:
            params['oauth_callback'] = oauth_callback
        
        return self.build_url(self.WEB_HOST, '/oauth/authorize', params)

    def obtain_request_token(self):
        self.token = None
        url = self.build_url(self.API_HOST, '/oauth/request_token')
        headers, params = self.build_access_headers()
        
        result = urlfetch.fetch(url=url, method=urlfetch.POST, headers=headers, payload=urllib.urlencode(params))

        self.request_token = self._parse_token(result.content)
        return self.request_token

    def obtain_access_token(self, request_token=None):
        request_token = request_token or self.request_token
        assert request_token, 'No request_token available on the session. Please pass one.'
        url = self.build_url(self.API_HOST, '/oauth/access_token')
        headers, params = self.build_access_headers(request_token=request_token)
        result = urlfetch.fetch(url=url, method=urlfetch.POST, headers=headers, payload=urllib.urlencode(params))
        self.token = self._parse_token(result.content)
        return self.token

    def build_access_headers(self, params=None, request_token=None):
        if params is None:
            params = {}
        else:
            params = params.copy()
        
        oauth_params = {
            'oauth_consumer_key': self.consumer_creds.key,
            'oauth_timestamp' : self._generate_oauth_timestamp(),
            'oauth_nonce': self._generate_oauth_nonce(),
            'oauth_version': self._oauth_version(),
            }
        token = request_token if request_token is not None else self.token 
        if token:
            oauth_params['oauth_token'] = token.key
        self._oauth_sign_request(oauth_params, self.consumer_creds, token)
        params.update(oauth_params)
        return {}, params
        
        
    @classmethod
    def _oauth_sign_request(cls, params, consumer_pair, token_pair):
        params.update({'oauth_signature_method' : 'PLAINTEXT',
                       'oauth_signature' : ('%s&%s' % (consumer_pair.secret, token_pair.secret) if token_pair is not None else '%s&' % (consumer_pair.secret,))})


    @classmethod
    def _generate_oauth_timestamp(cls):
        return int(time.time())

    @classmethod
    def _generate_oauth_nonce(cls, length=8):
        return ''.join([str(random.randint(0, 9)) for i in range(length)])
    
    @classmethod
    def _oauth_version(cls):
        return '1.0'

    @classmethod
    def _parse_token(cls, s):
        if not s:
            raise ValueError("Invalid parameter string.")
        params = parse_qs(s, keep_blank_values=False)
        if not params:
            raise ValueError("Invalid parameter string: %s" % s)
        
        try:
            key = params['oauth_token'][0]
        except Exception:
            raise ValueError("'oauth_token' not found in OAuth request.")

        try:
            secret = params['oauth_token_secret'][0]
        except Exception:
            raise ValueError("'oauth_token_secret' not found in OAuth request.")         
        return OAuthToken(key, secret)
