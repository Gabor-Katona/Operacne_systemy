#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// find all files with specific name in directory
// and writes path to found files.
// use: find <start_directory> <file_name>
void 
find(char *start, char *file)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(start, 0)) < 0){
        fprintf(2, "find usage: cannot open %s\n", start);
        return;
    }

    if (fstat(fd, &st) < 0){
        fprintf(2, "find usage: cannot stat %s\n", start);
        close(fd);
        return;
    }

    if (st.type != T_DIR){
        fprintf(2, "find usage: %s is not a directory\n", start);
        close(fd);
        return;
    }

    if(strlen(start) + 1 + DIRSIZ + 1 > sizeof buf){
        fprintf(2, "find usage: path too long\n");
        close(fd);
        return;
    }

    strcpy(buf, start);
    p = buf+strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0){
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        
        if(stat(buf, &st) < 0){
            fprintf(2, "find usage: cannot stat %s\n", buf);
            continue;
        }
        
        // skip current directory and parent directory entries
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;
        
        if(!strcmp(de.name, file))
            printf("%s\n", buf);
   
        if(st.type == T_DIR)
            find(buf, file);  	
    }
    close(fd);
}

int 
main(int argc, char *argv[])
{
    if (argc < 3){
        fprintf(2, "find usage: too few arguments\n");
        exit(-1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
