#ifndef _ELFD_H
#define _ELFD_H

/* Allocate and free internals structures used by the library */
int elfd_init();
int elfd_fini();

/* Open and elf file and save it in the elf_vector */
int elfd_open(const char * path); 

/* Close an elf file and free all structures */
int elfd_close(int handler);

/* Print differents header in stdout */
void elfd_dump_hdr(int handler);
void elfd_dump_pht(int handler);
void elfd_dump_sht(int handler);

/* Get the base address of some headers */
void * elfd_get_hdr(int handler);
void * elfd_get_pht(int handler);
void * elfd_get_sht(int handler);


#endif