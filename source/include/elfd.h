/* File : elfd.h */
/* Info : library header */
/* Note : All of theses functions should sanitize user input */

#ifndef _ELFD_H
#define _ELFD_H

/* 
    Init the library structures
    @return: 0 if success, -1 if failed
*/
int elfd_init();
/*
    Finit the library, free all the internals structures, unmap files, close fd
    @return: 0 if succes, if failed exit the program
*/
int elfd_fini();

/*
    Open an elf file with the library
    @arg1: path to the elf-file
    @return: an elfd-descriptor to access to this file with library functions, if fail -1
*/
int elfd_open(const char *);

/*
    Close an elf file, free all structures associated with him, unmap the file and close fd
    @arg1: elfd-descriptor
    @return: 0 if success, -1 if failed
*/
int elfd_close(int);


#endif