#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>

#include "include/elfd.h"
#include "include/elfd_structs.h"
#include "include/elfd_err.h"
#include "include/elfd_internals.h"

elfd_file_list_entry * elfd_file_list = NULL;
short elfd_is_file_list_init;


/* Allocate and free internals structures used by the library */
int elfd_init()
{

    /* Init elfd_file_list struct */
    if((elfd_file_list = (elfd_file_list_entry *)malloc(ELFDFLE_SIZE)) == (elfd_file_list_entry *)-1)
    {
        elfd_warning("elfd_init()","Can't allocate memory for struct elfd_file_list");
        return -1;
    }
        
    elfd_file_list->latest_handler = 0;
    elfd_file_list->list_size = 0;
    elfd_file_list->entry = NULL;

    elfd_is_file_list_init = 0;

    return 0;
    
}

int elfd_fini()
{
    if(_elfd_list_clear(elfd_file_list) == -1)
    {
        elfd_warning("elfd_fini()","Can't clear the elfd_file_list");
        return -1;
    }

    memset(elfd_file_list, 0x0, ELFDFLE_SIZE);
    free(elfd_file_list);
    elfd_file_list = NULL;

    return 0;
}

/* Open and elf file and save it in the elf_vector */
int elfd_open(const char * path)
{
    int fd;
    struct stat stat_buffer; 
    char file_buffer[4];
    void * mmapped_addr;
    elfd_file * elf_file;

    memset(&stat_buffer, 0x0, sizeof(stat_buffer));
    memset(file_buffer, 0x0, sizeof(file_buffer));

    /* Open the file */
    if(access(path, F_OK | R_OK) == -1)
    {
        elfd_warning("elfd_open()","The given file doesn't have perms to read / doesn't exist, access()");
        return -1;
    }

    if((fd = open(path, O_RDONLY)) == -1)
    {
        elfd_warning("elfd_open()","opening the given file failed, open()");
        return -1;
    }

    /* Check if this is an elf file */
    if(read(fd, file_buffer, sizeof(file_buffer)) == -1)
    {
        elfd_warning("elfd_open()","Can not determine if the file is an elf file, read()");
        goto _err;
    }
    if(!strncmp(file_buffer, ELFMAG, sizeof(ELFMAG)))
    {
        elfd_warning("elfd_open()","The given file isn't an elf file, strncmp()");
        goto _err;
    }

    /* Register elfd_file struct to handle the elf file */
    if((elf_file = _register_elfd_file()) == (elfd_file*)-1)
    {
        elfd_warning("elfd_open()", "Allocating memory to elfd_file struct failed");
        goto _err;
    } 

    /* Get the file size */
    if(stat(path, &stat_buffer )== -1)
    {
        elfd_warning("elfd_open()","Can not get the file size, stat()");
        goto _err_elf_file;
    }

    /* Mmap file */
    if( (mmapped_addr = mmap(NULL, stat_buffer.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    {
        elfd_warning("elfd_open()","Map file failed, mmap()");
        goto _err_elf_file;
    }

    /* Fill the elfd_file struct and append it to the dll */
    elf_file->fd = fd;
    memcpy((void*)elf_file->path, path, MAX_PATH_SIZE);
    elf_file->addr = mmapped_addr;
    elf_file->user_handle = elfd_file_list->latest_handler++;
    elf_file->size = stat_buffer.st_size;

    elfd_file_list->list_size++;

    if(_elfd_append_file(elfd_file_list, elf_file) == -1)
    {
        elfd_warning("elfd_open()","Linking the elf_file to the intern dll failed");
        goto _err_elf_file;
    }

    return elf_file->user_handle;

_err_elf_file:
    _unregister_elfd_file(elf_file);
_err:
    close(fd);
    return -1;
} 

/* Close an elf file and free all structures */
int elfd_close(int handler)
{
    elfd_file * victim;
    /* Sanitize handler */
    if(handler > elfd_file_list->latest_handler )
    {
        elfd_warning("elfd_close()","Bad handler");
        return -1;
    }

    if((victim = _elfd_get_file(elfd_file_list, handler)) == (elfd_file *)-1)
    {
        elfd_warning("elfd_close()","elfd_file not found");
        return -1;
    }

    _elfd_unlink_file(elfd_file_list, victim);    
    _unregister_elfd_file(victim);

    return 0;
}

/* Print differents header in stdout */
void elfd_dump_hdr(int handler);
void elfd_dump_pht(int handler);
void elfd_dump_sht(int handler);

/* Get the base address of some headers */
void * elfd_get_hdr(int handler);
void * elfd_get_pht(int handler);
void * elfd_get_sht(int handler);
