# Create your views here.
from django.views.decorators.csrf import csrf_exempt
from django.http import HttpResponse
from apps.peParse import read

def upload(request):
    return HttpResponse("""
<form method="POST" action="doUpload" enctype="multipart/form-data">
  <input type="file" name="file" />
  <input type="submit" value="Upload" />
</form>""")

@csrf_exempt
def doUpload(request):
    f = request.FILES['file']
    read(f)
    return HttpResponse('Done\n')
