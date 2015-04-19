GCC = ./compiler/bin/i686-elf-gcc
GPP = ./compiler/bin/i686-elf-g++
PFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -pedantic -std=c++11 -mno-red-zone
ASM = ./compiler/bin/i686-elf-as
CFLAGS = -std=gnu99 -ffreestanding  -O2 -Wall -Wextra
LFLAGS = -ffreestanding -O2 -nostdlib

all: assembly cppcompile link



assembly: 		boot idt
boot:
	$(ASM) src/boot.s -o obj/boot.o
idt:
	$(ASM) src/idt.s -o obj/idt.o



cppcompile: 	common input output memory_f exec app
app: 				shell clock
common: 			main interrupts kernel string time zft_memory
output: 			speaker terminal terminalstatebuffer
input: 				enterqueue inputstream
memory_f: 			bitmap memory new paging
exec: 				executable scheduler


main:
	$(GPP) -c src/main.c++ -o obj/main.o $(PFLAGS)
kernel:
	$(GPP) -c src/kernel.c++ -o obj/kernel.o $(PFLAGS)
enterqueue:
	$(GPP) -c src/input/enterqueue.c++ -o obj/input/enterqueue.o $(PFLAGS)
inputstream:
	$(GPP) -c src/input/inputstream.c++ -o obj/input/inputstream.o $(PFLAGS)
terminal:
	$(GPP) -c src/output/terminal.c++ -o obj/output/terminal.o $(PFLAGS)
terminalstatebuffer:
	$(GPP) -c src/output/terminalstatebuffer.c++ -o obj/output/terminalstatebuffer.o $(PFLAGS)
zft_memory:
	$(GPP) -c src/zft_memory.c -o obj/zft_memory.o $(PFLAGS)
memory:
	$(GPP) -c src/memory/memory.c++ -o obj/memory/memory.o $(PFLAGS)
string:
	$(GPP) -c src/string.c -o obj/string.o $(PFLAGS)
paging:
	$(GPP) -c src/memory/paging.c++ -o obj/memory/paging.o $(PFLAGS)
interrupts:
	$(GPP) -c src/interrupts.c -o obj/interrupts.o $(PFLAGS)
shell:
	$(GPP) -c src/app/shell.c++ -o obj/app/shell.o $(PFLAGS)
time:
	$(GPP) -c src/time.c++ -o obj/time.o $(PFLAGS)
new:
	$(GPP) -c src/memory/new.c++ -o obj/memory/new.o $(PFLAGS)
bitmap:
	$(GPP) -c src/memory/bitmap.c++ -o obj/memory/bitmap.o $(PFLAGS)
speaker:
	$(GPP) -c src/output/speaker.c++ -o obj/output/speaker.o $(PFLAGS)
executable:
	$(GPP) -c src/exec/executable.c++ -o obj/exec/executable.o $(PFLAGS)
scheduler:
	$(GPP) -c src/exec/scheduler.c++ -o obj/exec/scheduler.o $(PFLAGS)
clock:
	$(GPP) -c src/app/clock.c++ -o obj/app/clock.o $(PFLAGS)



link:
	$(GPP) -T src/linker.ld -o bin/zftos.bin $(LFLAGS) obj/output/speaker.o obj/memory/new.o \
	  obj/boot.o obj/main.o obj/kernel.o obj/output/terminalstatebuffer.o obj/app/shell.o \
	  obj/time.o obj/output/terminal.o obj/zft_memory.o obj/string.o obj/interrupts.o \
	  obj/memory/memory.o obj/input/inputstream.o obj/input/enterqueue.o obj/app/clock.o \
	  obj/memory/paging.o obj/idt.o obj/exec/executable.o obj/exec/scheduler.o obj/memory/bitmap.o -lgcc



run:
	qemu-system-i386 -kernel bin/zftos.bin



runiso:
	qemu-system-i386 -cdrom	 bin/zftos.iso



iso:
	cp bin/zftos.bin bin/isodir/boot/zftos.bin
	grub-mkrescue -o bin/zftos.iso bin/isodir
	qemu-system-i386 -cdrom	 bin/zftos.iso



burniso:
	sudo dd if=bin/zftos.iso of=/dev/sdc && sync
