from django.http import HttpResponseRedirect, HttpResponse
from django.shortcuts import render_to_response
from django.template.context import RequestContext
from django.views.decorators.csrf import csrf_exempt
from django.conf import settings

import logging, gdc, re, up0.token


def index(request):
    return render_to_response('index.html')

@csrf_exempt
def upload(request):
    pass

def find(request):
    pass

def reg(request):
    sess = _getSession()
    token = sess.obtain_request_token()
    url = sess.build_authorize_url(
        token, 'https://%s/callback?key=%s&secret=%s' % 
        (request.get_host(), token.key, token.secret))

    return HttpResponseRedirect(url)

def callback(request):

    key = request.GET.get('key', None)
    secret = request.GET.get('secret', None)
    if key is None or secret is None:
        return HttpResponse('fail')
    sess = _getSession()
    sess.obtain_access_token(gdc.session.OAuthToken(key, secret))

    oauthToken = sess.token
    up0.token.storeToken(_getClient(), oauthToken.key, oauthToken.secret)
    return HttpResponse('success')

def viewStoreTokens(request):
    return HttpResponse(_getStoreFile())

def _getSession():
    return gdc.session.DropboxSession(settings.DROPBOX_KEY \
                                      , settings.DROPBOX_SECRET \
                                      , settings.DROPBOX_ACCESS_TYPE)

def _getClient():
    sess = gdc.session.DropboxSession(settings.DROPBOX_KEY \
                                      , settings.DROPBOX_SECRET \
                                      , settings.DROPBOX_ACCESS_TYPE)
    sess.token = gdc.session.OAuthToken(settings.DROPBOX_TOKEN_KEY \
                                        , settings.DROPBOX_TOKEN_SECRET)
    return gdc.client.DropboxClient(sess)


def _getStoreFile():
    content = _getClient().get_file('/tokens.db')
    if content is not None or re.search('error":\s*"File not found', content):
        _getClient().put_file('/tokens.db', '')
        content = ''
    return content
