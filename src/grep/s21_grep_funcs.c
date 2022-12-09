#include "s21_grep_funcs.h"

static char *add_to_string(char *string, char *word) {
  char *new_string = NULL;
  if (string) {
    size_t new_length = strlen(string) + strlen(word) + 2;
    new_string = realloc(string, sizeof(char) * new_length);
    if (new_string) {
      if (strlen(new_string)) {
        strcat(new_string, "|");
        strcat(new_string, word);
      }
    } else {
      fprintf(stderr, "Error with allocating memory for patterns\n");
    }
  } else {
    size_t new_length = strlen(word);
    new_string = malloc(sizeof(char) * new_length + 1);
    if (new_string) {
      strcpy(new_string, word);
    } else {
      fprintf(stderr, "Error with allocating memory for patterns\n");
    }
  }
  //printf("NEW WORD: %s\n", word);
  //printf("NEW PATTERNS: %s\n", new_string);
  return new_string;
}

static char *load_patterns(char *patterns, char *filename, dflag flag) {
  FILE *patfile;
  patfile = fopen(filename, "r");
  if (patfile) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size = getline(&line_buf, &line_buf_size, patfile);
    while (line_size >= 0) {
      //printf("line_buf before cut: %s\n", line_buf);
      //line_buf = cut_pattern(line_buf);
      line_buf[strcspn(line_buf, "\n")] = 0;
      if (!line_buf) {
        return NULL;
      }
      //printf("line_buf after cut: %s\n", line_buf);
      patterns = add_to_string(patterns, line_buf);
      line_size = getline(&line_buf, &line_buf_size, patfile);
    }
    free(line_buf);
    line_buf = NULL;
    fclose(patfile);
  } else if (!flag.s) {
    fprintf(stderr, "No such file or directory '%s'\n", filename);
  }
  return patterns;
}

int get_flags(const char *short_options, int argc, char **argv, dflag *flag,
              char **patterns) {
  int res;
  while ((res = getopt_long(argc, argv, short_options, 0, 0)) != -1) {
    switch (res) {
      case 'e': {
        flag->e++;
        //printf("optind before e = %d\n", optind);
        //printf("patterns before e = %s\n", patterns);
        *patterns = add_to_string(*patterns, argv[optind]);
        *(argv + optind) = NULL;
        optind++;
        //printf("optind after e = %d\n", optind);
        //printf("patterns arter e = %s\n", patterns);
        break;
      }
      case 'i': {
        flag->i = 1;
        break;
      }
      case 'v': {
        flag->v = 1;
        break;
      }
      case 'c': {
        flag->c = 1;
        break;
      }
      case 'l': {
        flag->l = 1;
        break;
      }
      case 'n': {
        flag->n = 1;
        break;
      }
      case 'h': {
        flag->h = 1;
        break;
      }
      case 's': {
        flag->s = 1;
        break;
      }
      case 'f': {
        flag->f = 1;
        //printf("optind before f = %d\n", optind);
        //printf("patterns before f = %s\n", patterns);
        *patterns = load_patterns(*patterns, argv[optind], *flag);
        *(argv + optind) = NULL;
        optind++;
        //printf("optind after f = %d\n", optind);
        //printf("patterns arter f = %s\n", *patterns);
        break;
      }
      case 'o': {
        flag->o = 1;
        break;
      }

      case '?':
      default: {
        printf("found unknown option");
        break;
      }
    }
  }
  /*for (size_t i = 0; i < strlen(patterns); i++) {
    putchar(patterns[i]);
  }*/
  /*printf("!!!!!!!!!!!!!!!!!!!!\n");
  //optind = optind + flag->e + flag->f;
  int test_optind = optind;
  while (test_optind < argc) {
      printf("%s\n", *(argv + test_optind));
    test_optind++;
  }
  printf("!!!!!!!!!!!!!!!!!!!!\n");*/
  return optind;
}

int compile(regex_t *regex, dflag flag, char *patterns) {
  int cflags = REG_EXTENDED;
  if (flag.i) {
    cflags = REG_EXTENDED | REG_ICASE;
  }
  return regcomp(regex, patterns, cflags);
}

void files_controller(int optind, int argc, char **argv, dflag flag,
                      char *patterns) {
  FILE *src;
  regex_t regex;
  int result, multifile = 0;
  if (!patterns) {
    patterns = add_to_string(patterns, argv[optind++]);
  }

  //printf("PATTERNS: %s\n", patterns);
  
  result = compile(&regex, flag, patterns);
  if (result) {
    if (result == REG_ESPACE) {
      fprintf(stderr, "%s\n", strerror(ENOMEM));
    } else {
      fprintf(stderr, "Syntax error in the regular expression %s\n", patterns);
    }
    return;
  }
  
  if (argc - optind > 1) {
    multifile = 1;
  }
  
  //printf("optind = %d\n", optind);
  //printf("argc = %d\n", argc);
  while (optind < argc) {
    //printf("argv[optind]= %s\n", argv[optind]);
    src = fopen(argv[optind], "r");
    if (src) {
      flags_controller(src, flag, &regex, &result, argv[optind], multifile);
      fclose(src);
    } else if (!flag.s) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[optind]);
    }
  optind++;
  }
  regfree(&regex);
}

void flags_controller(FILE *src, dflag flag, regex_t *regex, int *result,
                      char *filename, int multifile) {
  int line_counter = 0, output_suppress = 0, file_match = 0, line = 1;
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  ssize_t line_size = getline(&line_buf, &line_buf_size, src);
  regmatch_t match;
  *result = regexec(regex, line_buf, 1, &match, 0);
  while (line_size >= 0) {
    if (flag.c && !(*result)) {
      line_counter++;
      output_suppress = 1;
    }
    if (flag.l && !(*result)) {
      output_suppress = 1;
      file_match = 1;
    }
    if (!output_suppress) {
      output(regex, result, line_buf, filename, multifile, flag, line, match);
    }
    line_size = getline(&line_buf, &line_buf_size, src);
    *result = regexec(regex, line_buf, 1, &match, 0);
    line++;
  }
  if (flag.c) {
    output_c(line_counter, filename, multifile);
  }
  if (flag.l && file_match) {
    printf("%s\n", filename);
    file_match = 0;
  }
  free(line_buf);
  line_buf = NULL;
}

void output_c(int line_counter, char *filename, int multifile) {
  if (multifile > 1) {
    printf("%s:%d\n", filename, line_counter);
  } else {
    printf("%d\n", line_counter);
  }
}

void output(regex_t *regex, int *result, char *line_buf, char *filename,
            int multifile, dflag flag, int line, regmatch_t match) {
  if (flag.v) {
    *result = !(*result);
  }
  if (!(*result)) {
    if (multifile && !flag.h) {
      printf("%s:", filename);
    }
    if (flag.n) {
      printf("%d:", line);
    }
    if (flag.o) {
      printf("%.*s\n", (int)(match.rm_eo - match.rm_so), line_buf + match.rm_so);
    } else {
      printf("%s", line_buf);
    }
    if (!strstr(line_buf, "\n")) {
      printf("\n");
    }
  } else if (*result != REG_NOMATCH) {
    size_t length = regerror(*result, regex, NULL, 0);
    print_regerror(*result, length, regex);
    return;
  }
}

void print_regerror(int errcode, size_t length, regex_t *compiled) {
  char buffer[length];
  regerror(errcode, compiled, buffer, length);
  fprintf(stderr, "Regex match failed: %s\n", buffer);
}
