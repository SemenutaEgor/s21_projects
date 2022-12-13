#define _GNU_SOURCE
#include "s21_cat_funcs.h"

int main(int argc, char **argv) {
  const char *short_options = "benstET";

  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  dflag flag = {0, 0, 0, 0, 0, 0, 0, 0};

  int optind = get_flags(short_options, long_options, argc, argv, &flag);
  files_controller(optind, argc, argv, flag);

  return 0;
}
