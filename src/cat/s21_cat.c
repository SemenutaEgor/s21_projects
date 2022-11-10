#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_flags(const char* short_options, const struct option long_options[], int argc, char **argv, int* flag_b, int* flag_e, int* flag_n, int* flag_s, int* flag_t);
void file_printer(int optind, int argc, char** argv, int flag_b, int flag_e, int flag_n, int flag_s, int flag_t);
void simple_output(char *filename);

int main(int argc, char **argv) {

    int flag_b, flag_e, flag_n, flag_s, flag_t;
    
    const char* short_options = "benst";

    const struct option long_options[] = {
        {"number-nonblank", no_argument, NULL, 'b'},
        {"number", no_argument, NULL, 'n'},
        {"squeeze-blank", no_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };

    int optind = get_flags(short_options, long_options, argc, argv, &flag_b, &flag_e, &flag_n, &flag_s, &flag_t);
    file_printer(optind, argc, argv, flag_b, flag_e, flag_n, flag_s, flag_t);
        
  return 0;
}

int get_flags(const char* short_options, const struct option long_options[], int argc, char **argv, int* flag_b, int* flag_e, int* flag_n, int* flag_s, int* flag_t) {
    
    int option_index;
    int res;

    while ((res = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (res) {
            case 'b': {
                printf("It was flag --number-nonblank\n");
                *flag_b = 1;
                break;
            }
            case 'n': {
                printf("It was flag --number\n");
                *flag_n = 1;
                break;
            }
            case 's': {
                printf("It was flag squeeze-blank\n");
                *flag_s = 1;
                break;
            }
            case 'e': {
                printf("It was flag e\n");
                *flag_e = 1;
                break;
            }
            case 't': {
                printf("It was flag t\n");
                *flag_t = 1;
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

void file_printer(int optind, int argc, char** argv, int flag_b, int flag_e, int flag_n, int flag_s, int flag_t) {
    while (optind < argc) {
        if (flag_b + flag_e + flag_n + flag_s + flag_t == 0) {
        printf("no flags\n");
        simple_output(argv[optind]);
        } else {
            // if there are flags
        }
        optind++;
    }
}


void simple_output(char *filename) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size; //ssize_t able to represent -1 for errors
    FILE *fp = fopen(filename, "r");
    if (fp) {
        line_size = getline(&line_buf, &line_buf_size, fp);
        while (line_size >= 0) {
            line_count++;
            printf("line[%06d]: chars=%6zd, buf size=%06zu, contents: %s", line_count, line_size, line_buf_size, line_buf);
            line_size = getline(&line_buf, &line_buf_size, fp);
        }
        free(line_buf);
        line_buf = NULL;
        fclose(fp);
    } else {
        fprintf(stderr, "Error opening file '%s'\n", filename);
    }
}
