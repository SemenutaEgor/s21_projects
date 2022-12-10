#include "s21_grep_funcs.h"

/*add new pattern to string of patterns*/
static char *add_to_string(char *string, const char *word) {
  char *new_string = NULL;
  if (string) { /*if string of patterns wasn't empty*/
    size_t new_length = strlen(string) + strlen(word) + 2;
    new_string = realloc(string, sizeof(char) * new_length);
    if (new_string) {
      if (strlen(new_string)) {
        strcat(new_string, "|");
        strcat(new_string, word);
      }
    } else {
      fprintf(stderr, "s21_grep: error with allocating memory for patterns\n");
    }
  } else { /*if string of patterns was empty*/
    size_t new_length = strlen(word);
    new_string = (char*)malloc(sizeof(char) * new_length + 1);
    if (new_string) {
      strcpy(new_string, word);
    } else {
      fprintf(stderr, "s21_grep: error with allocating memory for patterns\n");
    }
  }
  return new_string;
}

/*load patterns from file into string of patterns*/
static char *load_patterns(char *patterns, const char *filename, const dflag flag) {
  FILE *patfile;
  patfile = fopen(filename, "r");
  if (patfile) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size = getline(&line_buf, &line_buf_size, patfile);
    while (line_size >= 0) {
      line_buf[strcspn(line_buf, "\n")] = 0; /*remove \n from line with pattern*/
      if (!line_buf) {
        return NULL;
      }
      patterns = add_to_string(patterns, line_buf);
      line_size = getline(&line_buf, &line_buf_size, patfile);
    }
    free(line_buf);
    line_buf = NULL;
    fclose(patfile);
  } else if (!flag.s) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
  }
  return patterns;
}

/*parsing arguments of command line*/
int get_flags(const char *short_options, int argc, char **argv, dflag *flag,
              char **patterns) {
  int res;
  while ((res = getopt_long(argc, argv, short_options, 0, 0)) != -1) {
    switch (res) {
      case 'e': {
        flag->e++;
        *patterns = add_to_string(*patterns, argv[optind]);
        *(argv + optind) = NULL; /*except pattern from arguments of command line*/
        optind++;
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
        *patterns = load_patterns(*patterns, argv[optind], *flag);
        *(argv + optind) = NULL; /*except pattern from arguments of command line*/
        optind++;
        break;
      }
      case 'o': {
        flag->o = 1;
        break;
      }

      case '?':
      default: {
        printf("s21_grep: invalid option -- '%c'", res);
        break;
      }
    }
  }
  return optind;
}

/*compile regular expression*/
static int compile(regex_t *regex, const dflag flag, const char *patterns) {
  int cflags = REG_EXTENDED;
  if (flag.i) {
    cflags = REG_EXTENDED | REG_ICASE;
  }
  return regcomp(regex, patterns, cflags);
}

/*output matched lines*/
static void output(regex_t *regex, int *result, char *line_buf, const char *filename,
            const int multifile, const dflag flag, const int line, regmatch_t match) {
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
      char *remain_line = line_buf + match.rm_eo; /*stand in end of first match*/
      *result = regexec(regex, remain_line, 1, &match, 0); /*exec with remain string*/
      while (!(*result)) { /*while there are matches print they*/
        output(regex, result, remain_line, filename, multifile, flag, line, match);
      }
    } else {
      printf("%s", line_buf);
    }
    if (!strstr(line_buf, "\n")) {
      printf("\n");
    }
  }
}

/*special output for -c option*/
static void output_c(int line_counter, int line, dflag flag, char *filename, int multifile) {
  if (flag.v) {
    line_counter = line - line_counter - 1;
  }
  if (multifile && !flag.h) {
    printf("%s:%d\n", filename, line_counter);
  } else {
    printf("%d\n", line_counter);
  }
}

/*processing each file line by line*/
static void flags_controller(FILE *src, dflag flag, regex_t *regex, int *result,
                      char *filename, int multifile) {
  int line_counter = 0, output_suppress = 0, file_match = 0, line = 1;
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  ssize_t line_size = getline(&line_buf, &line_buf_size, src);
  regmatch_t match;
  *result = regexec(regex, line_buf, 1, &match, 0);
  while (line_size >= 0) {
    if (flag.c) {
      if (!(*result)) {
        line_counter++;
      }
      output_suppress = 1;
    }
    if (flag.l) {
      if (!(*result)) {
        file_match = 1;
      }
      output_suppress = 1;
    }
    if (flag.o && flag.v) {
      output_suppress = 1;
    }
    if (!output_suppress) {
      output(regex, result, line_buf, filename, multifile, flag, line, match);
    }
    line_size = getline(&line_buf, &line_buf_size, src);
    *result = regexec(regex, line_buf, 1, &match, 0);
    line++;
  }
  if (flag.c && !flag.l) {
    output_c(line_counter, line, flag, filename, multifile);
  }
  if (flag.l && file_match) {
    printf("%s\n", filename);
    file_match = 0;
  }
  free(line_buf);
  line_buf = NULL;
}

/*processing arguments of command line as files*/
void files_controller(int optind, const int argc, char **argv, const dflag flag,
                      char *patterns) {
  regex_t regex;
  int result, multifile = 0;
  if (!patterns) { /*if there wasn't -e option, then first argument is pattern*/
    patterns = add_to_string(patterns, argv[optind++]);
  }
  result = compile(&regex, flag, patterns);
  if (result) {
    fprintf(stderr, "s21_grep: syntax error in the regular expression %s\n", patterns);
    return;
  }
  if (argc - optind > 1) { /*check quantity of files*/
    multifile = 1;
  }
  while (optind < argc) {
    FILE *src;
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
