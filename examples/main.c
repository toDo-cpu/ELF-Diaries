#include <elfd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char ** argv)
{
    int handler_1;
    int handler_2;

    if(elfd_init() == -1)
    {
        perror(NULL);
    }

    if((handler_1 = elfd_open("./test")) == -1)
    {
        perror(NULL);
    }

    printf("handler 1: %d\n", handler_1);


    if((handler_2 = elfd_open("./test")) == -1)
    {
        perror(NULL);
    }

    printf("handler 2: %d\n", handler_2);  

    elfd_close(handler_2);
    elfd_close(handler_2);

    elfd_fini();

    return 0;
}