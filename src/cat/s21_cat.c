#include <stdio.h>
#include <string.h>
#include <getopt.h>

void output(char *filename);

int main(int argc, char **argv) {
    
    const char* short_options = //add short options

    const struct option long_options[] = {
        //add long options
    };

    int res;
    int option_index;

    while ((res = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (res) {
            //cases of options
        }
    }

  return 0;
}

void output(char *filename) {
    FILE *fp;
    if (fp = fopen(filename, "r")) {
        while (!eof) {
            //processing
        }
    } else {
        //put in stderr an error with wrong filename
    }
}
