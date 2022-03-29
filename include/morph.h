#ifndef MORPH
#define MORPH

#define MORPH_BASIC

/*******************************************************************************
 *
 * TYPE DEFINITIONS
 *
 ******************************************************************************/

typedef enum
{
	M_SINGLE_LINKED_LIST = 0,
	M_DOUBLE_LINKED_LIST,
	M_STACK,
	M_QUEUE,
	M_ARRAY,
	LIST_TYPE_COUNT
} morphtype;

typedef struct
{
  void **		data;
  unsigned int	size;
} array_s;

/*******************************************************************************
 *
 * INTERFACE
 *
 ******************************************************************************/

/*
 * create a new list with respect to it's type
 *
 * 1 type of list (see enum)
 */
extern void * mcreate
(morphtype);

/*
 * deallocate the memory occupied by a list and it's items
 *
 * 1 list
 * 2 function that deletes an item
 */
extern void * mdelete
(void *, void (*) (void *));

/*
 * search a list for an item with a distinct property and return a match by it's
 * first occurence
 *
 * 1 list
 * 2 property
 * 3 function that compares a property with an item
 * 4 function that duplicates an item if properties matched
 */
extern void * mfind
(void *, void *, unsigned char (*) (void *, void *), void * (*) (void *));

/*
 * search a list for items with a distinct property and put matching item into
 * a single linked list
 *
 * 1 list
 * 2 property
 * 3 function that compares a property with an item
 * 4 function that duplicates an item if properties matched
 */
extern void * mseek
(void *, void *, unsigned char (*) (void *, void *), void * (*) (void *));

/*
 * clear a list from of all of it's items
 *
 * 1 list
 * 2 function that deallocates the memory occupied by an item
 */
extern void mclear
(void *, void (*) (void *));

/*
 * iterate through a list and apply a function and an optional value to all
 * of it's items
 *
 * 1 list
 * 2 function that accepts a specific item and a pointer to a value [3]
 * 3 pointer to a value that is past as a second argument on [2]
 */
extern void mbatch
(void *, void (*) (void *, void *), void *);

/*
 * insert an item at a specific position in the list
 *
 * 1 list
 * 2 item to be inserted
 * 3 position
 * 4 function that duplicates [2] (optional, pass 0 to ignore)
 */
extern void minsert
(void *, void *, unsigned int, void * (*) (void *));

/*
 * remove items in a list that meet a certain condition
 *
 * 1 list
 * 2 property
 * 3 function that compares [2] (2nd) and the specific property of an item (1st)
 * 4 function that deallocates the memory occupied by an item (when 0, free ()
 *   will be applied
 */
extern void mremove
(void* list, void* value, unsigned char (*)(void *, void *), void (*)(void *));

/*
 * (bubble) sort the items of a list
 *
 * 1 list
 * 2 function compares to items of a list and return true if conditions are matched
 */
extern void msort
(void *, unsigned char (*) (void *, void *));

/*
 * insert an item on top of a list
 *
 * 1 list
 * 2 item
 */
extern void mpush
(void *, void *);

/*
 * remove the topmost item of a list
 *
 * 1 list
 * 2 function that deallocated the memory occupied by the topmost item, if zero
 *   free () will be applied
 */
extern void * mpop (void *);

/*
 * append the item at the end of a list
 *
 * 1 list
 * 2 item
 */
extern void madd
(void *, void *);

/*
 * remove the last item of a list
 *
 * 1 list
 * 2 function that deallocated the memory occupied by the topmost item, if zero
 *   free () will be applied
 */
extern void msub
(void *, void (*) (void *));

/*
 * append the item at the end of the list
 *
 * 1 list
 * 2 item
 */
extern void menqueue
(void *, void *);

/*
 * return (and remove) the topmost item of a list
 *
 * 1 list
 * 2 function that deallocated the memory occupied by the topmost item, if zero
 *   free () will be applied
 */
extern void * mdequeue (void *);

/*
 * return the item of a specific position in a list
 *
 * 1 list
 * 2 position
 */
extern void * mget
(void *, unsigned int);

/*
 * overwrite (or append if > size) an item at a specific position
 *
 * 1 list
 * 2 item
 * 3 position
 */
extern void mset
(void *, void *, unsigned int);

/*
 * return the topmost item of a list
 *
 * 1 list
 * 2 function that eventually duplicates the item, set to zero to ignore
 */
extern void * mpeek
(void *, void * (*) (void *));

/*
 * return the last item of a list
 *
 * 1 list
 * 2 function that eventually duplicates the item, set to zero to ignore
 */
extern void * mkeep
(void *, void * (*) (void *));

/*
 * returns the predecessor of an item in a list
 *
 * 1 list
 * 2 item, whose predecessor to be returned
 * 3 function that compares two item by a destinct property ([2]{1st})
 */
extern void * mpredecessor
(void *, void *, unsigned char (*) (void *, void *));

/*
 * returns the successor  of an item in a list
 *
 * 1 list
 * 2 item, whose successor to be returned
 * 3 function that compares two item by a destinct property ([2]{1st})
 */
extern void * msuccessor
(void *, void *, unsigned char (*) (void *, void *));

/*
 * create and returns an array of pointers, which reference to the list items
 *
 * 1 list
 * 2 function that duplicates an item, set to zero to ignore
 * 3 sizeof an pointer to an item
 */
extern array_s * mtoarray
(void *, void * (*) (void *), unsigned int);

/*
 * return the item count of a list
 *
 * 1 list
 */
extern unsigned int msize
(void *);

/*
 * return the type of a list
 *
 + 1 list
 */
extern morphtype mtype
(void *);

/*
 * function that create an eight byte (unsigned int) hash out of a string
 *
 * 1 string
 */
extern unsigned int mhash
(const char *);

#endif
