#include <stdio.h>
#include <stdlib.h>

#include "morph.h"

/*******************************************************************************
 *
 * MACRO
 *
 ******************************************************************************/

#define SET_U08_CONST(ITEM,VALUE)						\
*((unsigned char *) &ITEM) = VALUE;

#define SET_U16_CONST(ITEM,VALUE)						\
*((unsigned short*) &ITEM) = VALUE;

#define SET_U32_CONST(ITEM,VALUE)						\
*((unsigned int*) &ITEM) = VALUE;

/*******************************************************************************
 *
 * TYPE DEFINITIONS
 *
 ******************************************************************************/

struct node;

typedef struct node
{
	const	unsigned int		Type;

			void			*	Item;

	struct	node			* 	Prev;
	struct	node			* 	Next;
} node;

typedef struct
{
	const	unsigned char		Type;

	const	unsigned int 		Size;
	const	unsigned int		Item;

			node			* 	Head;
			node			*	Tail;

			void			**	Bank;
} morph;

typedef enum {NHEAD = 0, NITEM, NTAIL} nodeType;

/*******************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 ******************************************************************************/

static node * create_node (unsigned int type, void * item, node * prev, node * next)
{
	node * u = malloc (sizeof (node));
  
	if (! u)
    {
      return 0;
    }
  
	if (prev)
    {
		prev->Next = u;
    }
  
	if (next)
    {
		next->Prev = u;
    }

	u->Prev = prev;
	u->Item = item;
	u->Next = next;
  
	SET_U32_CONST (u->Type, type);
  
	return u;
}

/*******************************************************************************
 *
 * SINGLE LINKED LIST
 *
 ******************************************************************************/

static unsigned int slist_get_size (morph * m)
{
	return m->Size;
}

static void * slist_create ()
{
	morph * m = malloc (sizeof (morph));

	if (! m)
    {
		return 0;
    }

	m->Head 	= 0;
	m->Tail 	= 0;
	m->Bank 	= 0;

	SET_U08_CONST (m->Type, (unsigned char) M_SINGLE_LINKED_LIST);

	SET_U32_CONST (m->Size, 0);
	SET_U32_CONST (m->Item, 0);

  return m;
}

static void * slist_delete (morph * m, void (* del) (void *))
{
	if (m->Size)
    {
		mclear (m, del);
	}

	free (m);

	return 0;
}

static void * slist_find
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void * (* cpy) (void *))
{
	node * u = 0;

	if (! m->Size)
    {
		return 0;
    }

	for (u = m->Head; u; u = u->Next)
	{
		if (cmp (u->Item, value))
		{
			return cpy ? cpy (u->Item) : u->Item;
		}
	}

	return 0;
}

static void * slist_seek
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void * (* cpy) (void *))
{
	morph 	* n = 0;
	node	* u = 0;

	if (! m->Size)
    {
		return 0;
    }

	if (! (n = (morph *) mcreate (m->Type)))
    {
		return 0;
    }

	if (cpy)
	{
		for (u = m->Head; u; u = u->Next)
	    {
			if (cmp (u->Item, value))
			{
				madd (n, cpy (u->Item));
			}
	    }
	}
	else
	{
		for (u = m->Head; u; u = u->Next)
	    {
			if (cmp (u->Item, value))
			{
				madd (n, u->Item);
			}
	    }
	}

	return n;
}

static void slist_clear (morph * m, void (* del) (void *))
{
	node * u = 0;
	node * v = 0;

	if (! m->Size)
    {
		return;
    }

	if (del)
	{
		for (u = m->Head; u; u = v)
		{
			v = u->Next; del (u->Item); free (u);
		}
	}
	else
	{
		for (u = m->Head; u; u = v)
		{
			v = u->Next; free (u);
		}
	}

	m->Head = 0;
	m->Tail = 0;

	SET_U32_CONST (m->Size, 0);
}

static void slist_batch (morph * m, void (* edit) (void *, void *), void * value)
{
	node * u = 0;

	if (! m->Size)
    {
		return;
    }

	for (u = m->Head; u; u = u->Next)
    {
		edit (u->Item, value);
    }
}

static void slist_insert
(morph * m, void * item, unsigned int index, void * (* cpy) (void *))
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void slist_remove
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void (* del) (void *))
{
	node * u = 0;
	node * v = 0;

	if (! m->Size)
    {
		return;
    }

	while (cmp (m->Head->Item, value))
	{
		u			= m->Head;
		m->Head 	= u->Next;

		if (del)
		{
			del (u->Item);
		}

		free (u);

		SET_U32_CONST (m->Size, m->Size - 1);

		if (! m->Size)
		{
			return;
		}
	}

	if (del)
	{
		for (u = m->Head; u; u = v)
		{
			if (! (v = u->Next))
			{
				return;
			}

			if (! cmp (v->Item, value))
			{
				continue;
			}

			u->Next = v->Next;

			del	(v->Item);

			free (v);

			v = u->Next;

			SET_U32_CONST (m->Size, m->Size - 1);
		}
	}
	else
	{
		for (u = m->Head; u; u = v)
		{
			if (! (v = u->Next))
			{
				return;
			}

			if (! cmp (v->Item, value))
			{
				continue;
			}

			u->Next = v->Next;

			free (v);

			v = u->Next;

			SET_U32_CONST (m->Size, m->Size - 1);
		}
	}
}

static void slist_sort (morph * m, unsigned char (* cmp) (void *, void *))
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void slist_push (morph * m, void * item)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void * slist_pop (morph * m)
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void slist_add (morph * m, void * item)
{
	node* u = 0;

	if (! m->Size)
	{
		if (! (u = create_node (0, item, 0, 0)))
		{
			return;
		}

		m->Head = u;
	}
	else
	{
		for (u = m->Head; u; u = u->Next)
		{
			if (! u->Next)
			{
				if (! (u->Next = create_node (0, item, 0, 0)))
				{
					return;
				}

				break;
			}
		}
	}

	m->Tail = u;

	SET_U32_CONST (m->Size, m->Size + 1);
}

