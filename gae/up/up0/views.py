from django.http import HttpResponseRedirect, HttpResponse
from django.shortcuts import render_to_response
from django.template.context import RequestContext
from django.views.decorators.csrf import csrf_exempt
from django.conf import settings

import logging, gdc

def index(request):
    return render_to_response('index.html')

@csrf_exempt
def upload(request):
    pass

def find(request):
    pass

def _getClient():
    sess = session.DropboxSession(settings.DROPBOX_KEY \
                                      , settings.DROPBOX_SECRET \
                                      , settings.DROPBOX_ACCESS_TYPE)
    sess.token = session.OAuthToken(settings.DROPBOX_TOKEN_KEY \
                                        , settings.DROPBOX_TOKEN_SECRET)
    return client.DropboxClient(sess)


