#!/usr/bin/env python

import soaplib
from soaplib.core.service import DefinitionBase, soap
from soaplib.core.server import wsgi
from wsgiref.simple_server import make_server

class HelloWorldService(DefinitionBase):
    @soap(_returns=None)
    def hello(self):
        print "Hello world"


soapApplication = soaplib.core.Application([HelloWorldService], 'tns')
wsgiApplication = wsgi.Application(soapApplication)
server = make_server('127.0.0.1', 8081, wsgiApplication)
server.serve_forever()
