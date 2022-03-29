#ifndef _ELFD_INTERNALS_H
#define _ELFD_INTERNALS_H

#include "elfd_structs.h"


void _elfd_exit();

int _elfd_list_clear(elfd_file_list_entry * entry);

elfd_file * _elfd_get_file(elfd_file_list_entry * entry ,int handler);

int _elfd_append_file(elfd_file_list_entry * entry, elfd_file * a);
int _elfd_insert_file(elfd_file * a, elfd_file * b);
int _elfd_unlink_file(elfd_file_list_entry * entry, elfd_file * a);


elfd_file * _register_elfd_file();
void _unregister_elfd_file(elfd_file * p);

#endif