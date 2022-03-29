#include <stddef.h>
#include <sys/types.h>

#ifndef _ELFD_STRUCTS_H
#define _ELFD_STRUCTS_H

#define MAX_PATH_SIZE 256


typedef struct {
    
    int fd;
    
    const char path[MAX_PATH_SIZE];
    
    off_t size;

    void * addr;
    
    int user_handle;
    
    void * next;
    void * prev;

    short is_linked;
    
} elfd_file;


typedef struct {
    
    elfd_file * entry;
    
    int list_size;
    
    int latest_handler;
    
    elfd_file * last_file;

} elfd_file_list_entry;

#define ELFDFILE_SIZE sizeof(elfd_file)
#define ELFDFLE_SIZE sizeof(elfd_file_list_entry)

#endif