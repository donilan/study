#!/usr/bin/env python

import peParse

if __name__ == '__main__':
    data = open('NOTEPAD.EXE').read()
    dosHeader = peParse.parse(data, peParse.ImageDOSHeader)
    print peParse.parse(data, peParse.ImageFileHeader, dosHeader.lfanew+4)
