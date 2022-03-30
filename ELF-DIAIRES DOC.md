# ELF-DIAIRES DOC



```c
/* File : elfd_header.h */

//This struct handle 1 elf opened elf file
typedef struct {
    /* Internals */
  	int fd;
    const char path[MAX_PATH_SIZE];	/* Path the the file */
    void * addr;					/* Address to mapped file */    
    /* Returned to the user */
    int user_handle;				/* number sent to the user to keep track of the file */
} elfd_file;

// Collection of elfd_file, save pointers to them
typedef struct {
   	int last_user_handle;	/* To keep track the user handler */
    int item_count;	/* Total number of pointer in the collection */				
    int item_used;	/* Total number of valid pointer in the collection */
    elfd_file * last_freed_item;	/* Point the last item in the collection array which have been freed */
    elfd_file ** collection;	/* Pointer to the array of pointers to elfd_file */
}elfd_files_collection;

```

