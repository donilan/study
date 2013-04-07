;;; ASM 
        .386
        .model flat,stdcall
        option casemap:none

include windows.inc
include user32.inc
includelib user32.lib
include kernel32.inc
includelib kernel32.lib

;;; data
        .data
szText  db  'HelloWorld',0
;;; code
        .code
start:
        invoke MessageBox, NULL, offset szText, NULL, MB_OK
        invoke ExitProcess, NULL
end start        
	