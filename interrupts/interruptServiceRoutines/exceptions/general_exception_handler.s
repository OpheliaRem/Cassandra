.global isr_stubs

.section .text

.macro ISR_NO_ERRCODE vec
    .global isr\vec
    isr\vec:
        pushl $0          # dummy error_code
        pushl $\vec       # vector
        jmp isr_common_noerr
.endm

.macro ISR_ERRCODE vec
    .global isr\vec
    isr\vec:
        pushl $\vec       # vector
        jmp isr_common_err
.endm

ISR_NO_ERRCODE 0
ISR_NO_ERRCODE 1
ISR_NO_ERRCODE 2
ISR_NO_ERRCODE 3
ISR_NO_ERRCODE 4
ISR_NO_ERRCODE 5
ISR_NO_ERRCODE 6
ISR_NO_ERRCODE 7
ISR_ERRCODE    8
ISR_NO_ERRCODE 9
ISR_ERRCODE    10
ISR_ERRCODE    11
ISR_ERRCODE    12
ISR_ERRCODE    13
ISR_ERRCODE    14
ISR_NO_ERRCODE 15
ISR_NO_ERRCODE 16
ISR_ERRCODE    17
ISR_NO_ERRCODE 18
ISR_NO_ERRCODE 19
ISR_NO_ERRCODE 20
ISR_NO_ERRCODE 21
ISR_NO_ERRCODE 22
ISR_NO_ERRCODE 23
ISR_NO_ERRCODE 24
ISR_NO_ERRCODE 25
ISR_NO_ERRCODE 26
ISR_NO_ERRCODE 27
ISR_NO_ERRCODE 28
ISR_NO_ERRCODE 29
ISR_NO_ERRCODE 30
ISR_NO_ERRCODE 31


# Общая часть для векторов без error code
isr_common_noerr:
    pusha                   # сохраняем general-purpose регистры
    movl %esp, %eax         # eax = &regs (сейчас esp указывает на edi)
    pushl %eax              # arg3: regs*
    movl 36(%esp), %ecx     # vector (см. ниже пояснение про смещения)
    movl 40(%esp), %edx     # error_code (dummy 0)
    pushl %edx              # arg2
    pushl %ecx              # arg1
    call exception_handler
    addl $12, %esp          # убрать 3 аргумента
    popa
    addl $8, %esp           # убрать [vector][error_code=0]
    iret


# Общая часть для векторов с error code
isr_common_err:
    pusha
    movl %esp, %eax
    pushl %eax              # arg3: regs*
    movl 36(%esp), %ecx     # vector
    movl 40(%esp), %edx     # error_code (CPU положил)
    pushl %edx              # arg2
    pushl %ecx              # arg1
    call exception_handler
    addl $12, %esp
    popa
    addl $8, %esp           # убрать [vector][error_code]
    iret


.section .data
.global isr_stubs
isr_stubs:
    .long isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
    .long isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
    .long isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
    .long isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
