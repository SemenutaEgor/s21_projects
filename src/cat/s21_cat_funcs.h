#ifndef SRC_CAT_S21_CAT_FUNCS_H_
#define SRC_CAT_S21_CAT_FUNCS_H_

#include <stddef.h>
#include <getopt.h>

typedef struct flags {
        int b;
        int e;
        int v;
        int E;
        int n;
        int s; 
        int t;
        int T;
    } dflag;

int get_flags(const char* short_options, const struct option long_options[], int argc, char **argv, dflag* flag);
void flags_controller(int optind, int argc, char** argv, dflag flag);
void output(char *filename);
int no_flags(dflag flag);
void flag_n(char *filename);
void flag_b(char *filename);

#endif //  SRC_CAT_S21_CAT_FUNCS_H_
