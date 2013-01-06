#include <stdlib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "xtrace.h"
#include "utils.h"

void read_ascii_data(pid_t pid_child, long from, char **buf) {

    int i, j;
    int size = -1;
    data_u d;

    *buf = NULL;

    for(i = 0; ;i++) {
        d.val = ptrace(PTRACE_PEEKDATA, pid_child, from + (i * OFFSET), NULL);

        for(j = 0; j < OFFSET; j++) {
            if(d.data[j] == 0x00) {
                //size = OFFSET + (i * OFFSET) - (OFFSET - j) + 1;
                size = OFFSET + (i * OFFSET) + 1;
            }
        }

        if(size != -1) {
            break;
        }
    }

    if(size != -1) {
        *buf = (char *)malloc(sizeof(char) * size);

        for(i = 0, j = 0; ; i++) {
            d.val = ptrace(PTRACE_PEEKDATA, pid_child, from + (i * OFFSET), NULL);
            memcpy(&((*buf)[j]), d.data, OFFSET);
            j += OFFSET;

            if(j >= size)
                break;
        }
    }
}

void read_bin_data(pid_t pid_child, long from, char **buf, int size) {

    int i;
    data_u d;

    if(size > 0) {
        *buf = (char *)malloc(sizeof(char) * size);

        for(i = 0; i < size ; i++) {
            d.val = ptrace(PTRACE_PEEKDATA, pid_child, from + i, NULL);
            //(*buf)[i] = d.data[0];
            memcpy(&((*buf)[i]), d.data, 1);
        }
    }
}

/**
 * Returns the current date/time in the format: YYYYMMDD HH24:MI:SS:mmm
 * Where:
 *  YYYY - Year with 4 digits
 *  MM - Month from 01 to 12
 *  DD - Day from 01 to 31
 *  HH24 - Hour from 00 to 23
 *  MI - Minutes from 00 to 59
 *  SS - Seconds from 00 to 59
 *  mmm - Milliseconds from 000 to 999
 *
 * The returned pointer should be freed by the calling function
 * 
 */
char *get_full_time() {

    const int time_len = 32;
    char *str_time;
    struct timeval tv;
    struct tm *tm;
    time_t t;


    str_time = (char *)malloc(time_len);

    assert(str_time != NULL);
    memset(str_time, 0, time_len);

    t = time(NULL);
    tm = localtime(&t);
    gettimeofday(&tv, NULL);
    
    sprintf(str_time, "%4d-%02d-%02d %02d:%02d:%02d:%03d" , tm->tm_year + 1900
                                                        , tm->tm_mon + 1
                                                        , tm->tm_mday
                                                        , tm->tm_hour
                                                        , tm->tm_min
                                                        , tm->tm_sec
                                                        , (int)(tv.tv_usec/1000));
    return str_time;

}
