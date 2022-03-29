#include "type.h"
#include "config.h"
#include "task.h"
#include "morph.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* *****************************************************************************
 *
 * TYPEDEFINITIONS
 *
 * ****************************************************************************/

typedef struct
{
	u08 	text;
	nil	*	data;
}
wanderlust_task;

/* *****************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ****************************************************************************/

static s32		f_initialized 	= 0;

static nil	*	f_tasks			= 0;

/* *****************************************************************************
 *
 * BANK
 *
 * ****************************************************************************/

static nil NOP (nil * item)
{

}

#define CODE_BASE_EXTEND 0x100

static nil (* code_base [CODE_BASE_EXTEND]) (nil *) =
{
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP
};

/* *****************************************************************************
 *
 * INTERFACE
 *
 * ****************************************************************************/

s08 task_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized 	= e_configuration.success;
	f_tasks			= 0;

	if (! L)
	{
		goto FAILURE;
	}

	lua_register (L, WL_RUNDOWN_TASK, task_rundown);

	if (! (f_tasks = mcreate (e_configuration.morph.queue)))
	{
		goto FAILURE;
	}

	return e_configuration.success;

	FAILURE:

		task_terminate ();

		return e_configuration.failure;
}

nil task_terminate ()
{
	u16 i = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (f_tasks)
	{
		f_tasks = mdelete (f_tasks, SDL_free);
	}

	for (i = 0; i < CODE_BASE_EXTEND; i++)
	{
		code_base [i] = NOP;
	}

	f_initialized = e_configuration.failure;
}

u32 task_pending ()
{
	return msize (f_tasks);
}

nil task_discard ()
{
	mclear (f_tasks, SDL_free);
}

nil task_dismiss (u08 code)
{
	if (code_base [code] == NOP)
	{
		return;
	}

	code_base [code] = NOP;
}

nil task_execute ()
{
	wanderlust_task * u = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! msize (f_tasks))
	{
		return;
	}

	if (! (u = mdequeue (f_tasks)))
	{
		return;
	}

	code_base [u -> text] (u -> data),

	SDL_free (u);
}

s32 task_rundown (lua_State * L)
{
	wanderlust_task * u = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	while (msize (f_tasks))
	{
		if ((u = mdequeue (f_tasks)))
		{
			code_base [u -> text] (u -> data);

			SDL_free (u);
		}
	}

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

nil task_register (u08 code, nil (* instruction)(nil *))
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! instruction)
	{
		return;
	}

	code_base [code] = instruction;
}

nil task_enqueue (u08 text, nil * data)
{
	wanderlust_task * u = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! (u = SDL_malloc (sizeof (wanderlust_task))))
	{
		return;
	}

	u -> text = text;
	u -> data = data;

	menqueue (f_tasks, u);
}
