#include "s21_grep_funcs.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    const char *short_options = "eivclnhsfo";

    dflag flag = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char *patterns = malloc(sizeof(char));
    strcpy(patterns, "");
    char *patfiles = malloc(sizeof(char));
    strcpy(patfiles, "");

    int optind =
        get_flags(short_options, argc, argv, &flag, patterns, patfiles);

    /*while (optind < argc) {
      printf("%s\n", argv[optind]);
      optind++;
    }*/

    files_controller(optind, argc, argv, flag, patterns/*, patfiles*/);

    /*regex_t reegex;
    int value;
    value = regcomp(&reegex, "test", 0);
    value = regexec(&reegex, "test string", 0, NULL, 0);
    print_result(value);*/

    free(patterns);
    free(patfiles);
  } else {
    fprintf(stderr, "not enough arguments\n");
    exit(0);
  }

  return 0;
}
