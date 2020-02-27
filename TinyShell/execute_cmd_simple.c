#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BLANK_STRING " "

int make_argv(const char *s, const char *delimiters, char ***argvp);

void execute_cmd(char *incmd) {
    char **chargv;
    if (make_argv(incmd, BLANK_STRING, &chargv) <= 0) {
       fprintf(stderr, "Failed to parse command line\n");
       exit(1);
    }
    execvp(chargv[0], chargv);
    perror("Failed to execute command");
    exit(1);
}
