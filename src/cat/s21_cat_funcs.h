#ifndef SRC_CAT_S21_CAT_FUNCS_H_
#define SRC_CAT_S21_CAT_FUNCS_H_

#include <stddef.h>
#include <getopt.h>

typedef struct flags {
        int b;
        int e;
        int E;
        int n;
        int s; 
        int t;
        int T;
    } dflag;

int get_flags(const char* short_options, const struct option long_options[], int argc, char **argv, dflag* flag);
void flags_controller(int optind, int argc, char** argv, dflag flag);
void output(char *source_file);
int no_flags(dflag flag);
void flag_n(char *source_file);
void flag_b(char *source_file);
void flag_v(char *source_file);
void flag_e(char *source_file);

#endif //  SRC_CAT_S21_CAT_FUNCS_H_
