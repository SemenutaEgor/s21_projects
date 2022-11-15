#define _GNU_SOURCE
#include "s21_cat_funcs.h"

int main(int argc, char **argv) {

    const char* short_options = "benst";

    const struct option long_options[] = {
        {"number-nonblank", no_argument, NULL, 'b'},
        {"number", no_argument, NULL, 'n'},
        {"squeeze-blank", no_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };
    int flag_b, flag_e, flag_n, flag_s, flag_t;
   
    int optind = get_flags(short_options, long_options, argc, argv, &flag_b, &flag_e, &flag_n, &flag_s, &flag_t);
    flags_controller(optind, argc, argv, flag_b, flag_e, flag_n, flag_s, flag_t);
        
  return 0;
}
