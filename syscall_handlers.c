#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <fcntl.h>

#include "xtrace.h"
#include "syscall_handlers.h"
#include "utils.h"
#include "log.h"

void handle_sys_exceve(pid_t pid_child, int *in_syscall) {

    long r_eip;
    data_u d;

    if(*in_syscall == -1) {
        //*in_syscall = SYS_execve;

        r_eip = ptrace(PTRACE_PEEKUSER, pid_child, EIP * OFFSET, NULL);
        printf("r_eip = %08x", (unsigned int)r_eip);

        int i = 0;
        for(i = 0; i < 4; i++) {
            d.val = ptrace(PTRACE_PEEKDATA, pid_child, r_eip + (i * OFFSET), NULL);
            printf("%02X ", (unsigned char)d.data[0]);
            printf("%02X ", (unsigned char)d.data[1]);
            printf("%02X ", (unsigned char)d.data[2]);
            printf("%02X ", (unsigned char)d.data[3]);
            printf("%02X ", (unsigned char)d.data[4]);
            printf("%02X ", (unsigned char)d.data[5]);
            printf("%02X ", (unsigned char)d.data[6]);
            printf("%02X", (unsigned char)d.data[7]);
            
        }
        
    } else {
        //*in_syscall = -1;
    }
    
}

void handle_sys_write(pid_t pid_child, int *in_syscall) {

    int c = 1;

    long i;

    char *output;
    char *str_time;

    long r_fd;
    long r_buf;
    long r_count;
    long r_ret;

    char bin_data_path[255];
    char ascii_data_path[255];
    FILE *f_bin_data;
    FILE *f_ascii_data;

    memset(bin_data_path,   0, sizeof(bin_data_path));
    memset(ascii_data_path, 0, sizeof(ascii_data_path));

    sprintf(bin_data_path,   "%s/write-bin-%d",   log_dir, pid_child);
    sprintf(ascii_data_path, "%s/write-ascii-%d", log_dir, pid_child);

    f_bin_data   = fopen(bin_data_path,   "a");
    f_ascii_data = fopen(ascii_data_path, "a");

    str_time = get_full_time();
    
    if(*in_syscall == -1) {
        *in_syscall = SYS_write;

        r_fd    = ptrace(PTRACE_PEEKUSER, pid_child, SYSCALL_ARG1, NULL);
        r_buf   = ptrace(PTRACE_PEEKUSER, pid_child, SYSCALL_ARG2, NULL);
        r_count = ptrace(PTRACE_PEEKUSER, pid_child, SYSCALL_ARG3, NULL);

        do_log_time(str_time, "write(%ld, 0x%lX, %ld);", r_fd, r_buf, r_count);
        
        //read_bin_data(pid_child, r_buf, &output, r_count);

        output = (char *)malloc(r_count);
        memset(output, 0, r_count);

        fprintf(f_bin_data,   "---- Start - %s ----\n", str_time);
        fprintf(f_ascii_data, "---- Start - %s ----\n", str_time);

        for(i = 0; i < r_count; i++) {
            fprintf(f_bin_data, "%02X", output[i]);
            
            if((c % 16) == 0) {
                fprintf(f_bin_data, "\n");
                c = 1;
            } else {
                fprintf(f_bin_data, " ");
                c++;
            }
            
            fwrite(&output[i], 1, 1, f_ascii_data);
        }

        fprintf(f_bin_data,   "\n---- End   - %s ----\n", str_time);
        fprintf(f_ascii_data, "\n---- End   - %s ----\n", str_time);
        
        do_log_time(str_time, "\tbuf (0x%lX) = Check files %s and %s", r_buf, ascii_data_path, bin_data_path);
        free(output);
        
    } else {
        *in_syscall = -1;

        r_ret = ptrace(PTRACE_PEEKUSER, pid_child, SYSCALL_RET, NULL);
        do_log_time(str_time, "\twrite returned: %ld", r_ret);
    }

    fclose(f_bin_data);
    fclose(f_ascii_data);
    free(str_time);
}

