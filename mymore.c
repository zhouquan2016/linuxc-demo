#include <stdio.h>
#include<stdlib.h>
#define PAGELINES 24
#define LINELEN 512

void do_more(FILE *);
int see_more();
int main(int argc, char const *argv[])
{

    if (argc == 1)
    {
        do_more(stdin);
    }
    else
    {
        FILE *readIn;
        while (--argc)
        {
            if ((readIn = fopen(*++argv, "r")) != NULL)
            {
                do_more(readIn);
                fclose(readIn);
            }
            else
            {
                fprintf(stderr, "%s无法打开\n", *argv);
            }
        }
    }

    return 0;
}

void do_more(FILE *readIn)
{
    char readBuff[LINELEN];
    int lineNums = 0, reply = 0;
    FILE *ttyFile = fopen("/dev/tty", "r");
    if (ttyFile == NULL)
    {
        fprintf(stderr, "无法打开tty");
        exit(1);
    }
    
    while (fgets(readBuff, LINELEN, readIn))
    {
        //如果打印了一屏，就等待用户输入
        if (lineNums == PAGELINES)
        {
            reply = see_more(ttyFile);
            if (!reply)
            {
                break;
            }
            lineNums -= reply;
        }
        if (fputs(readBuff, stdout) == EOF)
        {
            break;
        }
        ++lineNums;
    }
    fclose(ttyFile);
}

int see_more(FILE * cmd)
{
    printf("\033[41;36mmore\033[0m");
    switch (getc(cmd))
    {
    case 'q':
        return 0;
    case ' ':
        return PAGELINES;
    case '\n':
        return 1;
    default:
        break;
    }

    return 0;
}