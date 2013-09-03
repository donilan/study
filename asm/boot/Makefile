boot: clean boot.s
	as --32 -o boot.o boot.s
	ld -Ttext 0 -m elf_i386 -x -o boot boot.o
	objcopy -R .pdr -R .comment -R .note -S -O binary boot
	dd if=boot of=image bs=512 count=1
	dd if=image of=/home/d/VirtualBox\ VMs/boot-test/boot-test-flat.vmdk
	hexdump -C image
clean:
	rm *~ *.o boot -f