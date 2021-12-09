#include <stdio.h>

#include "bipipe.h"

#define BUF_SIZE 0x100

void child_process_function(struct bipipe parent);

int main(void)
{
    struct bipipe child;

    printf("ctrl+d = gracefully close\n\n");

    printf("Forking rot13 processor...\n");
    child = bipipe_fork(child_process_function);

    while(1)
    {
        char buffer[BUF_SIZE] = {};

        printf("rot13> ");
        if(fgets(buffer, BUF_SIZE, stdin))
        {
            write(child.out, buffer, BUF_SIZE);

            char returned[BUF_SIZE] = {};
            read(child.in, returned, BUF_SIZE);

            printf(" = %s\n\n", returned);
        }
        else
        {
            printf("Exiting...\n");
            close(child.in);
            close(child.out);
            exit(0);
        }
    }

}

static char rot13(char c)
{
    if('a' <= c && c <= 'z')
    {
        c -= 'a' - 13;
        if(c >= 26) c -= 26;
        c += 'a';
    }

    else if('A' <= c && c <= 'Z')
    {
        c -= 'A' - 13;
        if(c >= 26) c -= 26;
        c += 'A';
    }

    return c;
}

void child_process_function(struct bipipe parent)
{
    int bytes_read;
    char buffer[BUF_SIZE];
    char *i;

    while(1)
    {   
        memset(buffer, '\0', BUF_SIZE);
        bytes_read = read(parent.in, buffer, BUF_SIZE);

        if(bytes_read)
        {
            for(i = buffer; *i; ++i)
            {
                *i = rot13(*i);
            }

            write(parent.out, buffer, bytes_read);
        }

        else
        {
            printf("Child Exiting...\n");
            return;
        }
    }
}