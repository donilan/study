from django.conf.urls import patterns, include, url

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

import myapp.views

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'djangoapp.views.home', name='home'),
    # url(r'^djangoapp/', include('djangoapp.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # url(r'^admin/', include(admin.site.urls)),
    url(r'^now', myapp.views.current_datetime),
    url(r'^upload', myapp.views.upload),
    url(r'^session', myapp.views.session),
)
