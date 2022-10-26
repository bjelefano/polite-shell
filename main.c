#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char * read_input(void) {
    // https://www.dmulholl.com/lets-build/a-command-line-shell.html
    char *cmd;
    size_t cmd_len = 0;
    getline(&cmd, &cmd_len, stdin);

    int length = 0;
    int capacity = 4;
    char **tokens = malloc(capacity * sizeof(char*));

    char *delimiters = " \t\r\n";
    char *token = strtok(cmd, delimiters);

    while (token != NULL) {
        tokens[length] = token;
        length++;

        token = strtok(NULL, delimiters);
    }

    tokens[length] = NULL;

    char * res;

    if (strcmp(tokens[0], "please") != 0 && strcmp(tokens[length - 1], "please")  != 0) {
        res = malloc(15);
        strcpy(res, "You are rude :(");
    } else {
        res = malloc(9);
        strcpy(res, "Hello! :)");
    }
    free(tokens);

    return res;
}

int main(int argc, char **argv) {
    char * res;

    fprintf(stderr, "(o _ o) <(");

    do {

        res = read_input();

        if(!res) {
            exit(1);
        }

        if(res[0] == '\0' || strcmp(res, "\n") == 0) {
            free(res);
            continue;
        }

        if(strcmp(res, "goodbye\n") == 0 || strcmp(res, "bye-bye\n") == 0 || strcmp(res, "see ya\n") == 0) {
            free(res);
            break;
        }

        printf("%s\n", res);

        free(res);

    } while(1);

    exit(0);
}