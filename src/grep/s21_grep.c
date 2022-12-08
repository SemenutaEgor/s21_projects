#include "s21_grep_funcs.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    const char *short_options = "eivclnhsfo";

    dflag flag = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char *patterns = (char*)malloc(sizeof(char));

    int optind =
        get_flags(short_options, argc, argv, &flag, patterns);

    /*while (optind < argc) {
      printf("%s\n", argv[optind]);
      optind++;
    }*/
    printf("PATTERNS IN MAIN: %s\n", patterns);

    files_controller(optind, argc, argv, flag, patterns);

    /*regex_t reegex;
    int value;
    value = regcomp(&reegex, "test", 0);
    value = regexec(&reegex, "test string", 0, NULL, 0);
    print_result(value);*/

    free(patterns);
  } else {
    fprintf(stderr, "not enough arguments\n");
  }

  return 0;
}
