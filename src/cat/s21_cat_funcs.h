#ifndef SRC_CAT_S21_CAT_FUNCS_H_
#define SRC_CAT_S21_CAT_FUNCS_H_

#include <stddef.h>
#include <getopt.h>

/*const char* short_options = "benst";

const struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {NULL, 0, NULL, 0}
};*/

int get_flags(const char* short_options, const struct option long_options[], int argc, char **argv, int* flag_b, int* flag_e, int* flag_n, int* flag_s, int* flag_t);
void flags_controller(int optind, int argc, char** argv, int flag_b, int flag_e, int flag_n, int flag_s, int flag_t);
void output(char *filename);
void flag_n_app(char *filename);

#endif //  SRC_CAT_S21_CAT_FUNCS_H_
