#!/usr/bin/env python

import smart.server, logging

logging.basicConfig(level=logging.DEBUG)
        
if __name__ == '__main__':
    smart.server.SmartServer().run()
