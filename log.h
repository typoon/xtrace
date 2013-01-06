#ifndef XT_LOG_H
#define XT_LOG_H

void do_log(char *fmt, ...);
void do_log_time(char *time, char *fmt, ...);
void do_log_pad(int pad, char *fmt, ...);
void do_log_time_pad(char *str_time, int pad, char *fmt, ...);

#endif // XT_LOG_H
