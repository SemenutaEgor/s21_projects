#include "s21_grep_funcs.h"

int get_flags(const char *short_options, int argc, char **argv, dflag *flag) {
  int res;

  while ((res = getopt(argc, argv, short_options)) != -1) {
    switch (res) {
      case 'e': {
        flag->e = 1;
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
  return optind;
}

void files_controller(int optind, int argc, char **argv, dflag flag) {
  FILE *src;

  while (optind < argc) {
    src = fopen(argv[optind], "r");
    if (src) {
      flags_controller(src, flag);
      fclose(src);
    } else {
      fprintf(stderr, "Error opening file '%s'\n", argv[optind]);
    }
    optind++;
  }
}

void flags_controller(FILE *src, dflag flag) {
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  ssize_t line_size = getline(&line_buf, &line_buf_size, src);
  dbuf buffer = {line_buf};
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
	    // flag h
    }
    output();
    line_size = getline(&line_buf, &line_buf_size, src);
    buffer.data = line_buf;
  }
  free(line_buf);
  line_buf = NULL;
}

void output() {
	//output
}
