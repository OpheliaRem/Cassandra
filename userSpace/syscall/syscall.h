#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

// syscalls.h - добавить новые системные вызовы
enum {
    SYSCALL_WRITE_CHAR = 0,
    SYSCALL_WRITE_STRING = 1,
    SYSCALL_EXIT = 2,
    SYSCALL_READ_CHAR = 3,
};

uint32_t sys_dispatch(uint32_t, uint32_t, uint32_t, uint32_t);

#endif