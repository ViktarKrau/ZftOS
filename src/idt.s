.section .text
.global get_cs
.type get_cs, @function
get_cs:
		xor		%eax, %eax
		movw	%cs, %ax
		movl	%eax, __cs
		ret
.size get_cs, . - get_cs



.section .text
.global keyboard_handler
.type keyboard_handler, @function
keyboard_handler:
		call keyboard_handler_c
		iret
.size keyboard_handler, . - keyboard_handler

.section .text
.global timer_handler
.type timer_handler, @function
timer_handler:
		call timer_handler_c
#		movw 'A', %ax						#TEST
#		movw %ax, (0x6800)					#TEST
		iret
.size timer_handler, . - timer_handler


.align 2                    /* Align to a 8-byte (2^3)*/
.global __cs
#.type __cs, @long
__cs:
.long  0x0
.word  0x0

.section .text
.global load_idt
.type load_idt, @function
load_idt:
#	popl ptr + 2

		movl	0x4(%esp), %edx
		lidt 	(%edx)
		sti
		ret
.size load_idt, . - load_idt

.section .text
.global load_gdt
.type load_gdt, @function
load_gdt:
		movl 	0x4(%esp), %edx
		lgdt 	(%edx)
		movw 	0x10, %ax      #0x10 is the offset in the GDT to our data segment
   		movw 	%ax, %ds       #Load all data segment selectors
    	movw 	%ax, %es
    	movw 	%ax, %fs
    	movw 	%ax, %gs
    	ret
.size load_gdt, . - load_gdt



