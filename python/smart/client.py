#!/usr/bin/env python

from smart.client import SmartClient
import logging

logging.basicConfig(level=logging.DEBUG)

if __name__ == '__main__':
    SmartClient(host='127.0.0.1').run()
