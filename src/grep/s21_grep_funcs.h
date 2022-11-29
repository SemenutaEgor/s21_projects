#ifndef SRC_GREP_S21_GREP_FUNCS_H_
#define SRC_GREP_S21_GREP_FUNCS_H_

#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
        int e;
        int i;
        int v;
        int c;
        int l; 
        int n;
        int h;
        int s;
        int f;
        int o;
    } dflag;

typedef struct string_buffer {
    char *data;
    ssize_t size;
} dbuf;

void print_result(int value);
int get_flags(const char* short_options, int argc, char **argv, dflag* flag, char *patters);
void files_controller(int optind, int argc, char** argv, dflag flag);
void flags_controller(FILE *src, dflag flag, regex_t *regeex, int *value);
void output();

#endif //  SRC_GREP_S21_GREP_FUNCS_H_
