from django.http import HttpResponseRedirect, HttpResponse
from django.shortcuts import render_to_response
from django.template.context import RequestContext
from django.views.decorators.csrf import csrf_exempt


from models import Post
from forms import PostForm
import logging
from gdc import session, client
from django.conf import settings

def index(request):
    query = Post.gql('ORDER BY date DESC')
    form = PostForm()
    c = _getClient()
    logging.info(c.thumbnail('/a.jpg'))
    return render_to_response('index.html',
                              {'posts': query.fetch(20), 'form': form},
                              context_instance=RequestContext(request))

def thumbnail(request):
    c = _getClient()
    resp = HttpResponse(content_type='image/jpg')
    resp.write(c.thumbnail('/a.jpg'))
    return resp

def file(request):
    c = _getClient()
    resp = HttpResponse(content_type='image/jpg')
    resp.write(c.get_file('/a.jpg'))
    return resp

@csrf_exempt
def up(request):
    c = _getClient()
    for f in request.FILES.values():
        logging.info(c.put_file(f.name, f))
    return HttpResponse('Hi')

def _getClient():
    sess = session.DropboxSession(settings.DROPBOX_KEY, settings.DROPBOX_SECRET, settings.DROPBOX_ACCESS_TYPE)
    sess.token = session.OAuthToken(settings.DROPBOX_TOKEN_KEY, settings.DROPBOX_TOKEN_SECRET)
    return client.DropboxClient(sess)



def sign(request):
    form = PostForm(request.POST)
    if form.is_valid():
        post = Post(author=form.cleaned_data['author'],
                    message=form.cleaned_data['message'])
        post.put()

    return HttpResponseRedirect('/')
