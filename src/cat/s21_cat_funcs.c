#include "s21_cat_funcs.h"

int get_flags(const char* short_options, const struct option long_options[], int argc, char **argv, dflag* flag) {
    
    int option_index;
    int res;

    while ((res = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
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
                break;
            }
            case 'E': {
                flag->E = 1;
                break;
            }
            case 't': {
                flag->t = 1;
                break;
            }
            case 'T': {
                flag->T = 1;
                break;
            }

            case '?': default: {
                printf("found unknown option");
                break;
            }
        }
    }
    return optind;
}

int no_flags(dflag flag) {
    int res = 0;
    if (flag.b + flag.e + flag.E + flag.n + flag.s + flag.t + flag.T == 0) {
        res = 1;
    }
    return res;
}

void files_controller(int optind, int argc, char** argv, dflag flag) { 

    FILE *src;
    int prev_empty = 0, all_count = 1, non_empty_count = 1;
    
    while (optind < argc) {
        src = fopen(argv[optind], "r");
        if (src) {
            flags_controller(src, flag, &prev_empty, &all_count, &non_empty_count);
            fclose(src);
        } else {
            fprintf(stderr, "Error opening file '%s'\n", argv[optind]);
        }
        optind++;
    }
}

void restart_prev_empty(int curr_empty, int *prev_empty) {
    if (curr_empty) {
        *prev_empty = 1;
    } else {
        *prev_empty = 0;
    }
}

void flags_controller(FILE *src, dflag flag, int *prev_empty, int *all_count, int *non_empty_count) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size = getline(&line_buf, &line_buf_size, src);
    dbuf buffer = {line_buf, 0, 0, line_size}; 
    while (buffer.size >= 0) {
        if (flag.t) {
            flag_v(&buffer);
            flag_t(&buffer);
        }
        if (flag.T) {
            flag_t(&buffer);
        }
        if (flag.s) {
            flag_s(&buffer);
        }
        if (flag.n) {
            flag_n(&buffer, all_count, flag.s, *prev_empty);
        }
        if (flag.b) {
            flag_b(&buffer, non_empty_count);
        }
        if (flag.e) {
            flag_v(&buffer);
            flag_e(&buffer);
        }
        if (flag.E) {
            flag_e(&buffer);
        }
        output(buffer, flag.s, *prev_empty);
        restart_prev_empty(buffer.empty, prev_empty);
        buffer.size = getline(&line_buf, &line_buf_size, src);
        buffer.data = line_buf;
        buffer.empty = 0;
        buffer.number = 0;
    }
    free(line_buf);
    line_buf = NULL;
}

void flag_n(dbuf *buffer, int  *all_count, int squeeze, int prev_empty) {
    if (!(squeeze && buffer->empty && prev_empty)) {
        buffer->number = (*all_count)++;
    }
}

void flag_b(dbuf *buffer, int  *non_empty_count) {
    if (buffer->size != 1) {
        buffer->number = (*non_empty_count)++;
    } else {
        buffer->number = 0;
    }
}

void flag_s(dbuf *buffer) {
    if (buffer->size == 1) {
        buffer->empty = 1;
    }
}

void flag_v(dbuf *buffer) {
    char *new_data = malloc(sizeof(char) * buffer->size * 2);
    ssize_t i = 0;
    int j = 0, code = 0;
    while(i < buffer->size) {
        code = buffer->data[i++];
        if (code <= 31 && code != 9 && code != 10 && code != 13) {
            new_data[j++] = '^';
            new_data[j++] = code + 64;
        } else if (code == 127) {
            new_data[j++] = '^';
            new_data[j++] = 63;
        } else {
            new_data[j++] = code;
        }
    }
    buffer->size = j;
    new_data = realloc(new_data, sizeof(char) * buffer->size);
    buffer->data = new_data;
}

void flag_e(dbuf* buffer) {
    char *new_data = malloc(sizeof(char) * (buffer->size + 2));
    ssize_t i = 0;
    int j = 0, code = 0;
    while(i < buffer->size) {
        code = buffer->data[i++];
        if (code == 10) {
            new_data[j++] = '$';
            new_data[j++] = '\n';
        } else {
            new_data[j++] = code;
        }
    }
    buffer->size = j;
    new_data = realloc(new_data, sizeof(char) * buffer->size);
    buffer->data = new_data;
}

void flag_t(dbuf* buffer) {
    char *new_data = malloc(sizeof(char) * (buffer->size + 2));
    ssize_t i = 0;
    int j = 0, code = 0;
    while(i < buffer->size) {
        code = buffer->data[i++];
        if (code == 9) {
            new_data[j++] = '^';
            new_data[j++] = 'I';
        } else {
            new_data[j++] = code;
        }
    }
    buffer->size = j;
    new_data = realloc(new_data, sizeof(char) * buffer->size);
    buffer->data = new_data;
}

void output(dbuf buffer, int squeeze, int prev_empty) {
    if (!(squeeze && buffer.empty && prev_empty)) {
        if (buffer.number) {
            printf("%6d\t", buffer.number);
        }
        ssize_t i = 0;
        while(i < buffer.size) {
            putchar(buffer.data[i++]);
        }
    }
}
