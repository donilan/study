from django.conf.urls.defaults import patterns, url

urlpatterns = patterns(
    '',
    url(r'^$', 'up0.views.index'),
    url(r'^reg$', 'up0.views.reg'),
    url(r'^callback$', 'up0.views.callback'),
    url(r'^up$', 'up0.views.upload'),
    url(r'^doUpload$', 'up0.views.doUpload'),
    url(r'^tokens$', 'up0.views.viewStoreTokens'),
)

