// syscalls.c
#include "syscall.h"
#include "../user_memory.h"
#include "../../vgaBufferTerminal/terminal.h"
#include "../process.h"
#include "../../innerStd/io.h"

uint32_t syscall_handler(uint32_t vector, uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5) {
    (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5;
    
    switch (vector) {
    case SYSCALL_WRITE_CHAR:
        putchar((char)arg0);
        return 0;
        
    case SYSCALL_WRITE_STRING: {
        if (arg0 == 0) {
            terminal_write("(null)");
            return 0;
        }
        
        // Буфер в стеке ядра для безопасности
        char buffer[256];
        int result = copy_string_from_user(buffer, (const char*)arg0, sizeof(buffer));
        
        if (result >= 0) {
            terminal_write(buffer);
            return 0;
        } else {
            terminal_write("[Invalid user pointer]");
            return (uint32_t)-1; // EFAULT
        }
    }
    
    case SYSCALL_EXIT: {
        terminal_write("\nProcess exited with code: ");
        terminal_write_hex(arg0);
        terminal_writeln("");
        
        // Здесь будет завершение процесса
        // Пока просто переходим обратно в ядро
        asm volatile(
            "mov $0x10, %ax\n"    // Сегмент данных ядра
            "mov %ax, %ds\n"
            "mov %ax, %es\n" 
            "mov %ax, %fs\n"
            "mov %ax, %gs\n"
            "mov %ax, %ss\n"
            "mov %esp, %ebp\n"    // Восстанавливаем стек ядра
        );
        
        // Не возвращаемся к пользовательскому коду
        while(1) {
            asm volatile("hlt");
        }
    }
    
    case SYSCALL_READ_CHAR:
        // Простой пример - всегда возвращает 'A'
        // В реальности здесь будет ожидание ввода
        return 'A';
        
    default:
        terminal_write("Unknown syscall: ");
        terminal_write_hex(vector);
        terminal_putchar('\n');
        return (uint32_t)-1; // ENOSYS
    }
}