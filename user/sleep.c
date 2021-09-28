#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//wait number of ticks, returns -1 for error
//inputs: 1st argument- number of ticks
int 
main(int argc, char *argv[])
{
    int ticks;

    if(argc !=2){
        fprintf(2,"sleep usage: sleep <ticks>\n");
        exit(-1);
    }
    
    ticks = atoi(argv[1]);
    sleep(ticks);

    exit(0);
}
