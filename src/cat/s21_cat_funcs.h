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
void flags_controller(FILE *src, dflag flag);
void output(dbuf buffer, int squeeze, int prev_empty);
int no_flags(dflag flag);
void flag_n(char *source_file);
void flag_b(char *source_file);
void flag_v(char *source_file);
void flag_e(char *source_file);

#endif //  SRC_CAT_S21_CAT_FUNCS_H_
