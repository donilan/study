#!/usr/bin/env python

from ctypes import *

class _PROCESS_INFOMATION(Structure):
    _fields_ = [('hProcess', c_void_p),
                ('hThread', c_void_p),
                ('dwProcessId', c_ulong),
                ('dwThreadId', c_ulong)]

class _STARTUPINFO(Structure):
    _fields_ = [('cb', c_ulong),
                ('lpReserved', c_char_p),
                ('lpDesktop', c_char_p),
                ('lpTitle', c_char_p),
                ('dwX', c_ulong),
                ('dwY', c_ulong),
                ('dwXSize', c_ulong),
                ('dwYSize', c_ulong),
                ('dwXCountChars', c_ulong),
                ('dwYCountChars', c_ulong),
                ('dwFillAttribute', c_ulong),
                ('dwFlags', c_ulong),
                ('wShowWindow', c_ushort),
                ('cbReserved2', c_ushort),
                ('lpReserved2', c_char_p),
                ('hStdInput', c_ulong),
                ('hStdOutput', c_ulong),
                ('hStdError', c_ulong),
]

NORMAL_PRIORITY_CLASS = 0x00000020
kernel32 = windll.LoadLibrary('kernel32.dll')
CreateProcess = kernel32.CreateProcessA
ReadProcessMemory = kernel32.ReadProcessMemory
WriteProcessMemory = kernel32.WriteProcessMemory
TerminateProcess = kernel32.TerminateProcess

ProcessInfo = _PROCESS_INFOMATION()
StartupInfo = _STARTUPINFO()

file = 'ModifyMe.exe'
address = 0x004010AA
buffer = c_char_p("_")
bytesRead = c_ulong(0)
bufferSize = len(buffer.value)

print "buffer size:", bufferSize

if CreateProcess(file, 0, 0, 0, 0, NORMAL_PRIORITY_CLASS, 0, 0, byref(StartupInfo), byref(ProcessInfo)):
    if ReadProcessMemory(ProcessInfo.hProcess, address, buffer, bufferSize, byref(bytesRead)):
        if buffer.value == '\x74':
            buffer.value = '\x75'
            if WriteProcessMemory(ProcessInfo.hProcess, address, buffer, bufferSize, byref(bytesRead)):
                print 'Success'
            else:
                print 'Error write.'

        else:
            print 'Error for open file.'
            TerminateProcess(ProcessInfo.hProcess, 0)
    else:
        print 'Error for read memory.'
else:
    print 'Error for create process.'
