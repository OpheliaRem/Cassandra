#ifndef GDT_INIT
#define GDT_INIT

enum {
    NULL_SEGMENT,
    KERNEL_CODE,
    KERNEL_DATA,
    USER_CODE,
    USER_DATA,
};

#define USER_CODE_SEGMENT (USER_CODE * 8 | 3)
#define USER_DATA_SEGMENT (USER_DATA * 8 | 3)

void init_gdt();

#endif