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
.global get_eip
.type get_eip, @function
get_eip:
		popl	%eax
		pushl	%eax
		pushl	%eax
		ret
.size get_eip, . - get_eip


.section .text
.global cmos_handler
.type cmos_handler, @function
cmos_handler:
        pusha
		call cmos_handler_c
		popa
		iret
.size cmos_handler, . - cmos_handler

.section .text
.global keyboard_handler
.type keyboard_handler, @function
keyboard_handler:
        pusha
		call keyboard_handler_c
		popa
		iret
.size keyboard_handler, . - keyboard_handler

.global previous_task_registers_ptr
previous_task_registers_ptr:
    .word 0


.global next_task_registers_ptr
next_task_registers_ptr:
    .word 0

previous_task_eip_ptr:
	.word 0


.section .text
.global timer_handler
.type timer_handler, @function
timer_handler:
        pusha
		call 	timer_handler_c
		popa
		push    %eax
		#previously:without $
		mov  	next_task_registers_ptr, %eax
		test	%eax, %eax
		jz 		end_timer_handler
		push    %ebx
		#previously: mov $previous_task_eip_ptr...

		#mov		$previous_task_eip_ptr, %eax
		mov 	8(%esp), %ebx                       # == mov 8(%esp), previous_task_eip_ptr
		mov 	%ebx, previous_task_eip_ptr

		mov		$after_interrupt_task_switch, %ebx
		mov 	%ebx, 8(%esp)
		pop     %ebx
end_timer_handler:
        pop     %eax
		iret
.size timer_handler, . - timer_handler

.section .text
.type after_interrupt_task_switch, @function
after_interrupt_task_switch:

		mov 	next_task_registers_ptr, %eax
		push 	%eax

        mov 	previous_task_registers_ptr, %eax
        push 	%eax

        mov 	previous_task_eip_ptr, %eax
        push 	%eax
        mov     'A', %eax
        mov     %eax, (0xB8000)
        jmp 	switch_tasks


.global __cs
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



