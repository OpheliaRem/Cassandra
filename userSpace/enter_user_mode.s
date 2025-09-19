.global enter_user_mode
.type enter_user_mode, @function

enter_user_mode:
    # entry -> 8(%esp)
    # user_stack_top -> 12(%esp)

    movw $0x23, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    pushl $0x23                # SS
    pushl 12(%esp)             # ESP (user_stack_top)
    pushfl
    popl %eax
    orl $0x200, %eax           # IF=1
    pushl %eax                 # EFLAGS
    pushl $0x1B                # CS
    pushl 8(%esp)              # EIP (entry)
    iret
