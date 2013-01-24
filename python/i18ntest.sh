#!/usr/bin/env sh

PROJECT_NAME=i18ntest
SETTINGS_FILE=$PROJECT_NAME/settings.py
URLS_FILE=$PROJECT_NAME/urls.py
PROJECT_HOME=`pwd`/$PROJECT_NAME
LANGUAGE=zh_CN

if [ -d $PROJECT_NAME ] 
then
    read -p "Do you want to delete $PROJECT_NAME?" yn
    case $yn in
	[yY]* ) rm $PROJECT_NAME -rf;;
	* ) exit 0;;
    esac
fi
	

## Create project
django-admin.py startproject $PROJECT_NAME

## Enter project dir
cd $PROJECT_NAME

## Create an app and add to settings file.
django-admin.py startapp app
sed -i "/'django.contrib.admindocs'/a\	'app'," $SETTINGS_FILE

## Add sqlite3 support for session use.
sed -i -e "s/django.db.backends./&sqlite3/g " \
    -e "s/'NAME':\s*'/&test.db/g" $SETTINGS_FILE

## Add locale middleware for i18n.
sed -i "/SessionMiddleware/a\	'django.middleware.locale.LocaleMiddleware'," \
    $SETTINGS_FILE

## Add url pattern for app view name index.
sed -i "/admin.site.urls/a\    url(r'^$', 'app.views.index')," $URLS_FILE

## Enter app dir to make locale files.
cd app

## First add a view.
TMP=`cat <<EOF
from django.utils.translation import ugettext as _
from django.http import HttpResponse

def index(request):
    return HttpResponse(_('Hello world!'))
EOF`
echo "$TMP" >> views.py

## Make locale file
mkdir locale
django-admin.py makemessages -l $LANGUAGE
sed -i -e '/Hello world/{n;s/""/"Hello Chinese!"/;}' \
    locale/$LANGUAGE/LC_MESSAGES/django.po
django-admin.py compilemessages


## Run app server
cd $PROJECT_HOME
yes 'no' | python manage.py syncdb
python manage.py runserver

