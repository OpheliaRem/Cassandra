.global isr80_stub
.extern syscall_handler

isr80_stub:
    cli

    push %edi
    push %esi
    push %ebp
    push %ebx
    push %edx
    push %ecx

    push %ebp
    push %edi
    push %esi
    push %edx
    push %ecx
    push %ebx
    push %eax

    call syscall_handler
    add $28, %esp

    pop %ecx
    pop %edx
    pop %ebx
    pop %ebp
    pop %esi
    pop %edi

    iret
