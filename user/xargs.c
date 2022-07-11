#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void forkexec(char *cmd, char **argv)
{
    if (fork() == 0)
    {
        exec(cmd, argv);
        exit(1);
    }
    else
        wait(0);
}

int readline(char *argv[MAXARG], int start)
{
    char buf[1024];
    int n = 0;
    while (read(0, buf+n, 1))
    {
        if (buf[n] == '\n')
            break;
        ++n;
    }
    buf[n] = 0;
    if (n == 0)
        return 0;
    int x = 0;
    while (x < n)
    {
        argv[start++] = buf + x;
        while (buf[x] != ' ' && x < n)
            ++x;
        while (buf[x] == ' ' && x < n)
            buf[x++] = 0;
    }
    return start;
}

int main(int argc, char *argv[])
{
    char *cmd = (char *)malloc(512*sizeof(char));
    strcpy(cmd, argv[1]);

    char *new_argv[MAXARG];
    for (int i = 0; i < MAXARG; ++i)
        new_argv[i] = (char *)malloc(512*sizeof(char));
    
    int argv_start_idx = 0;
    for (int i = 1; i < argc; ++i)
        strcpy(new_argv[argv_start_idx++], argv[i]);


    int curr_argv_idx;
    while ((curr_argv_idx = readline(new_argv, argc-1)) != 0)
    {
        new_argv[curr_argv_idx] = 0;
        forkexec(cmd, new_argv);
    }

    exit(0);
}