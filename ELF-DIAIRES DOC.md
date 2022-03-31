# ELF-DIAIRES DOC

```c
/* File : elfd_header.h */
/* Info : contain all of the internals structures used by the library and some other stuff */


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
    elfd_file * last_freed_item; /* Point the last item in the collection array which have been freed,usefull for appending new item after closed one */
    elfd_file ** collection;	/* Pointer to the array of pointers to elfd_file */
}elfd_files_collection;
```

```c
/* File : elfd_private.h */
/* Info : contain all private functions ( such as init functions, free etc )*/
/* Note : ALL input must be sanitizied before been passed to theses functions */

/*	
	Allocate on elfd_file on the heap and zeroed it
	@return: pointer to elfd_file or -1 if failed
*/
efd_file * _elfd_register_elfd_file();
/*
	Free an elfd_file on the heap, zeroed it before
	@arg1: a valid pointer to an elfd_file struct allocated on the heap to free
	@return: None
*/
void _elfd_unregister_elfd_file(elfd_file *);
/*
	Append elfd_file in the collection, if needed realloc the collection array
	@arg1: pointer to the collection
	@arg2: pointer to the elfd_file
	@return: 0 if success, -1 if failed
*/
int _elfd_collection_append_elfd_file(elfd_files_collection *, elfd_file *);
/*
	Remove elfd_file in the collection
	@arg1: pointer to the collection
	@arg2: handler
	@return: 0 if success, -1 if failed
*/
int _elfd_collection_remove_elfd_file(elfd_files_collection *, int);
/*
	Get a elfd_file in the collection
	@arg1: pointer the the collection
	@arg2: a sanitized user handler
	@return: a pointer to the corresponding elfd_file struct, -1 if failed or the elfd_file not found */
*/
elfd_file * _elfd_collection_get_file(elfd_files_collection *, int);
/*
	Resize the collection with realloc
	@arg1: pointer to the collection
	@return: 0 if sucess, -1 if failed
*/
int _elfd_collection_resize(elfd_file_collection *);
```

```c
/* File : elfd.h */
/* Info : library header */
/* Note : All of theses functions should sanitize user input */

int elfd_init();
void elfd_fini();

int elfd_open(const char *);
int elfd_close(int);
```

## Exceptions handling
- use functions in **elfd_err.h**
- the fonction where an error occured call **elfd_err/elfd_warn** and return -1