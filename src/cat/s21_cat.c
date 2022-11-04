#include <stdio.h>
#include <string.h>

#define MAX_FILE_NAME

struct file_name {
    name = char[255];
    struct file_name *next;
};

bool is_file(char *argv);
void output(char **files);

int main(int argc, char **argv) {
    char **files = NULL; 
    int num_files = 0;
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
        if (is_file(argv[i])) {
            files = realloc(files,  * sizeof(char*));

        } else {
        }
        for (size_t j = 0; j < strlen(argv[i]); j++) {
            printf("%c\n", argv[i][j]);
        }
    }
  return 0;
}
