#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p[2][2];
    int child_pid, parent_pid;

    pipe(p[0]);
    pipe(p[1]);

    if ((child_pid = fork()) == 0) // child
    {
        close(p[0][1]);
        close(p[1][0]);

        char buf[1];

        read(p[0][0], buf, 1);
        write(p[1][1], buf, 1);

        child_pid = getpid();
        printf("%d: received ping\n", child_pid);
    }
    else // parent
    {
        close(p[0][0]);
        close(p[1][1]);

        char buf[1] = "0";

        write(p[0][1], buf, 1);
        read(p[1][0], buf, 1);

        wait(0);

        parent_pid = getpid();
        printf("%d: received pong\n", parent_pid);
    }
    exit(0);
}