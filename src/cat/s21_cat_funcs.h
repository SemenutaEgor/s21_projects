#ifndef SRC_CAT_S21_CAT_FUNCS_H_
#define SRC_CAT_S21_CAT_FUNCS_H_

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct flags {
        int b;
        int e;
        int E;
        int n;
        int s; 
        int t;
        int T;
    } dflag;

typedef struct string_buffer {
    char *data;
    int empty;
    int number;
    ssize_t size;
} dbuf;

int get_flags(const char* short_options, const struct option long_options[], int argc, char **argv, dflag* flag);
void files_controller(int optind, int argc, char** argv, dflag flag);
void flags_controller(FILE *src, dflag flag, int *prev_empty, int *all_count, int *non_empty_count, int *new_line);
void restart_prev_empty(int curr_empty, int *prev_empty);
void output(dbuf buffer, int squeeze, int prev_empty, int *new_line);
void flag_v(dbuf *buffer);
void flag_e(dbuf *buffer);
void flag_t(dbuf *buffer);
void flag_s(dbuf *buffer);
void flag_b(dbuf *buffer, int *non_empty_count, int new_line);
void flag_n(dbuf *buffer, int *all_count, int squeeze, int prev_empty, int new_line);

#endif //  SRC_CAT_S21_CAT_FUNCS_H_
