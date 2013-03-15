from django.conf.urls.defaults import patterns, url

urlpatterns = patterns(
    '',
    url(r'^$', 'guestbook.views.index'),
    url(r'^sign$', 'guestbook.views.sign'),
    url(r'^thumbnail', 'guestbook.views.thumbnail'),
    url(r'^file', 'guestbook.views.file'),
    url(r'^up', 'guestbook.views.up'),
)

