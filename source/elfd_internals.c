#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "include/elfd_internals.h"
#include "include/elfd_structs.h"

#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#endif

int _elfd_list_remove_file(int handler);
int _elfd_list_add_file(int handler);

/* 
    Append @arg1 to the end of the double linked list, set is_linked
    @arg0: pointer to the  elfd_file_list_entry
    @arg1: elfd_file * to append at the end of the elfd_file double linked list
    @return: 0 if success, -1 if failed, no warning
*/
int _elfd_append_file(elfd_file_list_entry * entry, elfd_file * a)
{
#ifdef DEBUG
    printf("%p, %p\n", entry, a);
#endif
    elfd_file * last_file;

    if (entry == NULL | a == NULL)
    {
        return -1;
    }

    last_file = entry->last_file;
    
    if(last_file != NULL)
    {
        last_file->next = a;
    }
    entry->last_file = a;
    
    if(entry->entry == NULL)
    {
        entry->entry = a;
    }
    a->is_linked = 0;

    return 0;
}

/* 
    Insert struct elfd_file in file dll
    @arg0: elfd_file *, struct elfd_file to insert
    @arg1: elfd_file *, where insert the elfd_file 
    @return: 0 if success, -1 if failed, no warning
*/
int _elfd_insert_file(elfd_file * a, elfd_file * b)
{
    if (a == NULL || b == NULL)
    {
        return -1;
    }

    if(!b->is_linked)
    {
        return -1;
    }

    a->next = b->next;
    b->next = (void *)a;
    a->prev = (void *)b;

    return 0;
}

/* 
    Remove struct elfd_file in file dll 
    @arg0: pointer to the elfd_file_list_entry
    @arg1: pointer to the elfd_file to remove
    @return: 0 if success, -1 if failed, no warning
*/
int _elfd_unlink_file(elfd_file_list_entry * entry, elfd_file * a)
{
#ifdef DEBUG
    printf("_elfd_unlink_file(%p, %p)\n", entry, a);
#endif
    elfd_file * next_file;
    elfd_file * prev_file;
    

    switch(a)
    {
        case (a==NULL | !a->is_linked):
            return -1;
            break;
        case (a->nexv       t == NULL):
            /* a is at the end of the dll */
            prev_file = a->prev;
            prev_file->next=NULL;
            break;
        case (a->prev == NULL):
            /* a is at the start of the dll */
            entry->entry = NULL;
            break;
        default:
            /* a is in the dll */
            next_file = (elfd_file *)a->next;
            prev_file = (elfd_file *)a->prev;
            next_file->prev = prev_file;
            prev_file->next=next_file;
            break;
    }

    a->is_linked = -1;
    entry->list_size -= 1; 

    return 0;
}

elfd_file * _elfd_get_file(elfd_file_list_entry * entry ,int handler)
{
#ifdef DEBUG
    printf("_elfd_get_file(%p, %d)\n", entry, handler);
#endif
    elfd_file * current;
    if(entry->entry == NULL | entry->last_file == NULL)
        return (elfd_file*)-1;
    
    current = entry->entry;

    /* iterate over the dll */
    for (int i = 0; i < entry->list_size; i++)
    {   
#ifdef DEBUG
        printf("_elfd_get_file(): [%d] current->user_handler = %d, handler = %d\n", i, current->user_handle, handler);
#endif
        if(current->user_handle == handler)
        {
            return current;
        } 
        current = current->next;
    }

    return (elfd_file*)-1;
}


/*
    Register elfd_file struct on the heap (zeroed it) and return a pointer to it 
    @return: a pointer to an elfd_file struct or -1 if failed     
*/
elfd_file * _register_elfd_file()
{
    elfd_file * p;
    if((p = (elfd_file*)malloc(ELFDFILE_SIZE)) == (elfd_file*)-1)
    {
        return (elfd_file *)-1;
    }

    memset(p, 0x0, ELFDFILE_SIZE);

    p->is_linked = -1;

    return p;
}

/* 
    Free an elfd_file struct on the heap (zeroed it) and unmap associed mapping if there is one
    @arg0: pointer to the elfd_file to unregister
*/
void _unregister_elfd_file(elfd_file * p)
{
    if(p != NULL)
    {
        if(p->addr !=NULL)
        {
            munmap(p, p->size);
        }
        memset(p, 0x0, ELFDFILE_SIZE);
        free(p);
    }
}

/*
    Clear all the elfd_file in the dll
    @arg0: pointer to elfd_file_list
    @return: 0 if success, -1 if failed
*/
int _elfd_list_clear(elfd_file_list_entry * entry)
{
    elfd_file * next;
    elfd_file * current;

    /* If the dll is empty, there is nothing to clear */
    if(entry->entry == NULL)
    {
        return 0;
    }

    next = entry->entry;
    
    /* Free all elfd_file struct in the dll */
    for (int i = 0; i < entry->list_size;)
    {
        current = next;
        next = current->next;
        _unregister_elfd_file(current);
    }

    return 0;

}