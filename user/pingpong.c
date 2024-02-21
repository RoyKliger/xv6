#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{   
    int p1[2];
    int p2[2];
    int pid = fork();
    char *write_buf = "p"; // can be any byte
    int status;
    char *read_buf[2];

    if(argc > 1){
        fprintf(2, "no arguments please\n");
        exit(1);
    }

    pipe(p1);
    pipe(p2);
    if (pid > 0){
        close(p1[0]);
        write(p1[1], write_buf, 1);
        close(p1[1]);
        wait(&status);
        close(p2[1]);
        read(p2[0], read_buf, 1);
        close(p2[0]);
        printf("%d: received pong\n", getpid());
        exit(0);
    }
    else{
        close(p1[1]);
        read(p1[0], read_buf, 1);
        close(p1[0]);
        printf("%d: received ping\n", getpid());
        close(p2[1]);
        write(p2[0], read_buf, 1);
        close(p2[0]);
        exit(0);

    }
}