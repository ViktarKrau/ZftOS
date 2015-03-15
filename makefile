GCC=./compiler/bin/i686-elf-gcc
GPP=./compiler/bin/i686-elf-g++
PFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -pedantic
ASM=./compiler/bin/i686-elf-as
CFLAGS=-std=gnu99 -ffreestanding  -O2 -Wall -Wextra
LFLAGS=-ffreestanding -O2 -nostdlib

all: boot idt cppcompile link


boot:
	$(ASM) src/boot.s -o obj/boot.o
idt:
	$(ASM) src/idt.s -o obj/idt.o

cppcompile: interrupts main kernel enterqueue inputstream terminal memory string shell
main:
	$(GPP) -c src/main.c++ -o obj/main.o $(PFLAGS)
kernel:
	$(GPP) -c src/kernel.c++ -o obj/kernel.o $(PFLAGS)
enterqueue:
	$(GPP) -c src/enterqueue.c++ -o obj/enterqueue.o $(PFLAGS)
inputstream:
	$(GPP) -c src/inputstream.c++ -o obj/inputstream.o $(PFLAGS)
terminal:
	$(GPP) -c src/terminal.c++ -o obj/terminal.o $(PFLAGS)
memory:
	$(GPP) -c src/zft_memory.c -o obj/zft_memory.o $(PFLAGS)
string:
	$(GPP) -c src/string.c -o obj/string.o $(PFLAGS)
interrupts:
	$(GPP) -c src/interrupts.c -o obj/interrupts.o $(PFLAGS)
shell:
	$(GPP) -c src/shell.c++ -o obj/shell.o $(PFLAGS)


link:
	$(GPP) -T src/linker.ld -o bin/zftos.bin $(LFLAGS) obj/boot.o obj/main.o obj/kernel.o obj/shell.o obj/terminal.o obj/zft_memory.o obj/string.o obj/interrupts.o obj/inputstream.o obj/enterqueue.o obj/idt.o -lgcc



run:
	qemu-system-i386 -kernel bin/zftos.bin
	
iso:
	grub-mkrescue -o bin/zftos.iso bin/isodir