#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_WORDS 3

// J.F. Sebastian
void make_lower(char * inp_str) {
    for ( ; *inp_str; ++inp_str) *inp_str = tolower( (unsigned char)*inp_str );
}

// https://www.dmulholl.com/lets-build/a-command-line-shell.html
void execute_cmd(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) != 0) {
            perror("shy_shell - teleport_to");
        }
    } else {
        pid_t child_pid = fork();

        if (child_pid == 0) {
            execvp(args[0], args);
            perror("shy_shell - list_items_from");
            exit(1);
        } else if (child_pid > 0) {
            int status;
            do {
                waitpid(child_pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        } else {
            perror("shy_shell: list_items_from");
        }
    }
}


void process_cmd(char ** tokens, char ** res) {

    // Checking the cmd for curse words
    for (int i = 0; i < MAX_WORDS; i++) {
        if (tokens[i] == NULL) {
            break;
        } 
        make_lower(tokens[i]);

        // please don't swear, he is doing his best
        if (strcmp(tokens[i], "fuck") == 0 || strcmp(tokens[i], "shit") == 0 || 
            strcmp(tokens[i], "bitch") == 0 || strcmp(tokens[i], "cunt") == 0 || 
            strcmp(tokens[i], "bastard") == 0 || strcmp(tokens[i], "whore") == 0) {

            *res = malloc(28 * sizeof(char));
            strcpy(*res, "Please don't swear at me :(");
            return;
        }
    }

    if (strcmp(tokens[0], "hello") == 0) {
        *res = malloc(9 * sizeof(char));
        strcpy(*res, "Hello :)");
    } else if (strcmp(tokens[0], "goodbye") == 0) {
        *res = malloc(11 * sizeof(char));
        strcpy(*res, "Bye-bye! :)");
    } else if (strcmp(tokens[0], "please") == 0) {
        char * true_cmd;
        int flag = 0;
        int is_not_here = 1;

        if (strcmp(tokens[1], "list_items_from") == 0) {
            true_cmd = malloc(3 * sizeof(char));
            strcpy(true_cmd, "ls");

            if (strcmp(tokens[2], "here") == 0) {
                is_not_here--;
            } 

            flag = 1;
        } else if (strcmp(tokens[1], "teleport_to") == 0) {
            true_cmd = malloc(3 * sizeof(char));
            strcpy(true_cmd, "cd");

            if (strcmp(tokens[2], "here") == 0) {
                is_not_here--;
            } 

            flag = 1;
        }

        char ** args = malloc((is_not_here + 1) * sizeof(char *));


        if (strcmp(true_cmd, "cd") == 0 && !is_not_here) {
            *res = malloc(30 * sizeof(char));
            strcpy(*res, "You are already here silly! :)");
        } else if (flag) {
            *res = malloc(18 * sizeof(char));
            strcpy(*res, "Why certainly! :)");

            args[0] = true_cmd;

            if (is_not_here) {
                args[1] = tokens[2];
            }

            execute_cmd(args);
        } else {
            *res = malloc(28 * sizeof(char));
            strcpy(*res, "Sorry, I don't understand :(");
        }
    } else if (strcmp(tokens[1], "please") == 0) {
        *res = malloc(28 * sizeof(char));
        strcpy(*res, "Sorry, I don't understand :(");
    } else if (strcmp(tokens[2], "please") == 0) {
        char * true_cmd;
        int is_not_here = 1;
        int flag = 0;

        if (strcmp(tokens[0], "list_items_from") == 0) {
            true_cmd = malloc(3 * sizeof(char));
            strcpy(true_cmd, "ls");

            if (strcmp(tokens[1], "here") == 0) {
                is_not_here--;
            } 

            flag = 1;
        } else if (strcmp(tokens[0], "teleport_to") == 0) {
            true_cmd = malloc(3 * sizeof(char));
            strcpy(true_cmd, "cd");

            if (strcmp(tokens[1], "here") == 0) {
                is_not_here--;
            } 

            flag = 1;
        }

        char ** args = malloc((is_not_here + 1) * sizeof(char *));

        if (strcmp(true_cmd, "cd") == 0 && !is_not_here) {
            *res = malloc(30 * sizeof(char));
            strcpy(*res, "You are already here silly! :)");
        } else if (flag) {
            *res = malloc(18 * sizeof(char));
            strcpy(*res, "But of course! :)");

            args[0] = true_cmd;

            if (is_not_here) {
                args[1] = tokens[1];
            }

            execute_cmd(args);
        } else {
            *res = malloc(28 * sizeof(char));
            strcpy(*res, "Sorry, I don't understand :(");
        }
    } else {
        *res = malloc(16 * sizeof(char));
        strcpy(*res, "You are rude :(");
    }

    free(tokens);
    return;
}

char * read_input(void) {
    // https://www.dmulholl.com/lets-build/a-command-line-shell.html
    char *cmd;
    size_t cmd_len = 0;
    getline(&cmd, &cmd_len, stdin);

    char * res;

    int num_tokens = 0;
    char **tokens = malloc((MAX_WORDS + 1) * sizeof(char*));

    char *delimiters = " \t\r\n";
    char *token = strtok(cmd, delimiters);

    while (token != NULL) {
        tokens[num_tokens] = token;
        num_tokens++;

        if (num_tokens > MAX_WORDS) {
            res = malloc(43 * sizeof(char));
            strcpy(res, "Too much information. Can't understand. :(");

            free(tokens);
            return res;
        }

        token = strtok(NULL, delimiters);
    }

    tokens[num_tokens] = NULL;

    if (strcmp(tokens[0], "goodbye") == 0 || num_tokens == 3) {
        process_cmd(tokens, &res);
    } else {
        res = malloc(29 * sizeof(char));
        strcpy(res, "Sorry, I don't understand. :(");
    }
    
    return res;
}

int main(int argc, char **argv) {
    char * res;
    do {
        fprintf(stderr, "(o _ o) <(");
        res = read_input();

        if(!res) {
            exit(1);
        }

        if(res[0] == '\0' || strcmp(res, "\n") == 0) {
            free(res);
            continue;
        }

        printf("%s\n", res);

        if(strcmp(res, "Bye-bye! :)") == 0) {
            free(res);
            break;
        }
        free(res);

    } while(1);

    exit(0);
}