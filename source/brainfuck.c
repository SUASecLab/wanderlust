#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "brainfuck.h"

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ************************************************************************/

static void instruction_pointer_inc (context * c)
{
	++ c -> reg.d;
}

static void instruction_pointer_dec (context * c)
{
	-- c -> reg.d;
}

static void instruction_memory_inc (context * c)
{
	++ c -> mem.d [c -> reg.d];
}

static void instruction_memory_dec (context * c)
{
	-- c -> mem.d [c -> reg.d];
}

static void instruction_memory_in (context * c)
{
	scanf ("%d", &c -> mem.d [c -> reg.d]);
}

static void instruction_memory_out (context * c)
{
	printf ("%c", c -> mem.d [c -> reg.d]);	
}

static void instruction_loop_start (context * c)
{
	c -> mem.s [++ c -> reg.s] = c -> reg.p;
}

static void instruction_loop_end (context * c)
{
	if (c -> mem.d [c -> reg.d])
	{
		c -> reg.p = c -> mem.s [c -> reg.s];
	}
	else
	{
		-- c -> reg.s;
	}
}

static void instruction_stack_push (context * c)
{
	c -> mem.s [++ c -> reg.s] = c -> mem.d [c -> reg.d];
}

static void instruction_stack_pop (context * c)
{
	c -> mem.d [c -> reg.d] = c -> mem.s [c -> reg.s --];
}

static void instruction_stack_peek (context * c)
{
	c -> mem.d [c -> reg.d] = c -> mem.s [c -> reg.s];
}

static void instruction_undefined (context * c)
{

}

static void instruction_program_halt (context * c)
{
	c -> reg.f |= F_HALT;
}

static void instruction_program_fail (context * c)
{
	c -> reg.f |= F_ERROR;
}

static void instruction_program_status (context * c)
{
	context_examine (c);
}

static void (* f_table [])(context *) = {
	instruction_pointer_inc,
	instruction_pointer_dec,
	instruction_memory_inc,
	instruction_memory_dec,
	instruction_memory_in,
	instruction_memory_out,
	instruction_loop_start,
	instruction_loop_end,
	instruction_stack_push,
	instruction_stack_pop,
	instruction_stack_peek,
	instruction_undefined,
	instruction_program_halt,
	instruction_program_fail,
	instruction_program_status
};

context * context_create ()
{
	context * c = malloc (sizeof (context));

	if (c == 0)
	{
		return c;
	}

	if (! (c -> mem.p = calloc (sizeof (unsigned short) * 1000, sizeof (unsigned short) * 1000)))
	{
		return context_delete (c);
	}

	if (! (c -> mem.s = calloc (sizeof (unsigned short) * 1000, sizeof (unsigned short) * 1000)))
	{
		return context_delete (c);
	}

	if (! (c -> mem.d = calloc (sizeof (unsigned short) * 1000, sizeof (unsigned short) * 1000)))
	{
		return context_delete (c);
	}

	c -> reg.p = 0x0000;
	c -> reg.s = 0xFFFF;
	c -> reg.d = 0x0000;
	c -> reg.f = 0x0000;

	return c;
}

context * context_delete (context * c)
{
	if (! c)
	{
		return c;		
	}

	if (c -> mem.p)
	{
		free (c -> mem.p);
	}

	if (c -> mem.s)
	{
		free (c -> mem.s);
	}

	if (c -> mem.d)
	{
		free (c -> mem.d);
	}

	free (c);

	return 0;
}

void context_analyze (context * c, char * p, int (*f)(int))
{
	int i, l = strlen (p);

	if (! (l < 0x10000))
	{
		c -> reg.f |= F_OVERFLOW_P;

		return;
	}

	c -> mem.p [l]	= MACHINE_PROGRAM_HALT;

	for (i = 0; i < l; i++)
	{
		c -> mem.p [i] = f (p [i]);
	}
}

void context_examine (context * c)
{
	printf ("reg # f: %02X\n",
		c -> reg.f);
	printf ("reg # p: %04X | s: %04X | d: %04X\n", 
		c -> reg.p, 
		c -> reg.s, 
		c -> reg.d);
	printf ("mem # p: %04X | s: %04X | d: %04X\n---\n", 
		c -> mem.p [c -> reg.p], 
		c -> mem.s [c -> reg.s], 
		c -> mem.d [c -> reg.d]);
}

void context_execute (context * c)
{
	while (! (c -> reg.f & F_HALT))
	{
		f_table [c -> mem.p [c -> reg.p ++]] (c);
	}
}
