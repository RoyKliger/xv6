#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{   
    int numbers[35];
    int *p;
    int pid;
    //int ppid = getpid();
    int status;
    int i;
    int all_zero = 0;
    int prime = 0;

    if(argc > 1){
        fprintf(2, "no arguments please\n");
        exit(1);
    }

    for (i = 0 ; i < 35 ; i++){
        numbers[i] = i;
    }
    
    while (1){
        p = malloc(2 * sizeof(int));
        if (pipe(p) == -1){
            fprintf(2, "pipe creation failed\n");
            exit(1);
        }
        pid = fork();
        if (pid > 0){
            for (i = 2 ; i < 35 ; i++){
                if (numbers[i] != 0){
                    prime = i;
                    break;
                }
            }
            all_zero = i == 35;
            if (all_zero){
                free(p);
                exit(0);
            }
            printf("prime %d\n", prime);
            for (i = prime ; i < 35 ; i += prime){
                numbers[i] = 0;
                //printf("%d ", numbers[i]);
            }
            close(p[0]);
            write(p[1], numbers, 35 * sizeof(int));
            close(p[1]);
            wait(&status);
            exit(0);
        }
        else{

            close(p[1]);
            read(p[0], numbers , 35 * sizeof(int));
            close(p[0]);
            
        }
        free(p);
    }
}