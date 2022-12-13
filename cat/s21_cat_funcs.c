#include "s21_cat_funcs.h"

/*parsing arguments of command line*/
int get_flags(const char *short_options, const struct option long_options[],
              int argc, char **argv, dflag *flag) {
  int option_index = 0, res = 0;
  while ((res = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (res) {
      case 'b': {
        flag->b = 1;
        break;
      }
      case 'n': {
        flag->n = 1;
        break;
      }
      case 's': {
        flag->s = 1;
        break;
      }
      case 'e': {
        flag->e = 1;
        flag->v = 1;
        break;
      }
      case 'E': {
        flag->E = 1;
        break;
      }
      case 't': {
        flag->t = 1;
        flag->v = 1;
        break;
      }
      case 'T': {
        flag->T = 1;
        break;
      }

      case '?':
      default: {
        printf("s21_cat: invalid option -- '%c'\n", res);
        break;
      }
    }
  }
  return optind;
}

/*set info about previous string is empty of not*/
static void restart_prev_empty(int const curr_empty, int *prev_empty) {
  if (curr_empty) {
    *prev_empty = 1;
  } else {
    *prev_empty = 0;
  }
}

/*print result*/
static void output(dbuf buffer, int const squeeze, int const prev_empty,
                   int *new_line) {
  if (!(squeeze && buffer.empty && prev_empty)) {
    if (buffer.number) {
      printf("%6d\t", buffer.number);
    }
    ssize_t i = 0;
    while (i < buffer.size) {
      putchar(buffer.data[i++]);
    }
    if (buffer.data[i - 1] == '\n') {
      *new_line = 1;
    } else {
      *new_line = 0;
    }
  }
}

/*numerate lines if they are not to be missed*/
static void flag_n(dbuf *buffer, int *all_count, const int squeeze,
                   const int prev_empty, const int new_line) {
  if (!(squeeze && buffer->empty && prev_empty) && new_line) {
    buffer->number = (*all_count)++; /*global counter also changed*/
  }
}

/*numerate all non-empty lines*/
static void flag_b(dbuf *buffer, int *non_empty_count, const int new_line) {
  if ((buffer->size != 1) && new_line) {
    buffer->number =
        (*non_empty_count)++; /*global counter of empty lines also changed*/
  } else {
    buffer->number = 0;
  }
}

/*marks the string as empty*/
static void flag_s(dbuf *buffer) {
  if (buffer->size == 1) {
    buffer->empty = 1;
  }
}

/*make nonprinting symbols visible*/
static void flag_v(dbuf *buffer, int *alloc) {
  char *new_data = (char *)malloc(sizeof(char) * buffer->size *
                                  2); /*allocate x2 memory for each symbol*/
  if (new_data) {
    ssize_t i = 0;
    int j = 0;
    while (i < buffer->size) {
      int code = 0;
      code = buffer->data[i++];
      if (code <= UNIT_SEP && code != HOR_TAB && code != LINE_FEED &&
          code != CAR_RET) {
        new_data[j++] = '^';
        new_data[j++] = code + ALP_START;
      } else if (code == DEL) {
        new_data[j++] = '^';
        new_data[j++] = ALP_START - 1;
      } else {
        new_data[j++] = code;
      }
    }
    buffer->size = j;
    if (*alloc) {
      free(buffer->data); /*because new_data has new address*/
    }
    buffer->data = new_data;
    *alloc = 1;
  } else {
    fprintf(stderr, "s21_cat: error with memory allocating\n");
  }
}

/*changes line feeds to $*/
static void flag_e(dbuf *buffer, int *alloc) {
  char *new_data = (char *)malloc(sizeof(char) * (buffer->size + 2));
  if (new_data) {
    ssize_t i = 0;
    int j = 0;
    while (i < buffer->size) {
      int code = 0;
      code = buffer->data[i++];
      if (code == LINE_FEED) {
        new_data[j++] = '$';
        new_data[j++] = '\n';
      } else {
        new_data[j++] = code;
      }
    }
    buffer->size = j;
    if (*alloc) {
      free(buffer->data); /*because new_data has new address*/
    }
    buffer->data = new_data;
    *alloc = 1;
  } else {
    fprintf(stderr, "s21_cat: error with memory allocating\n");
  }
}

/*changes /t to ^I*/
static void flag_t(dbuf *buffer, int *alloc) {
  char *new_data = (char *)malloc(sizeof(char) * buffer->size * 2);
  if (new_data) {
    ssize_t i = 0;
    int j = 0;
    while (i < buffer->size) {
      int code = 0;
      code = buffer->data[i++];
      if (code == 9) {
        new_data[j++] = '^';
        new_data[j++] = 'I';
      } else {
        new_data[j++] = code;
      }
    }
    buffer->size = j;
    if (*alloc) {
      free(buffer->data); /*because new_data has new address*/
    }
    buffer->data = new_data;
    *alloc = 1;
  } else {
    fprintf(stderr, "s21_cat: error with allocating memory for patterns\n");
  }
}

/*processing each file line by line*/
static void flags_controller(FILE *src, dflag flag, int *prev_empty,
                             int *all_count, int *non_empty_count,
                             int *new_line) {
  char *line_buf = NULL; /*for store string*/
  int alloc = 0;
  size_t line_buf_size = 0;
  ssize_t line_size = getline(&line_buf, &line_buf_size, src);
  dbuf buffer = {line_buf, 0, 0, line_size};
  while (buffer.size >= 0) {
    if (flag.t) {
      flag_v(&buffer, &alloc);
      flag_t(&buffer, &alloc);
    }
    if (flag.T) {
      flag_t(&buffer, &alloc);
    }
    if (flag.s) {
      flag_s(&buffer);
    }
    if (flag.n) {
      flag_n(&buffer, all_count, flag.s, *prev_empty, *new_line);
    }
    if (flag.b) {
      flag_b(&buffer, non_empty_count, *new_line);
    }
    if (flag.e) {
      flag_v(&buffer, &alloc);
      flag_e(&buffer, &alloc);
    }
    if (flag.E) {
      flag_e(&buffer, &alloc);
    }
    output(buffer, flag.s, *prev_empty, new_line);
    if (alloc) { /*all flags that allocate memorry, except -v*/
      free(buffer.data);
      alloc = 0;
    }
    restart_prev_empty(buffer.empty, prev_empty);
    buffer.size = getline(&line_buf, &line_buf_size, src);
    buffer.data = line_buf;
    buffer.empty = 0;
    buffer.number = 0;
  }
  free(line_buf);
  line_buf = NULL;
}

/*processing arguments of command line as files*/
void files_controller(int optind, const int argc, char **argv, dflag flag) {
  int prev_empty = 0, all_count = 1, non_empty_count = 1, new_line = 1;

  while (optind < argc) {
    FILE *src;
    src = fopen(argv[optind], "r");
    if (src) {
      flags_controller(src, flag, &prev_empty, &all_count, &non_empty_count,
                       &new_line);
      fclose(src);
    } else {
      fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[optind]);
    }
    optind++;
  }
}
