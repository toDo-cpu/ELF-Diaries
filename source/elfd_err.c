#include "include/elfd_err.h"
#include "include/elfd_internals.h"
#include <stdio.h>

void elfd_warning(const char *from, const char *message)
{
    printf("ELFD WARNING from %s: %s\n", from, message);
}
void elfd_critical(const char *from, const char *message)
{
    printf("ELFD CRITICAL from %s: %s\n", from, message);
    printf("ELFD CRITICAL exiting ...\n");
    //_elfd_exit();
}