#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *get_file_name(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    strcpy(buf, p);
    return buf;
}

void _find(char *path, char *target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
        case T_FILE:
            fprintf(2, "find: starting-point is a file %s\n", path);
            break;
        
        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de))
            {
                if (de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (stat(buf, &st) < 0)
                {
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                char fname[512];
                strcpy(fname, get_file_name(buf));

                if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0)
                    continue;

                if (strcmp(fname, target) == 0)
                    printf("%s/%s\n", path, target);
                else
                {
                    if (st.type == T_DIR)
                        _find(buf, target);
                }
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    _find(argv[1], argv[2]);
    exit(0);
}