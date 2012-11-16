#!/usr/bin/env python

import threading
from time import ctime, sleep

def run():
    print 'start run function at:', ctime()
    sleep(6)
    print 'end run function at:', ctime()

def main():
    thread = threading.Thread(target=run)
    print 'is daemon', thread.isDaemon()
    thread.setDaemon(True)

    thread.start()
    print 'all DONE at: ', ctime()

if __name__ == '__main__':
    main()
