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

int get_flags(const char *short_options, int argc, char **argv, dflag *flag) {
  int res;
  while ((res = getopt_long(argc, argv, short_options, 0, 0)) != -1) {
    switch (res) {
      case 'e': {
        flag->e = 1;
        printf("flag e\n");
        // check for the next pattern
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
