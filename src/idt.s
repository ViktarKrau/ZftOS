
.section .text
.global keyboard_handler
.type keyboard_handler, @function
keyboard_handler:
	call keyboard_handler_c
	iret
.size keyboard_handler, . - keyboard_handler

.section .text
.global load_idt
.type load_idt, @function
load_idt:
	movl 0x4(%esp), %edx
	lidt (%edx)
	sti 
	ret
.size load_idt, . - load_idt
