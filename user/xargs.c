#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXDIS 1024

// control if input is not longer then MAXDIS,
// if longer write the problem to output and exit the program
void
control_distance(int distance){
    if(distance > MAXDIS){
        printf("xargs usage: too long input");
        exit(-1);
    }
}


// read lines from standard input, then executes the commands
// use the given line as an argument to the command
int
main(int argc, char *argv[])
{
    char buf[MAXDIS];
    char *pointer = buf;
    int distance = 0;
    char *arguments[MAXARG];        // MAXARG = max exec arguments

    if (argc < 2){
        fprintf(2, "xargs usage: too few arguments\n");
        exit(-1);
    }

    int i;
    for (i = 1; i < argc; i++){
        arguments[i - 1] = argv[i];
    }
    i -= 1;

    int read_end = 1;
    char ch;
    while (read_end){
        read_end = read(0, &ch, 1);

        if (read_end == 0){
            break;
        }

        if (ch == ' '){
            control_distance(distance);
            buf[distance++] = 0;
            arguments[i++] = pointer;
            pointer = buf + distance;
            continue;
        }

        if (ch != '\n'){
            control_distance(distance);
            buf[distance++] = ch;           
        } else{			//case ch = '\n'
            arguments[i++] = pointer;
            pointer = buf + distance;

            int pid = fork();
            if (pid == 0){
                // child
                exec(arguments[0], arguments);
            } else{
                // parent
                wait(0);
            }

            i = argc - 1;
        }
    }
    exit(0);
}
