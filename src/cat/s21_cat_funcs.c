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
                printf("It was flag squeeze-blank\n");
                flag->s = 1;
                break;
            }
            case 'e': {
                flag->e = 1;
                break;
            }
            case 'E': {
                printf("It was flag e\n");
                flag->E = 1;
                break;
            }
            case 't': {
                printf("It was flag t\n");
                flag->t = 1;
                break;
            }
            case 'T': {
                printf("It was flag T\n");
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
    int prev_empty = 0, line_count = 0;
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size = getline(&line_buf, &line_buf_size, src);
    dbuf buffer = {line_buf, 0, 0, line_size}; 
    while (buffer.size >= 0) {
        if (flag.e) {
            //buffer = flag_e(buffer);
        }
        if (flag.E) {
            //buffer = flag_E(buffer);
        }
        if (flag.t) {
            //buffer = flag_t(buffer);
        }
        if (flag.T) {
            //buffer = flag_T(buffer);
        }
        if (flag.s) {
            //buffer = flag_s(buffer);
        }
        if (flag.n) {
            //buffer = flag_n(buffer);
        }
        if (flag.b) {
            //buffer = flag_b(buffer);
        }
        output(buffer, flag.s, prev_empty);
        if (buffer.empty) {
            prev_empty = 1;
        }
        line_count++;
        buffer.size = getline(&line_buf, &line_buf_size, src);
        buffer.data = line_buf;
        buffer.empty = 0;
        buffer.number = 0;
    }
    free(line_buf);
    line_buf = NULL;
}

/*void flag_n(char *source_file) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;
    FILE *src = fopen(source_file, "r");
    FILE *buf1 = fopen(buffer_1, "w+");
    if (src) {
        line_size = getline(&line_buf, &line_buf_size, src);
        while (line_size >= 0) {
            line_count++;
            fprintf(buf1, "%6d\t", line_count);
            ssize_t i = 0;
            while(i < line_size) {
                putc(line_buf[i++], buf1);
            }
            line_size = getline(&line_buf, &line_buf_size, src);
        }
        free(line_buf);
        line_buf = NULL;
        fclose(src);
        fclose(buf1);
    } else {
        fprintf(stderr, "Error opening file '%s'\n", source_file);
    }
}

void flag_b(char *source_file) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 1;
    ssize_t line_size;
    FILE *src = fopen(source_file, "r");
    FILE *buf1 = fopen(buffer_1, "w+");
    if (src) {
        line_size = getline(&line_buf, &line_buf_size, src);
        while (line_size >= 0) {
            if (line_size > 1) {
                fprintf(buf1, "%6d\t", line_count);
                line_count++;
            }
            ssize_t i = 0;
            while(i < line_size) {
                putc(line_buf[i++], buf1);
            }
            line_size = getline(&line_buf, &line_buf_size, src);
        }
        free(line_buf);
        line_buf = NULL;
        fclose(src);
        fclose(buf1);
    } else {
        fprintf(stderr, "Error opening file '%s'\n", source_file);
    }
}

void flag_v(char *source_file) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;
    FILE *src = fopen(source_file, "r");
    FILE *buf1 = fopen(buffer_1, "w+");
    if (src) {
        line_size = getline(&line_buf, &line_buf_size, src);
        int code = 0;
        while (line_size >= 0) {
            line_count++;
            ssize_t i = 0;
            while(i < line_size) {
                code = line_buf[i];
                if (code <= 31 && code != 9 && code != 10 && code != 13) {
                    putc('^', buf1);
                    fprintf(buf1, "%c", code + 64);
                } else if (code == 127) {
                    putc('^', buf1);
                    fprintf(buf1, "%c", 63);
                } else {
                    putc(line_buf[i], buf1);
                }
                i++;
            }
            line_size = getline(&line_buf, &line_buf_size, src);
        }
        free(line_buf);
        line_buf = NULL;
        fclose(src);
        fclose(buf1);
    } else {
        fprintf(stderr, "Error opening file '%s'\n", source_file);
    }
}

void flag_e(char *source_file) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;
    FILE *src = fopen(source_file, "r");
    FILE *buf_2 = fopen(buffer_2, "w+");
    if (src) {
        line_size = getline(&line_buf, &line_buf_size, src);
        int code = 0;
        while (line_size >= 0) {
            line_count++;
            ssize_t i = 0;
            while(i < line_size) {
                code = line_buf[i];
                if (code == 10) {
                    putc('$', buf_2);
                    putc('\n', buf_2);
                } else {
                    putc(line_buf[i], buf_2);
                }
                i++;
            }
            line_size = getline(&line_buf, &line_buf_size, src);
        }
        free(line_buf);
        line_buf = NULL;
        fclose(src);
        fclose(buf_2);
    } else {
        fprintf(stderr, "Error opening file '%s'\n", source_file);
    }
}
*/
void output(dbuf buffer, int squeeze, int prev_empty) {
    if (!(squeeze && buffer.data && prev_empty)) {
        if (buffer.number) {
            printf("%6d\t", buffer.number);
        }
        ssize_t i = 0;
        while(i < buffer.size) {
            putchar(buffer.data[i++]);
        }
    }
}
