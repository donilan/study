#!/usr/bin/env python
# -*- coding: GBK -*-

import ctypes

EnumWindows = ctypes.windll.user32.EnumWindows
EnumWindowsProc = ctypes.WINFUNCTYPE(ctypes.c_bool, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int))
GetWindowText = ctypes.windll.user32.GetWindowTextW
GetWindowTextLength = ctypes.windll.user32.GetWindowTextLengthW
IsWindowVisible = ctypes.windll.user32.IsWindowVisible

titles = []
def foreach_window(hwnd, lParam):
	if IsWindowVisible(hwnd):
		length = GetWindowTextLength(hwnd)
		buff = ctypes.create_unicode_buffer(length + 1)
		GetWindowText(hwnd, buff, length + 1)
		print buff.value
		titles.append(buff.value)
	return True
EnumWindows(EnumWindowsProc(foreach_window), 0)

GetUserNameEx = ctypes.windll.secur32.GetUserNameExW

size = ctypes.pointer(ctypes.c_ulong(0))
print 'result:', GetUserNameEx(3, None, size)
print '==> size:',size.contents.value
nameBuffer = ctypes.create_unicode_buffer(size.contents.value)
print 'result:', GetUserNameEx(3, nameBuffer, size)
print '==============='
print nameBuffer.value

raw_input()
