#ifndef XT_SYSCALL_HANDLERS_H
#define XT_SYSCALL_HANDLERS_H

#include <sys/types.h>

void handle_sys_write(pid_t pid_child, int *in_syscall);
void handle_sys_open(pid_t pid_child, int *in_syscall);
void handle_sys_close(pid_t pid_child, int *in_syscall);
void handle_sys_exceve(pid_t pid_child, int *in_syscall);

#endif //XT_SYSCALL_HANDLERS_H
