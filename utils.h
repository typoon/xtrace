#ifndef XT_UTILS_H
#define XT_UTILS_H

void read_ascii_data(pid_t pid_child, long from, char **buf);
void read_bin_data(pid_t pid_child, long from, char **buf, int size);
char *get_full_time();

#endif //XT_UTILS_H
