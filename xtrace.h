#ifndef XT_XTRACE_H
#define XT_XTRACE_H

#include "simple_map.h"

#if __WORDSIZE == 64
    #define OFFSET (8)
    #define ORIG_EAX ORIG_RAX
    #define EAX RAX
    #define EBX RBX
    #define ECX RCX
    #define EDX RDX
    #define EIP RIP
    #define ESP RSP
    #define ESI RSI
    #define EBP RBP
#else
    #define OFFSET (4)
#endif

#if __WORDSIZE == 64
    #define SYSCALL_ARG1 (OFFSET * RDI)
    #define SYSCALL_ARG2 (OFFSET * RSI)
    #define SYSCALL_ARG3 (OFFSET * RDX)
    #define SYSCALL_ARG4 (OFFSET * R10)
    #define SYSCALL_ARG5 (OFFSET * R8 )
    #define SYSCALL_ARG6 (OFFSET * R9 )
    #define SYSCALL_RET  (OFFSET * RAX)
#else    
    #define SYSCALL_ARG1 (OFFSET * EBX)
    #define SYSCALL_ARG2 (OFFSET * ECX)
    #define SYSCALL_ARG3 (OFFSET * EDX)
    #define SYSCALL_ARG4 (OFFSET * ESI)
    #define SYSCALL_ARG5 (OFFSET * EDI)
    #define SYSCALL_ARG6 (OFFSET * EBP)
    #define SYSCALL_RET  (OFFSET * EAX)
#endif

typedef union _data_u {
    long val;
    char data[OFFSET];
} data_u;

simple_map *open_fds;
char log_dir[255];


#endif //XT_XTRACE_H
