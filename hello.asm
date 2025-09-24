BITS 32

global _start

section .text
_start:
    mov ebx, 65
    mov eax, 0
    int 0x80

    ret
