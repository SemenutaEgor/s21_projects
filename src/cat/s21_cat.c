#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void simple_output(char *filename);

int main(int argc, char **argv) {
    
    const char* short_options = "benst";

    const struct option long_options[] = {
        {"number-nonblank", no_argument, NULL, 'b'},
        {"number", no_argument, NULL, 'n'},
        {"squeeze-blank", no_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };

    int res;
    int option_index;

    while ((res = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (res) {
            case 'b': {
                printf("It was flag --number-nonblank\n");
                break;
            }
            case 'n': {
                printf("It was flag --number\n");
                break;
            }
            case 's': {
                printf("It was flag squeeze-blank\n");
                break;
            }
            case 'e': {
                printf("It was flag e\n");
                break;
            }
            case '?': default: {
                printf("found unknown option");
                break;
            }
        }
    }
    while (optind < argc) {
        printf("found sth not flag: %s\n", argv[optind]);
        simple_output(argv[optind]);
        optind++;
    }

  return 0;
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
