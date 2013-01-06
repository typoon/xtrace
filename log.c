#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "xtrace.h"
#include "log.h"
#include "utils.h"

static FILE *general_log = NULL;

void do_log(char *fmt, ...) {
    char path[255];
    char *str_time;
    va_list args;

    memset(path, 0, sizeof(path));
    
    if(general_log == NULL) {
        sprintf(path, "%s/xtrace.log", log_dir);
        printf("Creating file %s\n", path);
        general_log = fopen(path, "a");
        if(general_log == NULL) {
            perror("File: ");
        }
        assert(general_log != NULL);
    }

    str_time = get_full_time();

    va_start(args, fmt);

    fprintf(general_log, "[%s] ", str_time);
    vfprintf(general_log, fmt, args);
    fprintf(general_log, "\n");

    va_end(args);

    free(str_time);
    
}

void do_log_time(char *str_time, char *fmt, ...) {
    char path[255];
    va_list args;
    
    if(general_log == NULL) {
        sprintf(path, "%s/xtrace.log", log_dir);
        printf("Creating file %s\n", path);
        general_log = fopen(path, "a");
        if(general_log == NULL) {
            perror("File: ");
        }
        assert(general_log != NULL);
    }

    va_start(args, fmt);

    fprintf(general_log, "[%s] ", str_time);
    vfprintf(general_log, fmt, args);
    fprintf(general_log, "\n");
    
    va_end(args);

}

void do_log_pad(int pad, char *fmt, ...) {
    int i;
    char path[255];
    char *str_time;
    va_list args;

    memset(path, 0, sizeof(path));
    
    if(general_log == NULL) {
        sprintf(path, "%s/xtrace.log", log_dir);
        printf("Creating file %s\n", path);
        general_log = fopen(path, "a");
        if(general_log == NULL) {
            perror("File: ");
        }
        assert(general_log != NULL);
    }

    str_time = get_full_time();

    va_start(args, fmt);

    fprintf(general_log, "[%s] ", str_time);
    
    for(i = 0; i < pad; i++) {
        fprintf(general_log, " ");
    }
    
    vfprintf(general_log, fmt, args);
    fprintf(general_log, "\n");

    va_end(args);

    free(str_time);
}

void do_log_time_pad(char *str_time, int pad, char *fmt, ...) {
    int i;
    char path[255];
    va_list args;
    
    if(general_log == NULL) {
        sprintf(path, "%s/xtrace.log", log_dir);
        printf("Creating file %s\n", path);
        general_log = fopen(path, "a");
        if(general_log == NULL) {
            perror("File: ");
        }
        assert(general_log != NULL);
    }

    va_start(args, fmt);

    fprintf(general_log, "[%s] ", str_time);

    for(i = 0; i < pad; i++) {
        fprintf(general_log, " ");
    }
    
    vfprintf(general_log, fmt, args);
    fprintf(general_log, "\n");
    
    va_end(args);

}

