#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/ptrace.h>
#include <asm/ptrace-abi.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <libgen.h>
#include "xtrace.h"
#include "syscall_handlers.h"
#include "simple_map.h"

void usage(char **argv) {
    printf("Use: %s <binary>\n", argv[0]);
}

/**
 * This function is responsible for printing all information related to the
 * syscalls that are made by the execution of a program.
 * It will print the syscall number, syscall name, parameters received and
 * return value after the syscall has finished
 */
void print_syscall_info(pid_t pid_child, int *in_syscall) {

    //int status;
    long syscall;

    syscall = ptrace(PTRACE_PEEKUSER, pid_child, OFFSET * ORIG_EAX, NULL);

    switch(syscall) {
        
        case SYS_write:
            handle_sys_write(pid_child, in_syscall);
        break;

        case SYS_open:
            //handle_sys_open(pid_child, in_syscall);
        break;

        case SYS_execve:
            handle_sys_exceve(pid_child, in_syscall);
            // Gotta ask the user if he wants this syscall to execute or not
        break;

        case SYS_close:
            handle_sys_close(pid_child, in_syscall);
        break;
/*
        default:
            
            printf("Unhandled syscall %ld\n", syscall);
            if(*in_syscall == -1) *in_syscall = syscall;
            else                  *in_syscall = -1;
            */
    }

}

int main(int argc, char **argv) {

    // Flags used to determine what to show to the user
    char show_syscall = 1;

    // Control variables
    int in_syscall = -1;

    int status;
    //int signal;
    pid_t pid_child;

    if(argc < 2) {
        usage(argv);
        return -1;
    }

    // TODO: stat the file here to make sure it is a valid binary

    pid_child = fork();

    if(pid_child == 0) { // This is the child process
        printf("Starting the child process...\n");
        ptrace(PTRACE_TRACEME, pid_child, NULL, NULL);
        execv(argv[1], &argv[1]);
        return 0; // never reached, right? :)
    }

    open_fds = simple_map_new(NULL);

    // create the directory that will hold the log files
    memset(log_dir, 0, sizeof(log_dir));
    sprintf(log_dir, "/tmp/%s-%d", basename(argv[1]), pid_child);
    mkdir(log_dir, 0777);

    while(1) {
        //printf("Waiting child to signal something...\n");
        wait(&status);

        /*
        if(WIFSTOPPED(status)) {
            signal = WSTOPSIG(status);
            if(signal == 11) {
                printf("sisegv?\n");
            }
            //printf("Stopped by signal %d\n", signal);
        }
        */

        if(WIFEXITED(status)) {
            printf("Child process exited (%d)... we will exit too!\n", (status & 0xFF));
            break;
        }

        if(show_syscall) {
            print_syscall_info(pid_child, &in_syscall);
        }


        ptrace(PTRACE_SYSCALL, pid_child, NULL, NULL);
    }

    simple_map_free(open_fds);

    return 0;

}
