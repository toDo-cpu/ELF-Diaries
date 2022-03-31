#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "include/elfd_private.h"
#include "include/elfd_structs.h"

/*	
	Allocate on elfd_file on the heap and zeroed it
	@return: pointer to elfd_file or -1 if failed
*/
efd_file * _elfd_register_elfd_file()
{
    elfd_file * p;

    p = malloc(sizeof(elfd_file));

    if(!p)
        goto err;

    memset((void*)p, 0x00, sizeof(elfd_file));

    return p;
err:
    return -1
}
/*
	Free an elfd_file on the heap, zeroed it before
	@arg1: a valid pointer to an elfd_file struct allocated on the heap to free
	@return: None
*/
void _elfd_unregister_elfd_file(elfd_file * p)
{
    if(elfd_file != NULL)
    {
        memset((void*)p, 0x00, sizeof(elfd_file));
        free(p);
    }
}
/*
	Append elfd_file in the collection, 
	if needed realloc the collection array. finally it increase item_used field 
	@arg1: pointer to the collection
	@arg2: pointer to the elfd_file
	@return: 0 if success, -1 if failed
*/
int _elfd_collection_append_elfd_file(elfd_files_collection * collection_obj, elfd_file * file)
{
	if(new_file == -1)
		return -1;

	/* If the cache contain an free entry */
	if(collection_obj->last_freed_item != NULL)
	{
		*(collection->last_freed_item) = new_file;
		collection_obj->item_used++;
		return 0
	}

	/* If the collection is full, resize it */
	if(collection_obj->item_count == collection_obj->item_used)
	{
		if(!_elfd_collection_resize(collection_obj))
			return -1;
	}

	for (int i = 0; i < item_count; i++)
	{
		if(collection_obj->collection[i] == NULL)
		{
			collection_obj->collection[i] = file;
			collection_obj->item_used++;
			return 0;
		}
	}

	return -1;
}
/*
	Remove elfd_file in the collection
	@arg1: pointer to the collection
	@arg2: handler
	@return: 0 if success, -1 if failed
*/
int _elfd_collection_remove_elfd_file(elfd_files_collection * collection_obj, int handler)
{
	elfd_file * current;
	int item_used = 0;
	
	/* For each entry in the collection */
	for (int i = 0; i < collection_obj->item_count; i++)
	{
		if(item_used == collection_obj->item_used)
		{
			/* We have check for every elfd_file in the list so the user handler isn't here */
			return -1;
		}

		current = collection_obj->collection[i];

		/* current = NULL if the current pointer in the collection isn't used */
		if(current != NULL)
		{
			if(current->user_handle == handler)
			{
				/* current point the right elfd_file */
				_elfd_destruct_elfd_file(current);

				collection_obj->collection[i] = NULL;
				*collection_obj->last_freed_item =  &collection_obj->collection[i];
				collection_obj->item_used--;
				
				return 0;
			}

			item_used++;	/*Count used entry, so we can detect if we have checked all the allocated entry */
		} 
		/* If last_freed_item is null and collection_obj->collection[i] is null too, we can assign it 
			to speed up the next elfd_open()*/
		else if(*collection_obj->last_freed_item == NULL)
		{
			*collection_obj->last_freed_item = &collection_obj->collection[i];
		}
	}

	return -1;
}
/*
	Get a elfd_file in the collection
	@arg1: pointer the the collection
	@arg2: a sanitized user handler
	@return: a pointer to the corresponding elfd_file struct, -1 if failed or the elfd_file not found 
*/
elfd_file * _elfd_collection_get_file(elfd_files_collection * collection_obj, int handler)
{
	elfd_file * current;
	int item_used = 0;
	for (int i = 0; i < collection_obj->item_count; i ++)
	{
		if(item_used == collection_obj->item_used)
			return -1;
		current = collection_obj->collection[i];

		if(current != NULL)
		{
			if(current->user_handle == handler)
				return current;
			item_used++;
		}
	}

	return -1;
}

/*
	Resize the collection with realloc
	@arg1: pointer to the collection
	@return: 0 if sucess, -1 if failed
*/
int _elfd_collection_resize(elfd_file_collection * collection_obj)
{
	if(realloc(collection_obj->collection, _COLLECTION_PAGE_SIZE, sizeof(elfd_file *)) == NULL)
	{
		return -1;
	}

	collection_obj->item_count += _COLLECTION_PAGE_SIZE;

	return 0;
}
/*
	Destruct elfd_file, unmap it, close fd and free unregister it
	@arg1: pointer to an elfd_file
	@return: none
*/
void _elfd_destruct_elfd_file(elfd_file * victim)
{
	unmap(victim->addr, victim->len);
	close(victim->fd);
	_elfd_unregister_elfd_file(victim);
}

