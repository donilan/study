from django.http import HttpResponseRedirect
from django.shortcuts import render_to_response
from django.template.context import RequestContext

from models import Post
from forms import PostForm
import logging
from gdc import session, client
from django.conf import settings

def index(request):
    query = Post.gql('ORDER BY date DESC')
    form = PostForm()
    sess = session.DropboxSession(settings.DROPBOX_KEY, settings.DROPBOX_SECRET, settings.DROPBOX_ACCESS_TYPE)
    sess.token = session.OAuthToken(settings.DROPBOX_TOKEN_KEY, settings.DROPBOX_TOKEN_SECRET)
    c = client.DropboxClient(sess)
    logging.info(c.metadata('/'))
    return render_to_response('index.html',
                              {'posts': query.fetch(20), 'form': form},
                              context_instance=RequestContext(request))


def sign(request):
    form = PostForm(request.POST)
    if form.is_valid():
        post = Post(author=form.cleaned_data['author'],
                    message=form.cleaned_data['message'])
        post.put()

    return HttpResponseRedirect('/')
