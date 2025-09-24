#include "syscalls.h"
#include "../../vgaBufferTerminal/terminal.h"
#include "../ProcessModel.h"

uint32_t syscall_handler(
    uint32_t vector,
    uint32_t arg0,
    uint32_t arg1,
    uint32_t arg2,
    uint32_t arg3,
    uint32_t arg4,
    uint32_t arg5
) {
    switch (vector) {
    case SYSCALL_WRITE_CHAR:
        (void)arg1;(void)arg2;(void)arg3;(void)arg4;(void)arg5;
        terminal_putchar((char)arg0);
        return 0;

    case SYSCALL_WRITE_STRING:
        (void)arg1;(void)arg2;(void)arg3;(void)arg4;(void)arg5;
        terminal_write("I have succeeded\n");

        terminal_write_hex(arg0);

        if (arg0) {
            terminal_write("the string is NOT null.\n");
        } else {
            terminal_write("the string IS null\n");
        }

        terminal_write((const char*)arg0);
        return 0;

    default:
        return (uint32_t)-1;
    }
}