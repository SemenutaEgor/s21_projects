#ifndef SRC_GREP_S21_GREP_FUNCS_H_
#define SRC_GREP_S21_GREP_FUNCS_H_

#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
void files_controller(int optind, int argc, char** argv, dflag flag, char *patterns);
void flags_controller(FILE *src, dflag flag, regex_t *regex, int *result, char *filename, int multifile);
void print_regerror (int errcode, size_t length, regex_t *compiled);
int compile(regex_t *regex, dflag flag, char *patterns);
int multifile_check(int argc, int optind);
void output(regex_t *regex, int *result, dbuf buffer, char *filename, int multifile);

#endif //  SRC_GREP_S21_GREP_FUNCS_H_
