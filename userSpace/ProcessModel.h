#ifndef PROCESS_MODEL
#define PROCESS_MODEL

#include <stdint.h>

typedef struct {
    uint32_t pid;
    uint32_t cr3;            // физ. адр. каталога страниц
    uintptr_t entry;         // точка входа ELF
    uintptr_t user_stack_top;// виртуальный адрес вершины стека
    enum { P_NEW, P_RUNNING, P_EXITED } state;
} ProcessModel;

extern ProcessModel process;

#endif