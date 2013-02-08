;;;
        .386
        .model flat,stdcall
        option casemap:none
        
include windows.inc
include user32.inc
includelib user32.lib
include kernel32.inc
includelib kernel32.lib
include masm32.inc
includelib masm32.lib        
        
        .data
tx db 157,178,204,104,181,141,200,43,253,241,141,105,167,224,255,110
          db 137,175,193,109,128,136,249,73,0
tx2 db 'Hello world!', 0        
        
        .data?
hStdOut dd ?
result dd ?
        
        .code
start:
;;      invoke MessageBox, NULL, offset tx, NULL, MB_OK
        ;; invoke StdOut, addr tx2
        push ebp
        mov ebp, esp
        add esp, -04
        invoke GetStdHandle, STD_OUTPUT_HANDLE
        lea ebp, DWORD ptr [esp]
        invoke WriteFile, eax, ADDR tx2, LENGTHOF tx2, ebp, NULL
        invoke ExitProcess, NULL
        
end start