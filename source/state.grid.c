#include "type.h"
#include "entity.h"
#include "texture.h"
#include "morph.h"
#include "state.h"
#include "config.h"

#include "state.grid.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

#define STATE_GRID_CREATE_ARG_COUNT			2
#define STATE_GRID_CREATE_ARG_POS_SCRIPT	2

wanderlust_state * state_grid_create (lua_State * L)
{
	wanderlust_state * s = e_configuration.null;

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < STATE_GRID_CREATE_ARG_COUNT)
	{
		return 0;
	}

	if (! (s = SDL_malloc (sizeof (wanderlust_state))))
	{
		return 0;
	}

	s->lua			= e_configuration.null;
	s->background 	= e_configuration.null;
	s->entities		= e_configuration.null;

	if (! (s -> entities = mcreate (e_configuration.morph.array)))
	{
		SDL_free (s); return 0;
	}

	if (! (s -> lua = lua_open ()))
	{
		goto FAILURE;
	}

	if (luaL_loadfile (s->lua, lua_tostring (L, STATE_GRID_CREATE_ARG_POS_SCRIPT)))
	{
		goto FAILURE;
	}

	SET_U08_CONST (s->type, 	GRID_STATE);
	SET_U08_CONST (s->locked,	0xFF);

	lua_settop (L, 0);

	return s;

	FAILURE:

		if (s)
		{
			state_grid_delete (s);
		}

		lua_settop (L, 0);

		return e_configuration.null;
}

nil state_grid_delete (wanderlust_state * s)
{
	if (! s)
	{
		return;
	}

	if (s -> entities)
	{
		s -> entities = mdelete (s -> entities, entity_delete);
	}

	if (s->lua)
	{
		lua_close (s->lua);
	}

	SDL_free (s);
}

nil state_grid_direct (wanderlust_state * s) { }

nil state_grid_handle (wanderlust_state * s) { }

nil state_grid_update (wanderlust_state * s) { }

nil state_grid_render (wanderlust_state * s) { }

nil state_grid_adjust (wanderlust_state * s) { }
