#!/usr/bin/env python

'readTeztFile.py -- read and display text file'

# get filename

fname = raw_input('Enter filename: ')
print

# attempt to open file for reading
try:
    fobj = open(fname)
except IOError, e:
    print "*** file open error:", e
else:
    # display contents to the screen
    for eachLine in fobj:
        print eachLine,
    fobj.close()
