#ifndef _ELFD_ERR_H
#define _ELFD_ERR_H

#include <stdio.h>
#include "elfd_structs.h"

#ifdef DEBUG
#define debug(from, content) do { printf("DEBUG - %s: %s\n", from , content); } while(0)
#else 
#define debug(from, content)
#endif

void elfd_warning(const char *from, const char *message);
void elfd_critical(const char *from, const char *message);

#endif