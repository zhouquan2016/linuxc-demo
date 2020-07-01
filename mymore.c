#include <stdio.h>
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
    while (fgets(readBuff, LINELEN, readIn))
    {
        //如果打印了一屏，就等待用户输入
        if (lineNums == PAGELINES)
        {
            reply = see_more();
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
}

int see_more()
{
    printf("\033[41;36mmore\033[0m");
    switch (getchar())
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