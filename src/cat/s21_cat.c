#include <stdio.h>
#include <string.h>
#define _GNU_SOURCE
#include <getopt.h>

//void output(char *filename);

int main(int argc, char **argv) {
    
    const char* short_options = "benst";

    const struct option long_options[] = {
        {"number-nonblank", no_argument, NULL, 'b'},
        {"number", no_argument, NULL, 'n'},
        {"squeeze-blank", no_argument, NULL, 's'}
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
                printf("squeeze-blank\n");
                break;
            }
            case '?': default: {
                printf("found unknown option");
                break;
            }
        }
    }

  return 0;
}

/*void output(char *filename) {
    FILE *fp;
    if (fp = fopen(filename, "r")) {
        while (!eof) {
            //processing
        }
    } else {
        //put in stderr an error with wrong filename
    }
}*/
