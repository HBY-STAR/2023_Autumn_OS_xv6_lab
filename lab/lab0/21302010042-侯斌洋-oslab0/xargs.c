#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stdarg.h>

#define MAXBUF 512

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }
    char *all_argvs[MAXARG];
    int argvs_num = 0;

    for (int i = 1; i < argc; i++)
    {
        all_argvs[argvs_num++] = argv[i];
    }
    // printf("1\n");
    char buf[MAXBUF] = {'\0'};
    while (read(0, buf, MAXBUF) > 0)
    {
        // printf("2\n");
        char temp[MAXBUF] = {'\0'};
        for (int i = 0; i < MAXBUF; i++)
        {
            if (buf[i] == '\n')
            {
                temp[i]='\0';
                all_argvs[argvs_num] = temp;
                // printf("3\n");
                if (fork() == 0)
                {
                    // printf("4\n");
                    exec(all_argvs[0], all_argvs);
                }
                wait(0);
                break;
            }
            else
            {
                // printf("9\n");
                temp[i] = buf[i];
            }
        }
    }

    return 0;
}