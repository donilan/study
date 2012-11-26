from ctypes import *
import os
libtest = cdll.LoadLibrary(os.getcwd()+ '/example.dll')
libtest.hello()
