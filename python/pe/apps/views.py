# Create your views here.
from django.views.decorators.csrf import csrf_exempt
from django.http import HttpResponse
from django.shortcuts import render_to_response
from apps.peParse import read

def upload(request):
    return render_to_response('upload.html')

@csrf_exempt
def doUpload(request):
    f = request.FILES['file']
    return render_to_response('result.html', read(f))
