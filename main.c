#include <wanderlust/type.h>
#include <wanderlust/engine.h>
#incldue <core.h>

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ************************************************************************/

static nil shutdown_procedure ()
{
	engine_terminate ();

	if (task_pending ())
	{
		task_rundown (0);
	}

	task_terminate ();

	SDL_Quit ();
}

/* ************************************************************************
 *
 * MAIN
 *
 * ************************************************************************/

s32 main (int argc, char ** argv)
{
	if (argc < 2)
	{
		return WANDERLUST_FAILURE;
	}

	if (SDL_Init (SDL_INIT_EVERYTHING) < 0)
	{
		shutdown_procedure ();

		return WANDERLUST_FAILURE;
	}

	if (engine_bootstrap (argv [1]) == WANDERLUST_FAILURE)
	{
		shutdown_procedure ();

		return WANDERLUST_FAILURE;
	}

	if (engine_initialize (argv [1]) == WANDERLUST_FAILURE)
	{
		shutdown_procedure ();

		return WANDERLUST_FAILURE;
	}

	if (!core_init ())
	{
		printf ("core ready\n");
	}

	if (engine_execute () == WANDERLUST_FAILURE)
	{
		shutdown_procedure ();

		return WANDERLUST_FAILURE;
	}

	if (!core_quit ())
	{
		printf ("core terminated\n");
	}

	shutdown_procedure ();

	return WANDERLUST_SUCCESS;
}