void handle_sys_open(pid_t pid_child, int *in_syscall) {

    char *pathname = NULL;

    long r_pathname;
    long r_flags;
    long r_mode;
    long r_ret;

    char sep = ' ';

    if(*in_syscall == -1) {
        *in_syscall = SYS_open;

        r_pathname = ptrace(PTRACE_PEEKUSER, pid_child, SYSCALL_ARG1, NULL);
        r_flags    = ptrace(PTRACE_PEEKUSER, pid_child, SYSCALL_ARG2, NULL);
        r_mode     = ptrace(PTRACE_PEEKUSER, pid_child, SYSCALL_ARG3, NULL);

        if((r_flags & O_CREAT) == O_CREAT) {
            do_log("open(0x%lX, %ld, %ld);", r_pathname, r_flags, r_mode);
        } else {
            do_log("open(0x%lX, %ld);", r_pathname, r_flags);
        }

        read_ascii_data(pid_child, r_pathname, &pathname);
        
        if(pathname != NULL) {
            do_log("\tpathname (0x%lX) = %s", r_pathname, pathname);
            free(pathname);
        }

        do_log("\tflags (%ld) =", r_flags);
        if((r_flags & O_RDONLY) == O_RDONLY) {
            do_log("%c O_RDONLY ", sep);
            sep = '|';
        }

        if((r_flags & O_WRONLY) == O_WRONLY) {
            do_log("%c O_WRONLY ", sep);
            sep = '|';
        }

        if((r_flags & O_RDWR) == O_RDWR) {
            do_log("%c O_RDWR ", sep);
            sep = '|';
        }

        if((r_flags & O_APPEND) == O_APPEND) {
            do_log("%c O_APPEND ", sep);
            sep = '|';
        }

        if((r_flags & O_ASYNC) == O_ASYNC) {
            do_log("%c O_ASYNC ", sep);
            sep = '|';
        }

        if((r_flags & O_CLOEXEC) == O_CLOEXEC) {
            do_log("%c O_CLOEXEC ", sep);
            sep = '|';
        }

        if((r_flags & O_CREAT) == O_CREAT) {
            do_log("%c O_CREAT ", sep);
            sep = '|';
        }

        /*
        if((r_flags & O_DIRECT) == O_DIRECT) {
            printf("%c O_DIRECT ", sep);
            sep = '|';
        }
        */

        if((r_flags & O_DIRECTORY) == O_DIRECTORY) {
            do_log("%c O_DIRECTORY ", sep);
            sep = '|';
        }

        if((r_flags & O_EXCL) == O_EXCL) {
            do_log("%c O_EXCL ", sep);
            sep = '|';
        }

        /*
        if((r_flags & O_LARGEFILE) == O_LARGEFILE) {
            printf("%c O_LARGEFILE ", sep);
            sep = '|';
        }

        if((r_flags & O_NOATIME) == O_NOATIME) {
            printf("%c O_NOATIME ", sep);
            sep = '|';
        }
        */

        if((r_flags & O_NOCTTY) == O_NOCTTY) {
            do_log("%c O_NOCTTY ", sep);
            sep = '|';
        }

        if((r_flags & O_NOFOLLOW) == O_NOFOLLOW) {
            do_log("%c O_NOFOLLOW ", sep);
            sep = '|';
        }

        if((r_flags & O_NONBLOCK) == O_NONBLOCK) {
            do_log("%c O_NONBLOCK ", sep);
            sep = '|';
        }

        if((r_flags & O_SYNC) == O_SYNC) {
            do_log("%c O_SYNC ", sep);
            sep = '|';
        }

        if((r_flags & O_TRUNC) == O_TRUNC) {
            do_log("%c O_TRUNC ", sep);
            sep = '|';
        }


        if((r_flags & O_CREAT) == O_CREAT) {

            do_log("\tmode (%ld) =", r_mode);
            sep = ' ';

            if((r_mode & S_IRWXU) == S_IRWXU) {
                do_log("%c S_IRWXU ", sep);
                sep = '|';
            }

            if((r_mode & S_IRUSR) == S_IRUSR) {
                do_log("%c S_IRUSR ", sep);
                sep = '|';
            }

            if((r_mode & S_IWUSR) == S_IWUSR) {
                do_log("%c S_IWUSR ", sep);
                sep = '|';
            }

            if((r_mode & S_IXUSR) == S_IXUSR) {
                do_log("%c S_IXUSR ", sep);
                sep = '|';
            }

            if((r_mode & S_IRWXG) == S_IRWXG) {
                do_log("%c S_IRWXG ", sep);
                sep = '|';
            }

            if((r_mode & S_IRGRP) == S_IRGRP) {
                do_log("%c S_IRGRP ", sep);
                sep = '|';
            }

            if((r_mode & S_IWGRP) == S_IWGRP) {
                do_log("%c S_IWGRP ", sep);
                sep = '|';
            }

            if((r_mode & S_IXGRP) == S_IXGRP) {
                do_log("%c S_IXGRP ", sep);
                sep = '|';
            }

            if((r_mode & S_IRWXO) == S_IRWXO) {
                do_log("%c S_IRWXO ", sep);
                sep = '|';
            }

            if((r_mode & S_IROTH) == S_IROTH) {
                do_log("%c S_IROTH ", sep);
                sep = '|';
            }

            if((r_mode & S_IWOTH) == S_IWOTH) {
                do_log("%c S_IWOTH ", sep);
                sep = '|';
            }

            if((r_mode & S_IXOTH) == S_IXOTH) {
                do_log("%c S_IXOTH ", sep);
                sep = '|';
            }

        }
    } else {
        *in_syscall = -1;
        r_ret = ptrace(PTRACE_PEEKUSER, pid_child, SYSCALL_RET, NULL);
        do_log("\topen returned: %ld", r_ret);

        if(r_ret > 0) {
            r_pathname = ptrace(PTRACE_PEEKUSER, pid_child, SYSCALL_ARG1, NULL);
            read_ascii_data(pid_child, r_pathname, &pathname);

            simple_map_add(open_fds, r_ret, pathname);
        }
        
    }
}

void handle_sys_close(pid_t pid_child, int *in_syscall) {

    long r_fd;

    r_fd = ptrace(PTRACE_PEEKUSER, pid_child, SYSCALL_ARG1, NULL);

    if(*in_syscall == -1) {
        *in_syscall = SYS_close;
        if(simple_map_key_exists(open_fds, r_fd))
            do_log("close(%ld); // (File: %s)", r_fd, (char *)simple_map_get(open_fds, r_fd));
        else
            do_log("close(%ld); // (File: Unknown)", r_fd);
    } else {
        *in_syscall = -1;
        simple_map_rem(open_fds, r_fd);
    }

}
