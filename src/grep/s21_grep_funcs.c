#include "s21_grep_funcs.h"

void print_result(int value) {
	if (value == 0) {
		printf("Pattern found\n");
	} else if (value == REG_NOMATCH) {
		printf("Pattern not found\n");
	} else {
		printf("An error occured\n");
	}
}

char* add_pattern(char *patterns, char *pattern) {
  size_t new_length = strlen(patterns) + strlen(pattern) + 2; // \0 + |
  char* new_patterns = realloc(patterns, sizeof(char) * new_length);
  if (new_patterns) {
    patterns = new_patterns;
    strcat(patterns, "|");
    strcat(patterns, pattern);
  } else {
    fprintf(stderr, "Error with allocating memory for patterns");
    exit(0);
  }
  return new_patterns;
}
  

int get_flags(const char *short_options, int argc, char **argv, dflag *flag, char* patterns) {
  int res;
  while ((res = getopt_long(argc, argv, short_options, 0, 0)) != -1) {
    switch (res) {
      case 'e': {
        flag->e++;
        //printf("flag e\n");
        //printf("next %s\n", argv[optind]); // take the next argunent after -e
        patterns = add_pattern(patterns, argv[optind]);
        break;
      }
      case 'i': {
        flag->i = 1;
        printf("flag i\n");
        break;
      }
      case 'v': {
        flag->v = 1;
        printf("flag v\n");
        break;
      }
      case 'c': {
        flag->c = 1;
        printf("flag c\n");
        break;
      }
      case 'l': {
        flag->l = 1;
        printf("flag l\n");
        break;
      }
      case 'n': {
        flag->n = 1;
        printf("flag n\n");
        break;
      }
      case 'h': {
        flag->h = 1;
        printf("flag h\n");
        break;
      }
      case 's': {
        flag->s = 1;
        printf("flag s\n");
        break;
      }
      case 'f': {
        flag->f = 1;
        printf("flag f\n");
        break;
      }
      case 'o': {
        flag->o = 1;
        printf("flag o\n");
        break;
      }

      case '?':
      default: {
        printf("found unknown option");
        break;
      }
    }
  }
  for (size_t i = 0; i < strlen(patterns); i++) {
    putchar(patterns[i]);
  }
  return optind;
}

void files_controller(int optind, int argc, char **argv, dflag flag) {
  FILE *src;
  regex_t reegex;
  int value;

  while (optind < argc) {
    //printf("try to open %s\n", argv[optind]);
    //printf("regcomp value %d\n", value);
    src = fopen(argv[optind], "r");
    if (src) {
      flags_controller(src, flag, &reegex, &value);
      fclose(src);
    } else {
      //fprintf(stderr, "Error opening file '%s'\n", argv[optind]);
      value = regcomp(&reegex, argv[optind], 0);
      //printf("next %s\n", argv[optind + 1]);
      //optind++;
    }
    optind++;
  }
}

void flags_controller(FILE *src, dflag flag, regex_t *reegex, int *value) {
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  ssize_t line_size = getline(&line_buf, &line_buf_size, src);
  dbuf buffer = {line_buf, line_size};
  *value = regexec(reegex, buffer.data, 0, NULL, 0);
  //printf("regexec value %d\n", *value);
  while (line_size >= 0) {
    if (flag.e) {
	    // flag e
    }
    if (flag.i) {
	    // flag i
    }
    if (flag.v) {
	    // flag v
    }
    if (flag.c) {
	    // flag c
    }
    if (flag.l) {
	    // flag l
    }
    if (flag.n) {
	    // flag n
    }
    if (flag.h) {
	    //flag h
    }
    if (*value == 0) {
      //printf("got pattern\n");
      output(buffer);
    }
    line_size = getline(&line_buf, &line_buf_size, src);
    *value = regexec(reegex, buffer.data, 0, NULL, 0);
    //printf("regexec value %d\n", *value);
    buffer.data = line_buf;
  }
  free(line_buf);
  line_buf = NULL;
}

void output(dbuf buffer) {
    ssize_t i = 0;
    while (i < buffer.size) {
      putchar(buffer.data[i++]);
    }
}
