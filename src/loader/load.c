#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage:\n\t%s <file-to-exec>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *load_argv[]   = { NULL, NULL };
    char *load_env[]    = { "LD_PRELOAD=./hooks.so", NULL };

    load_argv[0] = argv[1];
    execve(argv[1], load_argv, load_env);
    perror("execve");
    exit(EXIT_FAILURE);
}