#!/usr/bin/env python

from smart.client import SmartClient
import logging

logging.basicConfig(level=logging.DEBUG)

if __name__ == '__main__':
    client = SmartClient(host='127.0.0.1')
    while True:
        try:
            msg = raw_input('')
            client.send(msg)
        except KeyboardInterrupt:
            break
        if msg == 'quit': 
            break
    client.close()
