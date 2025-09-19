BITS 32
GLOBAL _start

SECTION .text
_start:
    ; Вызов системного вызова write (номер = 0, строка = в EAX)
    mov eax, msg
    mov ebx, 0          ; a2, a3 пока не нужны
    mov ecx, 0
    mov edx, 0
    mov edi, 0          ; num = SYS_WRITE
    int 0x80           ; твой системный вызов

    ; Вызов SYS_EXIT
    mov edi, 1          ; num = SYS_EXIT
    int 0x80

SECTION .rodata
msg db "Hello from user mode!", 0
