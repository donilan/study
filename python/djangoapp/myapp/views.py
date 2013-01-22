# Create your views here.

from django.conf import settings
from django.utils.importlib import import_module
from django.http import HttpResponse
from django.template import RequestContext
from django.shortcuts import render_to_response, render
from datetime import datetime
from django import forms

class UploadFileForm(forms.Form):
    title = forms.CharField(max_length=20)
    file = forms.FileField()

def current_datetime(request):
    now = datetime.now()
    html = "<html><body>It is now %s.</body></html>" % now
    return HttpResponse(html)

def upload(request):

#    engine = import_module(settings.SESSION_ENGINE)
#    engine.SessionStore(None).save()
#    print request.COOKIES.get(settings.SESSION_COOKIE_NAME, None)
#    print settings.SESSION_COOKIE_NAME

    if request.method == 'POST':
        form = UploadFileForm(request.POST, request.FILES)
        if form.is_valid():
            f = request.FILES['file']
            
            print f.name
            print f.size
#            for c in f.chunks():
#                print c
        else:
            print form
    else:
        form = UploadFileForm()
#    c = RequestContext(request, {'form': form})
#    return render_to_response('myapp/upload.html', c)
    return render(request, 'myapp/upload.html', {'form': form})


def session(request):
    if(request.GET.get('set')):
        for k in request.GET.keys():
            request.session[k] = request.GET[k]
            return HttpResponse('Success')
    if(request.GET.get('get')):
        return HttpResponse(request.session[request.GET['get']])
    
    return HttpResponse('No date.')
