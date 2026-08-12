CC=gcc
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c
LDFLAGS=-T link.ld -melf_i386
ASFLAGS=-f elf

all: kernel.elf

kernel.elf: kernel.asm.o kernel.o keyboard.o
	ld $(LDFLAGS) kernel.asm.o kernel.o keyboard.o -o kernel.elf

kernel.asm.o: kernel.asm
	nasm $(ASFLAGS) kernel.asm -o kernel.asm.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

keyboard.o: keyboard.c
	$(CC) $(CFLAGS) keyboard.c -o keyboard.o

clean:
	rm -f *.o kernel.elf

run: kernel.elf
	qemu-system-i386 -kernel kernel.elf

.PHONY: all clean run