static void slist_sub (morph * m, void (* del) (void *))
{
	node * u = 0;
	node * v = 0;

	if (! m->Size)
    {
		return;
    }

	if (del)
    {
		if (! m->Head->Next)
		{
			del (m->Head->Item);

			free (m->Head);

			m->Head = 0;
			m->Tail = 0;
		}
		else
		{
			for (u = m->Head; u; u = v)
			{
				v = u->Next;

				if (! v->Next)
				{
					del (v->Item);

					free (v);

					u->Next	= 0;
					m->Tail 	= u;

					break;
				}
			}
		}

    }
	else
	{
		if (! m->Head->Next)
		{
			free (m->Head);

			m->Head = 0;
			m->Tail = 0;
		}
		else
		{
			for (u = m->Head; u; u = v)
			{
				v = u->Next;

				if (! v->Next)
				{
					free (v);

					u->Next	= 0;
					m->Tail 	= u;

					break;
				}
			}
		}
	}

  SET_U32_CONST (m->Size, m->Size - 1);
}

static void slist_enqueue (morph * m, void * item)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void* slist_dequeue (morph * m)
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void * slist_get (morph * m, unsigned int index)
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void slist_set (morph * m, void * item, unsigned int index)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void * slist_peek (morph * m, void * (* cpy) (void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	if (! m->Size)
    {
		return 0;
    }

	return cpy ? cpy (m->Head->Item) : m->Head->Item;
#endif
}

static void * slist_keep (morph * m, void * (* cpy)(void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	if (! m->Size)
	{
		return 0;
	}
	return cpy ? cpy (m->Tail->Item) : m->Tail->Item;
#endif
}

static void * slist_predecessor
(morph * m, void * item, unsigned char (* cmp) (void *, void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void * slist_successor (morph * m, void * item, unsigned char (* cmp) (void *, void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static array_s * slist_to_array (morph * m, void * (* cpy) (void *), unsigned int size)
{
	unsigned int		i = 0;

	node				* 	u = 0;

	array_s				* 	a = 0;

	if (! m->Size)
    {
		return 0;
    }

	if (! (a = malloc (sizeof (array_s))))
    {
		return 0;
    }

	if (! (a->data = malloc (size * m->Size)))
    {
		free (a); return 0;
    }

	a->size = m->Size;

	if (cpy)
    {
		for (u = m->Head, i = 0; u; u = u->Next)
		{
			a->data [i ++] = cpy (u->Item);
		}
    }
	else
    {
		for (u = m->Head, i = 0; u; u = u->Next)
		{
			a->data [i ++] = u->Item;
		}
    }

	return a;
}

/*******************************************************************************
 *
 * DOUBLY LINKED LIST
 *
 ******************************************************************************/

static unsigned int dlist_get_size (morph * m)
{
	return m->Size;
}

static void * dlist_create ()
{
	morph	* m = malloc (sizeof (morph));
	node	* u = 0;
	node	* v = 0;

	if (! m)
	{
		return 0;
	}

	u = create_node (NHEAD, 0, 0, 0);
	v = create_node (NTAIL, 0, 0, 0);

	if ((! u) || (! v))
	{
		free (m);

		if (u)
		{
			free (u);
		}

		if (v)
		{
			free (v);
		}

		return 0;
	}

	m->Head = u;
	u->Next = v;
	v->Prev = u;
	m->Tail = v;

	m->Bank = 0;

	SET_U08_CONST (m->Type, (unsigned char) M_DOUBLE_LINKED_LIST);
	SET_U32_CONST (m->Size, 0);
	SET_U32_CONST (m->Item, 0);

	return m;
}

static void* dlist_delete (morph* m, void (* del) (void *))
{
	if (m->Size)
	{
		mclear (m, del);
	}

	if (m->Head)
	{
		free (m->Head);
	}

	if (m->Tail)
	{
		free (m->Tail);
	}

	free (m);

	return 0;
}

static void* dlist_find
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void * (* cpy) (void *))
{
	node * u = 0;

	if (! m->Size)
	{
		return 0;
	}

	if (! cmp)
	{
		return 0;
	}

	for (u = m->Head->Next; u; u = u->Next)
	{
		if (u->Type != NITEM)
		{
			return 0;
		}

		if (cmp (u->Item, value))
		{
			return cpy ? cpy (u->Item) : u->Item;
		}
	}

	return 0;
}

static void* dlist_seek
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void * (* cpy) (void *))
{
	morph	* 	n = 0;
	node	*	u = 0;

	if (! m->Size)
	{
		return 0;
	}

	if (! cmp)
	{
		return 0;
	}

	if (! (n = mcreate (m->Type)))
	{
		return 0;
	}

	if (cpy)
	{
		for (u = m->Head->Next; u; u = u->Next)
		{
			if (u->Type != NITEM)
			{
				break;
			}

			if (cmp (u->Item, value))
			{
				madd (n, cpy (u->Item));
			}
		}
	}
	else
	{
		for (u = m->Head->Next; u; u = u->Next)
		{
			if (u->Type != NITEM)
			{
				break;
			}

			if (cmp (u->Item, value))
			{
				madd (n, u->Item);
			}
		}
	}

	return n;
}

static void dlist_clear (morph * m, void (* del) (void *))
{
	node * u = 0;
	node * v = 0;

	if (! (m->Size))
	{
		return;
	}

	if (del)
	{
		for (u = m->Head->Next; u; u = v)
		{
			v = u->Next;

			if (u->Type != NITEM)
			{
				break;
			}

			del (u->Item);

			free (u);
		}
	}
	else
	{
		for (u = m->Head->Next; u; u = v)
		{
			v = u->Next;

			if (u->Type != NITEM)
			{
				break;
			}

			free (u);
		}
	}

	m->Head->Next = m->Tail;
	m->Tail->Prev = m->Head;

	SET_U32_CONST (m->Size, 0);
}

static void dlist_batch (morph * m, void (* edit) (void *, void *), void * value)
{
	node * u = 0;

	if (! m->Size)
	{
		return;
	}

	for (u = m->Head->Next; u; u = u->Next)
	{
		if (u->Type != NITEM)
		{
			break;
		}

		edit (u->Item, value);
	}
}

static void dlist_insert
(morph * m, void * item, unsigned int index, void * (* cpy) (void *))
{
#ifdef	MORPH_BASIC
	return;
#else
	return;
#endif
}

static void dlist_remove
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void (* del) (void *))
{
	node * u = 0;
	node * v = 0;

	if (! m->Size)
	{
		return;
	}

	if (! cmp)
	{
		return;
	}

	if (del)
	{
		for (u = m->Head->Next; u; u = v)
		{
			if (u->Type != NITEM)
			{
				return;
			}

			if (! (v = u->Next))
			{
				return;
			}

			if (cmp (u->Item, value))
			{
				u->Prev->Next 	= v;
				v->Prev			= u->Prev;

				del (u->Item);

				free (u);

				SET_U32_CONST (m->Size, m->Size - 1);
			}
		}
	}
	else
	{
		for (u = m->Head->Next; u; u = v)
		{
			if (u->Type != NITEM)
			{
				return;
			}

			if (! (v = u->Next))
			{
				return;
			}

			if (cmp (u->Item, value))
			{
				u->Prev->Next 	= v;
				v->Prev			= u->Prev;

				free (u->Item);

				free (u);

				SET_U32_CONST (m->Size, m->Size - 1);
			}
		}
	}
}

static void dlist_sort (morph * m, unsigned char (* cmp)(void *, void *))
{
	void			*	t = 0;
	node			*	u = 0;
	node			*	v = 0;
	unsigned char		n = 0;

	if (! m->Size)
	{
		return;
	}

	if (! cmp)
	{
		return;
	}

	n = 1;

	while (n)
	{
		for (u = m->Head->Next, n = 0; u; u = v)
		{
			if (u->Type != NITEM)
			{
				break;
			}

			if (! (v = u->Next))
			{
				break;
			}

			if (v->Type == NTAIL)
			{
				break;
			}

			if (cmp (u->Item, v->Item))
			{
				t			= u->Item;
				u->Item	= v->Item;
				v->Item	= t;
				n ++;
			}
		}
	}
}

static void dlist_push (morph * m, void * item)
{
#ifdef MORPH_BASIC
	return;
#else
	if (! item)
	{
		return;
	}

	if (create_node (NITEM, item, m->Head, m->Head->Next))
	{
		SET_U32_CONST (m->Size, m->Size + 1)
	}
#endif
}

static void * dlist_pop (morph * m)
{
#ifdef MORPH_BASIC
	return 0;
#else
	node 	* u = 0;
	void		* a = 0;

	if (m->Size == 0)
	{
		return 0;
	}

	if (! (u = m->Head->Next))
	{
		return 0;
	}

	a 					= u->Item;
	m->Head->Next 	= u->Next;
	u->Next->Prev 	= m->Head;


	free (u);

	SET_U32_CONST (m->Size, m->Size - 1);

	return a;
#endif
}

static void dlist_add (morph * m, void * item)
{
	if (! item)
	{
		return;
	}

	if (create_node (NITEM, item, m->Tail->Prev, m->Tail))
    {
		SET_U32_CONST (m->Size, m->Size + 1);
    }
}

static void dlist_sub (morph * m, void (* del) (void *))
{
	node * u = 0;

	if (! m->Size)
	{
		return;
	}

	if (! (u = m->Tail->Prev))
	{
		return;
	}

	if (u->Type != NITEM)
	{
		return;
	}

	m->Tail->Prev = u->Prev;
	u->Prev->Next = m->Tail;

	if (del)
	{
		del (u->Item);
	}

	free (u);

	SET_U32_CONST (m->Size, m->Size - 1);
}

static void dlist_enqueue (morph* m, void* item)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void * dlist_dequeue (morph * m)
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void * dlist_get (morph * m, unsigned int index)
{
#ifdef MORPH_BASIC
	return 0;
#else
	unsigned int 	c = 0;
	node *	u = 0;

	if (! m-> Size)
	{
		return 0;
	}

	for (u = m->Head->Next; u; u = u->Next)
	{
		if (u->Type != NITEM)
		{
			return 0;
		}

		if (c == index)
		{
			return u->Item;
		}

		c ++;
	}

	return 0;
#endif
}

static void dlist_set (morph * m, void * item, unsigned int index)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void * dlist_peek (morph* m, void * (* cpy) (void *))
{
	node * u;

	if (! m->Size)
	{
		return 0;
	}

	if (! (u = m->Head->Next))
	{
		return 0;
	}

	if (u->Type != NITEM)
	{
		return 0;
	}

	return cpy ? cpy (u->Item) : u->Item;
}

static void * dlist_keep (morph * m, void * (* cpy) (void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	node * u;

	if (! m->Size)
	{
		return 0;
	}

	if (! (u = m->Head->Next))
	{
		return 0;
	}

	if (u->Type != NITEM)
	{
		return 0;
	}

	return cpy ? cpy (u-> Item) : u->Item;
#endif
}

static void * dlist_predecessor
(morph * m, void * item, unsigned char (* cmp) (void *, void *))
{
	node* u = 0;
	node* v = 0;

	if (! m->Size)
	{
		return 0;
	}

	if (! cmp)
	{
		return 0;
	}

	for (u = m->Head->Next; u; u = v)
	{
		if (u->Type != NITEM)
		{
			break;
		}

		if (! (v = u->Next))
		{
			break;
		}

		if (v->Type != NITEM)
		{
			break;
		}

		if (cmp (v->Item, item))
		{
			return u->Item;
		}
	}

	return 0;
}

static void * dlist_successor
(morph * m, void * item, unsigned char (* cmp) (void *, void *))
{
	node* u = 0;
	node* v = 0;

	if (! m->Size)
	{
		return 0;
	}

	if (! cmp)
	{
		return 0;
	}

	for (u = m->Tail->Prev; u; u = v)
	{
		if (u->Type != NITEM)
		{
			break;
		}

		if (! (v = u->Prev))
		{
			break;
		}

		if (v->Type != NITEM)
		{
			break;
		}

		if (cmp (v->Item, item))
		{
			return u->Item;
		}
	}

	return 0;
}

static array_s * dlist_to_array (morph * m, void * (* cpy) (void *), unsigned int size)
{
	unsigned int		i = 0;

	node				*	u = 0;
	array_s				*	a = 0;

	if (! m->Size)
	{
		return 0;
	}

	if (! (u = m->Head->Next))
	{
		return 0;
	}

	if (! (a = (array_s *) malloc (sizeof (array_s))))
	{
		return 0;
	}

	if (! (a->data = malloc (size * m->Size)))
	{
		free (a); return 0;
	}

	a->size = m->Size;

	if (cpy)
	{
		for (u = m->Head, i = 0; u; u = u->Next)
		{
			a->data [i ++] = cpy (u->Item);
		}
	}
	else
	{
		for (u = m->Head, i = 0; u; u = u->Next)
		{
			a->data [i ++] = u->Item;
		}
	}

	return a;
}

/*******************************************************************************
 *
 * STACK
 *
 ******************************************************************************/

static unsigned int stack_get_size (morph* m)
{
	return m->Size;
}

static void * stack_create ()
{
	morph * m = (morph*) malloc (sizeof (morph));

	if (! m)
    {
		return 0;
    }

	m->Head = 0;
	m->Tail = 0;
	m->Bank = 0;

	SET_U08_CONST (m->Type, (unsigned char) M_STACK);

	SET_U32_CONST (m->Size, 0);
	SET_U32_CONST (m->Item, 0);

	return m;
}

static void * stack_delete (morph* m, void (* del)(void *))
{
	if (m->Size)
	{
		mclear (m, del);
	}

	m->Head = 0;
	m->Tail = 0;

	free (m);

	return 0;
}

static void * stack_find
(morph * m, void * value, unsigned char (* cmp)(void *, void *), void * (* cpy)(void *))
{
	node * u = 0;

	if (! m->Size)
	{
		return 0;
	}

	for (u = m->Head; u; u = u->Next)
	{
		if (cmp (u->Item, value))
		{
			if (cpy)
			{
				return (cpy (u->Item));
			}
			else
			{
				return u->Item;
			}
		}
	}

	return 0;
}

static void * stack_seek
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void * (* cpy) (void *))
{
	morph*  n = 0;
	node*		u = 0;

	if (! m->Size)
	{
		return 0;
	}

	if (! (n = mcreate (m->Type)))
	{
		return 0;
	}

	for (u = m->Head; u; u = u->Next)
	{
		if (cmp (u->Item, value))
		{
			mpush (n, cpy ? cpy (u->Item) : u->Item);
		}
	}

	return n;
}

static void stack_clear (morph* m, void (* del)(void *))
{
	node* u = 0;
	node* v = 0;

	if (! m->Size)
	{
		return;
	}

	if (! (u = m->Head))
	{
		return;
	}

	if (del)
	{
		for (u = m->Head; u; u = v)
		{
			v = u->Next;

			del (u->Item);

			free (u);

			if (! v)
			{
				break;
			}
		}
	}
	else
	{
		for (u = m->Head; u; u = v)
		{
			v = u->Next;

			free (u);

			if (! v)
			{
				break;
			}
		}
	}

	m->Head = 0;
	m->Tail = 0;

	SET_U32_CONST (m->Size, 0);
}

static void stack_batch (morph * m, void (* edit) (void *, void *), void * value)
{
	node * u = 0;

	if (! edit)
	{
		return;
	}

	if (! m->Size)
	{
		return;
	}

	for (u = m->Head; u; u = u->Next)
	{
		edit (u->Item, value);
	}
}

static void stack_insert
(morph * m, void * item, unsigned int index, void * (* cpy) (void *))
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void stack_remove
(morph * m, void * value, unsigned char (* cmp)(void *, void *), void (* del) (void *))
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void stack_sort (morph * m, unsigned char (* cmp) (void *, void *))
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void stack_push (morph * m, void * item)
{
	node* u = 0;

	if (! (u = create_node (NITEM, item, 0, 0)))
	{
		return;
	}

	u->Item = item;
	u->Next = m->Head;
	m->Head = u;

	SET_U32_CONST (m->Size, m->Size + 1);
}

static void * stack_pop (morph * m)
{
	node	* u = 0;
	void 	* a = 0;

	if (! m->Size)
	{
		return 0;
	}

	u			= m->Head;
	a 			= m->Head->Item;
	m->Head 	= m->Head->Next;

	free (u);

	SET_U32_CONST (m->Size , m->Size - 1);

	if (! m->Size)
	{
		m->Head = 0;
		m->Tail = 0;
	}

	return a;
}

static void stack_add (morph * m, void * item)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void stack_sub (morph * m, void (* del) (void *))
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void stack_enqueue (morph * m, void * item)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void * stack_dequeue (morph * m)
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void * stack_get (morph * m, unsigned int index)
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void stack_set (morph * m, void * item, unsigned int index)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void * stack_peek (morph* m, void * (* cpy)(void *))
{
	if (! m->Size)
    {
		return 0;
    }

	return cpy ? cpy (m->Head->Item) : m->Head->Item;
}

static void * stack_keep (morph * m, void * (* cpy) (void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void * stack_predecessor
(morph * m, void * item, unsigned char (* cmp) (void *, void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void * stack_successor
(morph * m, void * item, unsigned char (* cmp) (void *, void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static array_s * stack_to_array
(morph * m, void * (* cpy) (void *), unsigned int size)
{
#ifdef MORPH_BASIC
	return 0;
#else
	node	*	u = 0;
	unsigned int			i = 0;
	array_s	*	a = 0;

	if (! m->Size)
	{
		return 0;
	}

	if (! (u = m->Head))
	{
		return 0;
	}

	if (! (a = malloc (sizeof (array_s))))
	{
		return 0;
	}

	if (! (a->data = malloc (size * m->Size)))
	{
		free (a); return 0;
	}

	a->size = m->Size;

	if (! cpy)
	{
		for (u = m->Head, i = 0; u; u = u->Next)
		{
			a->data [i ++] = u->Item;
		}
	}
	else
	{
		for (u = m->Head, i = 0; u; u = u->Next)
		{
			a->data [i ++] = u->Item;
		}
	}

	return a;
#endif
}

/*******************************************************************************
 *
 * QUEUE
 *
 ******************************************************************************/

static unsigned int queue_get_size (morph * m)
{
	return m->Size;
}

static void * queue_create ()
{
	morph * m = malloc (sizeof (morph));

	if (! m)
	{
		return 0;
	}

	m->Head = 0;
	m->Tail = 0;
	m->Bank = 0;

	SET_U08_CONST (m->Type, (unsigned char) M_QUEUE);

	SET_U32_CONST (m->Size, 0);
	SET_U32_CONST (m->Item, 0);

	return m;
}

static void * queue_delete (morph * m, void (* del) (void *))
{
	if (m->Size)
	{
		mclear (m, del);
	}

	free (m);

	return 0;
}

static void * queue_find
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void * (* cpy) (void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	node * u = 0;

	if (! m->Size)
	{
		return 0;
	}

	for (u = m->Head; u; u = u->Next)
	{
		if (cmp (u->Item, value))
		{
			if (cpy)
			{
				return cpy (u->Item);
			}
			else
			{
				return u->Item;
			}
		}
	}

	return 0;
#endif
}

static void * queue_seek
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void * (* cpy) (void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	morph*	n = 0;
	node*	u = 0;

	if (! cmp)
	{
		return 0;
	}

	if (! m->Size)
	{
		return 0;
	}

	if (! (n = mcreate (m->Type)))
	{
		return 0;
	}

	if (cpy)
	{
		for (u = m->Head; u; u = u->Next)
		{
			if (cmp (u->item, value))
			{
				menqueue (n, cpy (u->item));
			}
		}
	}
	else
	{
		for (u = m->Head; u; u = u->Next)
		{
			if (cmp (u->item, value))
			{
				menqueue (n, u->Item);
			}
		}
	}

	return n;
#endif
}

static void queue_clear (morph * m, void (* del) (void *))
{
	node* u = 0;
	node* v = 0;

	if (! m->Size)
	{
		return;
	}

	if (del)
	{
		for (u = m->Head; u; u = v)
		{
			if (u->Item)
			{
				del (u->Item);
			}

			v = u->Next;

			free (u);
		}
	}
	else
	{
		for (u = m->Head; u; u = v)
		{
			if (u->Item)
			{
				free (u->Item);
			}

			v = u->Next;

			free (u);
		}
	}

	m->Head = 0;
	m->Tail = 0;

	SET_U32_CONST (m->Size, 0);
}

static void queue_batch (morph * m, void (* edit) (void *, void *), void * value)
{
#ifdef MORPH_BASIC
	node * u = 0;

	if (! edit)
	{
		return;
	}

	if (! m->Size)
	{
		return;
	}

	for (u = m->Head; u; u = u->Next)
	{
		edit (u->Item, value);
	}
#else
	return;
#endif
}

static void queue_insert
(morph * m, void * item, unsigned int index, void * (* cpy)(void *))
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void queue_remove
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void (* del)(void *))
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void queue_sort (morph * m, unsigned char (* cmp)(void *, void *))
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void queue_push (morph * m, void * item)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void * queue_pop (morph * m)
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void queue_add (morph * m, void * item)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void queue_sub (morph * m, void (* del)(void *))
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void queue_enqueue (morph * m, void * item)
{
	node * u = create_node (NITEM, item, 0, 0);

	if (! u)
	{
		return;
	}

	if (! m->Size)
	{
		m->Head = u;
		m->Tail = u;
	}
	else
	{
		u->Prev			= m->Tail;
		m->Tail->Next	= u;
		m->Tail 			= u;
	}

	SET_U32_CONST (m->Size, m->Size + 1);
}

static void * queue_dequeue (morph * m)
{
	node * u = 0;
	void * a = 0;

	if (! m->Size)
	{
		return 0;
	}

	u = m->Head;
	a = u->Item;

	if (u == m->Tail)
	{
		m->Head 			= 0;
		m->Tail 			= 0;
	}
	else
	{
		m->Head			= u->Next;
		m->Head->Prev	= 0;
	}

	SET_U32_CONST (m->Size, m->Size - 1);

	free (u);

	return a;
}

static void * queue_get (morph * m, unsigned int index)
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void queue_set (morph * m, void * item, unsigned int index)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void * queue_peek (morph * m, void * (* cpy)(void *))
{
#ifdef MORPH_BASIC
	if (! m->Size)
	{
		return 0;
	}

	return cpy ? cpy (m->Head->Item) : m->Head->Item;
#else
	return 0;
#endif
}

static void * queue_keep (morph * m, void * (* cpy)(void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	if (! m->Size)
	{
		return 0;
	}

	return cpy ? cpy (m->Tail->Item) : m->Tail->Item;
#endif
}

static void * queue_predecessor
(morph * m, void * item, unsigned char (* cmp) (void *, void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void * queue_successor
(morph * m, void * item, unsigned char (* cmp) (void *, void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static array_s * queue_to_array
(morph * m, void * (* cpy)(void *), unsigned int size)
{
#ifdef MORPH_BASIC
	return 0;
#else
	node*		u = 0;
	unsigned int			i = 0;
	array_s	*	a = 0;

	if (! m->Size)
	{
		return 0;
	}

	if (! (a = malloc (sizeof (array_s))))
	{
		return 0;
	}

	if (! (a->data = malloc (size * m->Size)))
	{
		free (a); return 0;
	}

	a->size = m->Size;

	if (! cpy)
	{
		for (u = m->Head, i = 0; u; u = u->Next)
		{
			a->data [i ++] = u->Item;
		}
	}
	else
	{
		for (u = m->Head, i = 0; u; u = u->Next)
		{
			a->data [i ++] = u->Item;
		}
	}

	return a;
#endif
}

/*******************************************************************************
 *
 * ARRAY
 *
 ******************************************************************************/

static unsigned int array_get_size (morph * m)
{
	return m->Size;
}

static void * array_create ()
{
	morph * m = (morph*)malloc (sizeof (morph));
	int		i = 0;

	if (! m)
	{
		return 0;
	}

	SET_U08_CONST (m->Type, (unsigned char) M_ARRAY);

	SET_U32_CONST (m->Size, 4);
	SET_U32_CONST (m->Item, -1);

	m->Head = 0;
	m->Tail = 0;

	if (! (m->Bank = calloc (m->Size, sizeof (void *))))
	{
		free (m);

		return 0;
	}

	for (i = 0; i < m->Size; i++)
	{
		m->Bank [i] = 0;
	}

	return m;
}

static void * array_delete (morph * m, void (* del) (void *))
{
	if (msize (m))
	{
		mclear (m, del);
	}

	free (m->Bank);
	free (m);

	return 0;
}

static void * array_find
(morph * m, void * value, unsigned char (* cmp)(void *, void *), void * (* cpy) (void *))
{
	unsigned int i = 0;
	unsigned int j = 0;

	if (m->Item < 0)
	{
		return 0;
	}

	if ((j = m->Item + 1) > m->Size)
	{
		return 0;
	}

	for (i = 0; i < j; i++)
	{
		if (cmp (m->Bank[i], value))
		{
			return cpy ? cpy (m->Bank [i]) : m->Bank [i];
		}
	}

	return 0;
}

static void * array_seek
(morph * m, void * value, unsigned char (* cmp)(void *, void *), void * (* cpy)(void *))
{
	morph				*	n = 0;
	unsigned int		i = 0;
	unsigned int		j = 0;

	if (m->Item < 0)
	{
		return 0;
	}

	if (! (n = mcreate (m->Type)))
	{
		return 0;
	}

	if ((j = m->Item + 1) > m->Size)
	{
		return 0;
	}

	for (i = 0; i < j; i++)
	{
		if (cmp (m->Bank [i], value))
		{
			madd (n, cpy ? cpy (m->Bank [i]) : m->Bank [i]);
		}
	}

	return n;
}

static void array_clear
(morph * m, void (* del) (void *))
{
	unsigned int i = 0;

	if (m->Item < 0)
	{
		return;
	}

	if (del)
	{
		for (i = 0; i < m->Size; i++)
		{
			if (m->Bank [i])
			{
				del (m->Bank [i]);

				m->Bank [i] = 0;
			}
		}
	}
	else
	{
		for (i = 0; i < m->Size; i++)
		{
			if (m->Bank [i])
			{
				m->Bank [i] = 0;
			}
		}
	}

	SET_U32_CONST (m->Item, -1);
}

static void array_batch
(morph * m, void (* edit) (void *, void *), void * value)
{
	unsigned int	i = 0;
	unsigned int	j = 0;

	if (! m->Bank)
	{
		return;
	}

	if (m->Item < 0)
	{
		return;
	}

	if ((j = m->Item + 1) > m->Size)
	{
		return;
	}

	for (i = 0; i < j; i++)
	{
		if (m->Bank [i])
		{
			edit (m->Bank [i], value);
		}
	}
}

static void array_insert
(morph * m, void * item, unsigned int index, void * (* cpy) (void *))
{
	unsigned int 		i = 0;
	unsigned int		j = 0;

	void				*	a = 0;
	void 				*	b = 0;

	unsigned int		s = 0;

	if (m->Item < 0)
	{
		return;
	}

	if (index >= m->Size)
	{
		return;
	}

	if ((j = m->Item + 1) >= m->Size)
	{
		s = m->Size * 2;

		if (! (m->Bank = realloc (m->Bank, (s * sizeof (void **)))))
		{
			return;
		}

		if (cpy)
		{
			a 						= m->Bank [index - 1];
			m->Bank [index - 1] 	= cpy (item);
		}
		else
		{
			a 						= m->Bank [index - 1];
			m->Bank [index - 1] 	= item;

		}

		for (i = index; i < j; i++)
		{
			b 				= m->Bank [i + 1];
			m->Bank [i]	= a;
			a				= b;
		}

		SET_U32_CONST (m->Item, m->Item + 1);
		SET_U32_CONST (m->Size, s);
	}
	else
	{
		if (cpy)
		{
			a 						= m->Bank [index - 1];
			m->Bank [index - 1] 	= cpy (item);
		}
		else
		{
			a 						= m->Bank [index - 1];
			m->Bank [index - 1] 	= item;

		}

		for (i = index; i < j; i++)
		{
			b 				= m->Bank [i + 1];
			m->Bank [i]	= a;
			a				= b;
		}

		SET_U32_CONST (m->Item, m->Item + 1);
	}
}

static void array_remove
(morph * m, void * value, unsigned char (* cmp) (void *, void *), void (* del) (void *))
{
	unsigned int i = 0;
	unsigned int j = 0;

	if (m->Item < 0)
	{
		return;
	}

	if ((j = m->Item + 1) > m->Size)
	{
		return;
	}

	if (del)
	{
		for (i = 0; i <= j; i++)
		{
			if (cmp (m->Bank [i], value))
			{
				del (m->Bank [i]);
				m->Bank [i]	= 0;

				SET_U32_CONST (m->Item, m->Item - 1);
			}
		}
	}
	else
	{
		for (i = 0; i <= j; i++)
		{
			if (cmp (m->Bank [i], value))
			{
				m->Bank [i] = 0;

				SET_U32_CONST (m->Item, m->Item - 1);
			}
		}
	}
}

static void array_sort (morph * m, unsigned char (* cmp)(void *, void *))
{
	unsigned int		i = 0;
	unsigned int		j = 0;
	unsigned char			s = 0;
	void				*	u = 0;

	if (m->Item < 0)
	{
		return;
	}

	j = m->Item + 1;

	while (! s)
	{
		s = 0;

		for (i = 0; i < j; i++)
		{
			if (cmp (m->Bank [i], m->Bank [i + 1]))
			{
				u 				  = m->Bank [i    ];
				m->Bank [i    ] = m->Bank [i + 1];
				m->Bank [i + 1] = u;

				s ++;
			}
		}
	}
}

static void array_push (morph* m, void* item)
{
#ifndef MORPH_BASIC
	return;
#endif
}

static void * array_pop (morph * m)
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void array_add (morph * m, void * item)
{
	unsigned int		i = 0;
	unsigned int 		s = 0;

	for (i = 0; i < m->Size; i++)
	{
		if (! m->Bank [i])
		{
			m->Bank [i] = item; return;
		}
	}

	s = m->Size;

	if (! (m->Bank = realloc (m->Bank, sizeof (void *) * (m->Size * 2))))
	{
		return;
	}

	SET_U32_CONST (m->Size, m->Size * 2);

	for (i = s + 1; i < m->Size; i++)
	{
		m->Bank [i] = 0;
	}

	m->Bank [s] = item;
}

static void array_sub (morph * m, void (* del) (void *))
{
	if (m->Item < 0)
	{
		return;
	}

	if (! m->Bank [m->Item])
	{
		return;
	}

	if (del)
	{
		del (m->Bank [m->Item]);
	}

	m->Bank [m->Item] = 0;

	SET_U32_CONST (m->Item, m->Item - 1);
}

static void array_enqueue (morph * m, void * item)
{
#ifdef MORPH_BASIC
	return;
#else
	return;
#endif
}

static void * array_dequeue (morph * m)
{
#ifdef MORPH_BASIC
	return 0;
#else
	return 0;
#endif
}

static void * array_get (morph * m, unsigned int index)
{
	if (index >= m->Size)
	{
		return 0;
	}

	return m->Bank [index];
}

static void array_set (morph * m, void * item, unsigned int index)
{
	if (! item)
	{
		return;
	}

	if (index >= m->Size)
	{
		return;
	}

	m->Bank [index] = item;
}

static void * array_peek (morph * m, void * (* cpy)(void *))
{
#ifdef	MORPH_BASIC
	return 0;
#else
	if (m->Item < 0)
	{
		return 0;
	}

	if (! m->Bank [0])
	{
		return 0;
	}

	return cpy ? cpy (m->Bank [0]) : m->Bank [0];
#endif
}

static void * array_keep (morph* m, void * (* cpy)(void *))
{
#ifdef	MORPH_BASIC
	return 0;
#else
	if (m->Item < 0)
	{
		return 0;
	}

	if (! m->Bank [m->Item])
	{
		return 0;
	}

	return cpy ? cpy (m->Bank [m->Item]) : m->Bank [m->Item];
#endif
}

static void* array_predecessor
(morph * m, void * item, unsigned char (* cmp) (void *, void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	unsigned int i = 0;

	if (! cmp)
	{
		return 0;
	}

	if (m->Item < 1)
	{
		return 0;
	}

	for (i = 1; i < m->Item; i++)
	{
		if (cmp (m->Bank [i], item))
		{
			return m->Bank [i - 1];
		}
	}

	return 0;
#endif
}

static void * array_successor
(morph * m, void * item, unsigned char (* cmp) (void *, void *))
{
#ifdef MORPH_BASIC
	return 0;
#else
	unsigned int i = 0;
	unsigned int j = 0;

	if (! cmp)
	{
		return 0;
	}

	if (m->Item < 1)
	{
		return 0;
	}

	j = m->Item - 1;

	for (i = 0; i < j; i++)
	{
		if (cmp (m->Bank [i], item))
		{
			return m->Bank [i + 1];
		}
	}

	return 0;
#endif
}

static array_s * array_to_array (morph* m, void * (* cpy)(void *), unsigned int size)
{
	unsigned int		i = 0;
	unsigned int		j = 0;

	array_s				*	a = 0;

	if (m->Item < 0)
	{
		return 0;
	}

	if (! (a = (array_s *) malloc (sizeof (array_s))))
	{
		return 0;
	}

	a->size 	= m->Size;
	j			= m->Item + 1;

	if (! (a->data = malloc (j * size)))
	{
		free (a); return 0;
	}

	if (cpy)
	{
		for (i = 0; i < j; i++)
		{
			a->data [i] = cpy (m->Bank [i]);
		}
	}
	else
	{
		for (i = 0; i < j; i++)
		{
			a->data [i] = m->Bank [i];
		}
	}

	return a;
}

/*******************************************************************************
 *
 * INTERFACE
 *
 ******************************************************************************/

static void * (* f_create [LIST_TYPE_COUNT]) () =
{
	slist_create,
	dlist_create,
	stack_create,
	queue_create,
	array_create
};

void * mcreate (morphtype type)
{
	if (type > LIST_TYPE_COUNT)
	{
		return 0;
	}

	return f_create [type] ();
}

static void * (* f_delete [LIST_TYPE_COUNT]) (morph *, void (*) (void *)) =
{
	slist_delete,
	dlist_delete,
	stack_delete,
	queue_delete,
	array_delete
};

void * mdelete (void * list, void (* del) (void *))
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	return f_delete [m->Type] (m, del);
}

static void * (* f_find [LIST_TYPE_COUNT])
(morph *, void *, unsigned char (*) (void *, void *), void * (*)(void *)) =
{
	slist_find,
	dlist_find,
	stack_find,
	queue_find,
	array_find
};

void * mfind
(void * list, void * value, unsigned char (* cmp) (void *, void *), void * (* cpy) (void *))
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	if (! cmp)
	{
		return 0;
	}

	return f_find [m->Type] (m, value, cmp, cpy);
}

static void * (* f_seek [LIST_TYPE_COUNT])
(morph *, void *, unsigned char (* cmp) (void *, void *), void * (*) (void *)) =
{
	slist_seek,
	dlist_seek,
	stack_seek,
	queue_seek,
	array_seek
};

void * mseek
(void * list, void * value, unsigned char (* cmp) (void *, void *), void * (* cpy) (void *))
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	if (! cmp)
	{
		return 0;
	}

	return f_seek [m->Type] (m, value, cmp, cpy);
}

static void (* f_clear [LIST_TYPE_COUNT]) (morph *, void (*) (void *)) =
{
	slist_clear,
	dlist_clear,
	stack_clear,
	queue_clear,
	array_clear
};

void mclear (void * list, void (* del) (void *))
{
	morph * m = list;

	if (! m)
	{
		return;
	}

	f_clear [m->Type] (m, del);
}

static void (* f_batch [LIST_TYPE_COUNT]) (morph *, void (*)(void *, void *), void*) =
{
	slist_batch,
	dlist_batch,
	stack_batch,
	queue_batch,
	array_batch
};

void mbatch (void * list, void (* edit)(void *, void *), void * value)
{
	morph * m = list;

	if (! m)
	{
		return;
	}

	if (! edit)
	{
		return;
	}

	f_batch [m->Type] (m, edit, value);
}

static void (* f_insert [LIST_TYPE_COUNT])
(morph *, void *, unsigned int, void * (*) (void *)) =
{
	slist_insert,
	dlist_insert,
	stack_insert,
	queue_insert,
	array_insert
};

void minsert (void * list, void * item, unsigned int index, void * (* cpy) (void *))
{
	morph * m = list;

	if (! m)
	{
		return;
	}

	f_insert [m->Type] (m, item, index, cpy);
}

static void (* f_remove [LIST_TYPE_COUNT])
(morph *, void *, unsigned char (*) (void *, void *), void (*)(void *)) =
{
	slist_remove,
	dlist_remove,
	stack_remove,
	queue_remove,
	array_remove
};

void mremove
(void * list, void * value, unsigned char (* cmp) (void *, void *), void (* del)(void *))
{
	morph * m = list;

	if (! m)
	{
		return;
	}

	if (! cmp)
	{
		return;
	}

	f_remove [m->Type] (m, value, cmp, del);
}

static void (* f_sort [LIST_TYPE_COUNT])
(morph *, unsigned char (* cmp) (void *, void *)) =
{
	slist_sort,
	dlist_sort,
	stack_sort,
	queue_sort,
	array_sort
};

void msort (void * list, unsigned char (* cmp) (void *, void *))
{
	morph * m = list;

	if (! m)
	{
		return;
	}

	if (! cmp)
	{
		return;
	}

	f_sort [m->Type] (m, cmp);
}

static void (* f_push [LIST_TYPE_COUNT]) (morph *, void *) =
{
	slist_push,
	dlist_push,
	stack_push,
	queue_push,
	array_push
};

void mpush (void * list, void * item)
{
	morph * m = list;

	if (! m)
	{
		return;
	}

	f_push [m->Type] (m, item);
}

static void * (* f_pop [LIST_TYPE_COUNT]) (morph *) =
{
	slist_pop,
	dlist_pop,
	stack_pop,
	queue_pop,
	array_pop
};

void * mpop (void * list)
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	return f_pop [m->Type] (m);
}

static void (* f_add [LIST_TYPE_COUNT]) (morph *, void *) =
{
	slist_add,
	dlist_add,
	stack_add,
	queue_add,
	array_add
};

void madd (void * list, void * item)
{
	morph * m = list;

	if (! m)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_add [m->Type] (m, item);
}

static void (* f_sub [LIST_TYPE_COUNT]) (morph *, void (*) (void *)) =
{
	slist_sub,
	dlist_sub,
	stack_sub,
	queue_sub,
	array_sub
};

void msub (void * list, void (* del) (void *))
{
	morph * m = list;

	if (! m)
	{
		return;
	}

	f_sub [m->Type] ((morph*)list, del);
}

static void (* f_enqueue [LIST_TYPE_COUNT]) (morph *, void *) =
{
	slist_enqueue,
	dlist_enqueue,
	stack_enqueue,
	queue_enqueue,
	array_enqueue
};

void menqueue (void * list, void * item)
{
	morph * m = list;

	if (! m)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_enqueue [m->Type] (m, item);
}

static void* (* f_dequeue [LIST_TYPE_COUNT]) (morph *) =
{
	slist_dequeue,
	dlist_dequeue,
	stack_dequeue,
	queue_dequeue,
	array_dequeue
};

void * mdequeue (void * list)
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	return f_dequeue [m->Type] (m);
}

static void * (* f_peek [LIST_TYPE_COUNT]) (morph *, void * (*) (void *)) =
{
	slist_peek,
	dlist_peek,
	stack_peek,
	queue_peek,
	array_peek
};

void * mpeek (void* list, void * (* cpy) (void *))
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	return f_peek [m->Type] (m, cpy);
}

static void * (* f_keep [LIST_TYPE_COUNT]) (morph *, void * (*)(void *)) =
{
	slist_keep,
	dlist_keep,
	stack_keep,
	queue_keep,
	array_keep
};

void * mkeep (void * list, void * (* cpy)(void *))
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	return f_keep [m->Type] (m, cpy);
}

static void * (* f_get [LIST_TYPE_COUNT]) (morph *, unsigned int) =
{
	slist_get,
	dlist_get,
	stack_get,
	queue_get,
	array_get
};

void * mget (void * list, unsigned int index)
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	return f_get [m->Type] (m, index);
}

static void (* f_set [LIST_TYPE_COUNT]) (morph *, void *, unsigned int) =
{
	slist_set,
	dlist_set,
	stack_set,
	queue_set,
	array_set
};

void mset (void * list, void * item, unsigned int index)
{
	morph * m = list;

	if (! m)
	{
		return;
	}

	f_set [m->Type] (m, item, index);
}

static void * (* f_predecessor [LIST_TYPE_COUNT])
(morph *, void *, unsigned char (*) (void *, void *)) =
{
	slist_predecessor,
	dlist_predecessor,
	stack_predecessor,
	queue_predecessor,
	array_predecessor
};

void * mpredecessor (void * list, void* item, unsigned char (* cmp) (void *, void *))
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	if (! cmp)
	{
		return 0;
	}

	return f_predecessor [m->Type] (m, item, cmp);
}

static void* (* f_successor [LIST_TYPE_COUNT])
(morph *, void *, unsigned char (*) (void *, void *)) =
{
	slist_successor,
	dlist_successor,
	stack_successor,
	queue_successor,
	array_successor
};

void * msuccessor (void * list, void * item, unsigned char (* cmp) (void *, void *))
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	if (! cmp)
	{
		return 0;
	}

	return f_successor [m->Type] (m, item, cmp);
}
static array_s * (* f_array [LIST_TYPE_COUNT])
(morph *, void * (*)(void *), unsigned int) =
{
	slist_to_array,
	dlist_to_array,
	stack_to_array,
	queue_to_array,
	array_to_array,
};

array_s * mtoarray (void * list, void * (* cpy) (void *), unsigned int size)
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	return f_array [m->Type] (m, cpy, size);
}

static unsigned int (* f_size [LIST_TYPE_COUNT]) (morph *) =
{
	slist_get_size,
	dlist_get_size,
	stack_get_size,
	queue_get_size,
	array_get_size
};

unsigned int msize (void * list)
{
	morph * m = list;

	if (! m)
	{
		return 0;
	}

	return f_size [m->Type] (m);
}

morphtype mtype (void * list)
{
	return list ? ((morph *) list)->Type : 0XFF;
}

unsigned int mhash (const char * text)
{
	unsigned int 		hash	= 5381;
	char				symbol	= 0;
	char			*	source	= (char*) text;

	if (! source)
	{
		return 0;
	}

	while (*source)
	{
		hash = ((hash << 5) + hash) + (symbol = *(source++));
	}

	return hash;
}
