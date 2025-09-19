    .globl isr80_stub
isr80_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax        # kernel data selector
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    push %edx             # a3
    push %ecx             # a2
    push %ebx             # a1
    push %eax             # num
    call sys_dispatch
    add $16, %esp

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    iret
