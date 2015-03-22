
.section .text
.global keyboard_handler
.type keyboard_handler, @function
keyboard_handler:
	call keyboard_handler_c
	iret
.size keyboard_handler, . - keyboard_handler


.align 2                    /* Align to a 8-byte (2^3)*/
ptr:
.word  0x0
.word  0x0

.section .text
.global load_idt
.type load_idt, @function

load_idt:
#	popl ptr + 2
#	popl ptr
	movl 0x4(%esp), %edx
	lidt (%edx)
#	lidt ptr
	sti
	ret
.size load_idt, . - load_idt
