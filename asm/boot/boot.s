.code16
.global _start, begtext, begdata, begbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text
	.equ BOOTSEG, 0x07c0
	ljmp $BOOTSEG, $_start
_start:
	mov $0x03, %ah
	xor %bh, %bh
	int $0x10
	
	mov $24, %cx
	mov $0x0007, %bx
	mov $msg, %bp
	mov $0x1301, %ax
	int $0x10
sectors:
	.word 0
msg:
	.byte 13,10
	.ascii "Loading system ..."
	.byte 13,10,13,10
	
	.org 510
boot_flat:
.word 0xAA55



	