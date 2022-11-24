#include "s21_grep_funcs.h"
#include <regex.h>

void print_result(int value) {
	if (value == 0) {
		printf("Pattern found\n");
	} else if (value == REG_NOMATCH) {
		printf("Pattern not found\n");
	} else {
		printf("An error occured\n");
	}
}

int main(int argc, char **argv) {
  const char *short_options = "eivclnhsfo";

  dflag flag = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  int optind = get_flags(short_options, argc, argv, &flag);
  files_controller(optind, argc, argv, flag);

  return 0;
}
