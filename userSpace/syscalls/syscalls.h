#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

enum {SYS_WRITE = 1, SYS_EXIT = 2};

uint32_t sys_dispatch(uint32_t num, uint32_t a1, uint32_t a2, uint32_t a3);

#endif