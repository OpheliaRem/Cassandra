#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

enum SyscallVector {
    SYSCALL_WRITE_CHAR,
    SYSCALL_WRITE_STRING,
};

uint32_t sys_dispatch(uint32_t, uint32_t, uint32_t, uint32_t);

#endif