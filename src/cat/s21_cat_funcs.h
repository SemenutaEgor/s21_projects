#ifndef SRC_CAT_S21_CAT_FUNCS_H_
#define SRC_CAT_S21_CAT_FUNCS_H_

#include <stddef.h>
#include <getopt.h>

int get_flags(const char* short_options, const struct option long_options[], int argc, char **argv, int* flag_b, int* flag_e, int* flag_n, int* flag_s, int* flag_t);
void flags_controller(int optind, int argc, char** argv, int flag_b, int flag_e, int flag_n, int flag_s, int flag_t);
void output(char *filename);
void flag_n_app(char *filename);
void flag_b_app(char *filename);

#endif //  SRC_CAT_S21_CAT_FUNCS_H_
