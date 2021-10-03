#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void 
read_funct(int from_parent[])
{
    int received;
    int stat = 1;
    int first = 0;
    close(from_parent[1]);

    stat = read(from_parent[0], &received, sizeof(int));
    if (stat == 0)
        exit(-1);
    if (first == 0)
    {
        printf("prime %d\n", received);
        first = 1;
    }

    int to_child[2];
    int no_pipe = 0;
    int num;
    int pid;
    while (read(from_parent[0], &num, sizeof(int)))
    {
        if (num % received)
        {
            if (no_pipe == 0)
            {
                pipe(to_child);
                pid = fork();
                if (pid < 0)
                {
                    fprintf(2, " usage: fork error\n");
                    exit(-1);
                }
                no_pipe=1;
            }

            if (pid == 0)
            {
            	close(from_parent[0]);
                wait(0);
                read_funct(to_child);
                exit(0);
            }
            else
            {
                close(to_child[0]);
                write(to_child[1], &num, sizeof(int));
            }
        }
    }
    close(to_child[1]);
    exit(0);
}

// write prime numbers to standart input from 2 to 35
int 
main(int argc, char *argv[])
{
    int to_child[2];
    if (pipe(to_child) != 0)
    {
        fprintf(2, "primes usage: pipe error\n");
        exit(-1);
    }

    int pid = fork();
    if (pid < 0)
    {
        fprintf(2, "primes usage: fork error\n");
        exit(-1);
    }

    if (pid == 0)
    {
        //child
        wait(0);
        read_funct(to_child);
    }
    else
    {
        //  parent
        close(to_child[0]);
        for (int i = 2; i < 36; i++)
            write(to_child[1], &i, sizeof(int));
            
        //wait(0);          // uncomment for running without '$' 
        exit(0);
    }
    return 0;
}
