#include "s21_grep_funcs.h"

int main(int argc, char **argv) {
  const char *short_options = "eivclnhsfo";

  dflag flag = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  int optind = get_flags(short_options, argc, argv, &flag);
  files_controller(optind, argc, argv, flag);

  return 0;
}
