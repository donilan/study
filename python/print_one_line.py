#!/usr/bin/env python

import time, sys
tmpl = "\rDownload %s%%."
for i in range(1,101):
    tmp = tmpl % (i)
#    print chr(8) * (len(tmp) + 3),
#    time.sleep(0.1)
#    print tmp


def progressbar():
    for i in range(100):
        sys.stdout.write("%i\r" % i)
        time.sleep(0.01)

def update_progress():
    print ''
    for i in range(1000):
        time.sleep(0.1)
        print chr(27) + '[A[{0}] {1}%'.format('#'*(i/10), i)
        

update_progress()
    
