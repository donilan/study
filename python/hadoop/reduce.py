#!/usr/bin/env python

import sys

wordCounter = dict()
for line in sys.stdin:
    word = line.split("\t")[0]
    val = wordCounter.get(word, 0)
    wordCounter[word] = val + 1

for it in wordCounter.items():
    print "%s\t%d" % it
    
