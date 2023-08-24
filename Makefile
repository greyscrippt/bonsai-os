ASM = nasm -f elf32
GCC = gcc -m32 -c
LNK = ld -m elf_i386 -T linker.ld

all: build run

build:
	$(GCC) -o kernel.o kernel.c
	$(ASM) -o boot.o boot.asm
	$(LNK) -o bonsai.bin kernel.o boot.o

run: bonsai.bin
	qemu-system-i386 --kernel bonsai.bin

clean:
	rm *.o
	rm *.bin
