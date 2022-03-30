/* File : elfd.h */
/* Info : library header */
/* Note : All of theses functions should sanitize user input */


#ifndef _ELFD_H
#define _ELFD_H

#include "elfd_private.h"
#include "elfd_structs.h"

int elfd_init();
int elfd_fini();

int elfd_open(const char *);
int elfd_close(int);


#endif