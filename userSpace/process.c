#include "process.h"
#include "../gdt/gdt_init.h"
#include "../innerStd/allocator.h"
#include "../vgaBufferTerminal/terminal.h"

// Ассемблерная функция для перехода в ring 3
__attribute__((naked)) 
void switch_to_user_mode(void* entry_point, void* user_stack) {
    (void)entry_point; (void)user_stack;
    asm volatile(
        "cli\n"
        "mov 8(%esp), %ecx\n"     // user_stack -> ECX
        "mov 4(%esp), %edx\n"     // entry_point -> EDX
        
        // Настраиваем сегменты данных пользователя
        "mov $0x23, %ax\n"        // USER_DATA_SEGMENT
        "mov %ax, %ds\n"
        "mov %ax, %es\n"
        "mov %ax, %fs\n"
        "mov %ax, %gs\n"
        
        // Создаем фрейм для IRET
        "push $0x23\n"            // SS
        "push %ecx\n"             // ESP
        "push $0x200\n"           // EFLAGS (IF=1)
        "push $0x1B\n"            // CS (USER_CODE_SEGMENT)
        "push %edx\n"             // EIP
        
        // Очищаем регистры
        "xor %eax, %eax\n"
        "xor %ebx, %ebx\n"
        "xor %ecx, %ecx\n"
        "xor %edx, %edx\n"
        "xor %esi, %esi\n"
        "xor %edi, %edi\n"
        "xor %ebp, %ebp\n"
        
        "iret\n"
    );
}