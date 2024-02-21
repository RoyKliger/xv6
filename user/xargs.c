#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"
#include "user.h"

int get_args_from_stdin(char** cmd) {
    int index = 0;
    int starting_index = 0;
    int argnum = 0;
    char buf[1024];
    while(read(0, buf + index, 1) == 1) {
        if (buf[index] == '\n') {
            buf[index] = '\0';
            printf("");
            // printf("strlen of buf: %d\n", strlen(buf + starting_index));
            // printf("last buf: %s with argnum %d\n", buf + starting_index, argnum);
            cmd[argnum] = buf + starting_index;
            return ++argnum;
        }
        if (buf[index] == ' ') {
            buf[index] = '\0';
            // printf("buf: %s with argnum %d\n", buf + starting_index, argnum);
            cmd[argnum] = buf + starting_index;
            ++argnum;
            starting_index = index + 1;
        }
        index++;
    }
    return 0;
    
    
}


int main(int argc, char *argv[]) {
    char *cmd[MAXARG];
    int status;
    int argnum;
    int i;

    if (argc < 2) {
        fprintf(2, "Usage: xargs <command>\n");
        exit(1);
    }

    // Parse command arguments
    for (i = 1; i < argc; i++) {
        cmd[i-1] = argv[i];
    }

    while ((argnum = get_args_from_stdin(cmd + argc - 1)) > 0) {
        // Execute command
        if (fork() == 0) {
            // for (i = 0; i < argc + argnum - 1; i++) {
            //     printf("cmd[%d]: %s\n", i, cmd[i]);
            // }
            exec(cmd[0], cmd);
            fprintf(2, "xargs: %s failed to execute\n", cmd[0]);
            exit(1);
        } else {
            wait(&status);
        }
    }
    exit(0);
}
