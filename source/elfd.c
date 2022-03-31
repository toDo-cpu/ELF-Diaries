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
#include "include/elfd_private.h"

elfd_files_collection * collection_obj = NULL;

int elfd_fini()
{
    elfd_file * current;
    if(collection_obj == NULL)
        return -1;
    
    for (int i = 0; collection_obj->item_count; i++)
    {
        if(current != NULL)
        {
            _elfd_destruct_elfd_file(current);
        }
    }

    return 0;
}

int elfd_init()
{
    /* Set up the collections */
    collection_obj = malloc(sizeof(elfd_files_collection));

    if(collection_obj == NULL)
        goto err;
    
    memset((void*)collection_obj, 0x00, sizeof(elfd_files_collection));

    /* Set up the array */
    void * arr = calloc(_COLLECTION_PAGE_SIZE, sizeof(elfd_file *));
    
    if(arr == NULL)
    {
        elfd_warning("elfd_init","Can not aklocate the collection vector");
        goto err_free;
    }

    memset((void*)arr, 0x00, sizeof(elfd_file *) * _COLLECTION_PAGE_SIZE);

    /* Fill the collection struct */
    collection_obj->last_user_handle = 0;
    collection_obj->item_count = _COLLECTION_PAGE_SIZE;
    collection_obj->item_used = 0x0;
    collection_obj->last_freed_item;
    collection_obj->collection = (elfd_file **)arr;

    return 0;
err_free:
    free(collection_obj);
    collection_obj = NULL;
err:
    return -1;
}

int elfd_close(int elfd_descriptor)
{
    if(elfd_descriptor < 0 || elfd_descriptor > collection_obj->last_user_handle)
    {
        elfd_warning("elfd_close","given elfd_descriptor is out of range");
        return -1;
    }
    if(_elfd_collection_remove_elfd_file(collection_obj, elfd_descriptor) == -1)
    {
        return -1;
    }
    return 0;    
}

/* Open and elf file and save it in the elf_vector */
int elfd_open(const char * path)
{
    int fd;
    struct stat stat_buffer; 
    char file_buffer[4];
    void * mmapped_addr;
    elfd_file * file;

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
    if((file = _elfd_register_elfd_file()) == (elfd_file*)-1)
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
    if((mmapped_addr = mmap(NULL, stat_buffer.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    {
        elfd_warning("elfd_open()","Map file failed, mmap()");
        goto _err_elf_file;
    }

    /* Fill the elfd_file struct and append it to the dll */
    file->fd = fd;
    memcpy((void*)file->path, path, MAX_PATH_SIZE);
    file->addr = mmapped_addr;
    file->user_handle = collection_obj->last_user_handle++;
    file->len= stat_buffer.st_size;

    if(_elfd_collection_append_elfd_file(collection_obj, file) == -1)
    {
        elfd_warning("elfd_open()","appending the file in the collection failed");
        goto _err_unmap;
    }
    
    return file->user_handle;

_err_unmap:
    munmap(file->addr, file->len);
_err_elf_file:
    _elfd_unregister_elfd_file(file);
_err:
    close(fd);
    return -1;
} 

/* Print differents header in stdout */
void elfd_dump_hdr(int handler);
void elfd_dump_pht(int handler);
void elfd_dump_sht(int handler);

/* Get the base address of some headers */
void * elfd_get_hdr(int handler);
void * elfd_get_pht(int handler);
void * elfd_get_sht(int handler);
