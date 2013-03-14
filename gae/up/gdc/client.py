from google.appengine.api import urlfetch
import urllib, pickle, re, logging

def format_path(path):
    """Normalize path for use with the Dropbox API.

    This function turns multiple adjacent slashes into single
    slashes, then ensures that there's a leading slash but
    not a trailing slash.
    """
    if not path:
        return path

    path = re.sub(r'/+', '/', path)

    if path == '/':
        return (u"" if isinstance(path, unicode) else "")
    else:
        return '/' + path.strip('/')


class DropboxClient(object):
    def __init__(self, session):
        self.session = session

    def request(self, path, params=None, content_server=False):
        host = self.session.API_CONTENT_HOST if content_server else self.session.API_HOST
        if params is None:
            params = {}
        headers, params = self.session.build_access_headers(params)
        url = self.session.build_url(self.session.API_HOST, path, params)
        result = urlfetch.fetch(url=url, method=urlfetch.GET, headers=headers)
        return result.content

    def account_info(self):
        return self.request('/account/info')


    def metadata(self, path, list=True, file_limit=25000, hash=None, rev=None, include_deleted=False):
        path = "/metadata/%s%s" % (self.session.root, format_path(path))
        params = {'file_limit': file_limit,
                  'list': 'true',
                  'include_deleted': include_deleted
                  }
        if not list:
            params['list'] = 'false'
        if hash is not None:
            params['hash'] = hash
        if rev:
            params['rev'] = rev
        
        return self.request(path, params)

    def thumbnail(self, from_path, size='large', format='JPEG'):
        assert format in ['JPEG', 'PNG'], 'expected a thumbnail format of "JPEG" or "PNG", got %s' % format
        path = "/thumbnails/%s%s" % (self.session.root, format_path(from_path))
        
