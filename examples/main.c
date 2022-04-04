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

    for (int i = 0; i < 33 ; i ++)
    {
        printf("%d\n",i);
        elfd_open("./test");
    }

    elfd_fini();

    return 0;
}