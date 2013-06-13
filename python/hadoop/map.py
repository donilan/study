#!/usr/bin/env python

import sys

f = open("/tmp/hadoop.out", "aw")
for line in sys.stdin:
    for word in line.split():
        f.write("%s\n" % word)
        print "%s\t1" % (word)

f.close()
