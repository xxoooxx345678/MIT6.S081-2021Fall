#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUMBER_UPPER_BOUND 35

int main()
{
    int ancestor = getpid(), a_idx = 2, g_p_idx = 0, pr, l_p_idx = 0;
    int p[256][2];
    int forked;

start:
    forked = 0;
    if (getpid() == ancestor)
        pr = a_idx++;
    else
    {
        close(p[l_p_idx-1][1]);
        read(p[l_p_idx-1][0], &pr, sizeof(pr));
    }
    printf("prime %d\n", pr);

    while (1)
    {
        int n;
        if (getpid() == ancestor)
        {
            n = a_idx++;
            if (n > NUMBER_UPPER_BOUND)
            {
                n = -1;
                write(p[l_p_idx][1], &n, sizeof(n));
                wait(0);
                exit(0);
            }
        }
        else
            read(p[l_p_idx-1][0], &n, sizeof(n));

        if (n == -1)
        {
            if (forked)
            {
                write(p[l_p_idx][1], &n, sizeof(n));
                wait(0);
            }
            exit(0);
        }

        if (n % pr)
        {
            if (!forked)
            {
                pipe(p[l_p_idx]);
                ++g_p_idx;
                if (fork() == 0)
                {
                    l_p_idx = g_p_idx;
                    goto start;
                }
                else
                {
                    l_p_idx = g_p_idx - 1;
                    close(p[l_p_idx][0]);
                }
                forked = 1;
            }
            write(p[l_p_idx][1], &n, sizeof(n));
        }
    }
}