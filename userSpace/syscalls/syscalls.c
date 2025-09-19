#include "syscalls.h"
#include "../../vgaBufferTerminal/terminal.h"
#include "../ProcessModel.h"

ProcessModel process;

uint32_t sys_dispatch(uint32_t num, uint32_t a1, uint32_t a2, uint32_t a3) {
    (void)a2;(void)a3;

    switch (num) {
        case SYS_WRITE: terminal_write((char*)a1); return 0;
        case SYS_EXIT: process.state = P_EXITED; for (;;) {asm volatile("hlt");}
        default: return (uint32_t)-1;
    }
}