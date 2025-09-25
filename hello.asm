BITS 32

global _start

section .text
_start:
    ; Данные встроены в код
    jmp .start_code
    
.hello_msg db "Hello from user mode!", 0xA, 0

.start_code:
    ; Получаем адрес сообщения (EIP-relative)
    call .get_ip
.get_ip:
    pop ebx
    lea ecx, [ebx + .hello_msg - .get_ip]
    
    mov eax, 1      ; SYSCALL_WRITE_STRING
    mov ebx, ecx
    int 0x80
    
    mov eax, 2      ; SYSCALL_EXIT
    mov ebx, 0
    int 0x80