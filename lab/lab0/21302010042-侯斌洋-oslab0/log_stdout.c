#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stdarg.h>

char buf[1024];

void creat_log_name(int i, char *s);

int read_stdin(char *buf)
{
    /*
    Description: Read stdin into buf
    Example:
        - read_stdin(); // Read the stdin into buf
    Parameters:
        - buf (char*): A buffer to store all characters
    Return:
        - 0 (int)
    */
    // Your code here
    if (read(0, buf, sizeof(buf)) < 0)
    {
        printf("Read error\n");
    }
    // End
    return 0;
}

int log_stdout(uint i)
{
    /*
    Description: Redirect stdout to a log file named i.log
    Example:
        - log_stdout(1); // Redirect the stdout to 1.log and return 0
    Parameters:
        - i (uint): A number
    Return:
        - 0 (int)
    */
    char log_name[15] = "0.log";
    // Your code here
    creat_log_name((int)i, log_name);
    int fd = open(log_name, O_RDWR | O_CREATE); // O_CREATE is necessary!
    close(1);
    dup(fd);
    close(fd);
    // End
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(2, "Usage: log_stdout number\n");
        exit(1);
    }
    if (log_stdout(atoi(argv[1])) != 0)
    {
        fprintf(2, "log_stdout: log_stdout failed\n");
        exit(1);
    }
    if (read_stdin(buf) != 0)
    {
        fprintf(2, "log_stdout: read_stdin failed\n");
        exit(1);
    }
    printf(buf);
    exit(0);
}

void creat_log_name(int i, char *s)
{
    int a;
    int b = 0;                 
    char *pString, pBuffer[10]; 
    pString = s;               
    if (i < 10)
    {
        *pString = i + '0';
        pString++;
    }
    else
    {
        while (i > 0)
        {
            a = i % 10;
            i = i / 10;
            pBuffer[b] = a + '0'; // 通过计算把数字编成ASCII码形式
            b++;
            if (b > 10)
            {
                printf("Error:file name too long\n");
                break;
                return;
            }
        }
        b--;
        for (; b >= 0; b--) // 将得到的字符串倒序
        {
            *pString = pBuffer[b];
            pString++;
        }
    }
    *pString++ = '.';
    *pString++ = 'l';
    *pString++ = 'o';
    *pString++ = 'g';
    *pString = '\0';
}
