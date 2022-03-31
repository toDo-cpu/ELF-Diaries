/* File : elfd_header.h */
/* Info : contain all of the internals structures used by the library and some other stuff */

#include <stddef.h>
#include <sys/types.h>

#ifndef _ELFD_STRUCTS_H
#define _ELFD_STRUCTS_H

#define MAX_PATH_SIZE 256
#define _COLLECTION_PAGE_SIZE 0x20

//This struct handle 1 elf opened elf file
typedef struct {
    /* Internals */
  	int fd;
    const char path[MAX_PATH_SIZE];	/* Path the the file */
    void * addr;					/* Address to mapped file */    
    off_t len;
    /* Returned to the user */
    int user_handle;				/* number sent to the user to keep track of the file */
} elfd_file;

// Collection of elfd_file, save pointers to them
typedef struct {
   	int last_user_handle;	/* To keep track the user handler */
    int item_count;	/* Total number of pointer in the collection */				
    int item_used;	/* Total number of valid pointer in the collection */
    elfd_file ** last_freed_item; /* used as a cache which store the last entry which have been freed */
    elfd_file ** collection;	/* Pointer to the array of pointers to elfd_file */
}elfd_files_collection;
#endif