/* File : elfd_private.h */
/* Info : contain all private functions ( such as init functions, free etc )*/
/* Note : ALL input must be sanitizied before been passed to theses functions */

#ifndef _ELFD_PRIVATE_H
#define _ELFD_PRIVATE_H

#include "elfd_structs.h"
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
	@return: a pointer to the corresponding elfd_file struct, -1 if failed or the elfd_file not found 
*/
elfd_file * _elfd_collection_get_file(elfd_files_collection *, int);
/*
	Resize the collection with realloc
	@arg1: pointer to the collection
	@return: 0 if sucess, -1 if failed
*/
int _elfd_collection_resize(elfd_file_collection *);
/*
	Destruct elfd_file, unmap it, close fd and free unregister it
	@arg1: pointer to an elfd_file
	@return: none
*/
void _elfd_destruct_elfd_file(elfd_file *);
#endif