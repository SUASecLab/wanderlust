#include "type.h"
#include "config.h"
#include "morph.h"
#include "task.h"
#include "gamepad.h"
#include "input.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * TYPEDEFINITIONS
 *
 * ***********************************************************************/

typedef struct
{
	const	u16						id;

			u16						input;

			s32						x;
			s32						y;

			SDL_GameController	*	gamepad;
			nil					*	buttons;
} wanderlust_gamepad;

typedef struct
{
	const 	u16	id;
			u08 pressed;
			u08 buffer;
} wanderlust_button;

/* ************************************************************************
 *
 * COMPARATOR
 *
 * ***********************************************************************/

static u08 compare_wanderlust_gamepad_by_id (nil * item, nil * value)
{
	if (! item)
	{
		return 0;
	}

	if (! value)
	{
		return 0;
	}

	return ((wanderlust_gamepad *)item) -> id == *((u16 *) value);
}

static u08 compare_wanderlust_button_by_id (nil * item, nil * value)
{
	if (! item)
	{
		return 0;
	}

	if (! value)
	{
		return 0;
	}

	return ((wanderlust_button *) item) -> id == * ((u16 *) value);
}

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

static s08						f_initialized	= WANDERLUST_FAILURE;

static nil					*	f_gamepads		= 0;

static wanderlust_gamepad	*	f_gamepad		= 0;

static u16						f_idcounter		= 0;

static s16						f_deadzone_p	= +0x4000;

static s16						f_deadzone_n	= -0x4000;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ***********************************************************************/

static wanderlust_button * create_wanderlust_button (u16 id)
{
	wanderlust_button * b = SDL_malloc (sizeof (wanderlust_button));

	if (! b)
	{
		return 0;
	}

	SET_U16_CONST (b -> id, id);

	b -> pressed 	= 0;
	b -> buffer		= 0;

	return b;
}

static wanderlust_gamepad * create_wanderlust_gamepad (SDL_GameController * c)
{
	wanderlust_gamepad * gamepad = e_configuration.null;

	if (! c)
	{
		goto FAILURE;
	}

	if (! (gamepad = SDL_malloc (sizeof (wanderlust_gamepad))))
	{
		goto FAILURE;
	}

	if (! (gamepad->buttons = mcreate (e_configuration.morph.singlelinkedlist)))
	{
		goto FAILURE;
	}

	gamepad -> gamepad	= c;
	gamepad -> x		= 0;
	gamepad -> y		= 0;

	SET_U16_CONST (gamepad->id, f_idcounter ++);

	return gamepad;

	FAILURE:

		if (gamepad)
		{
			SDL_free (gamepad);
		}

		return e_configuration.null;
}

static nil delete_wanderlust_gamepad (nil * item)
{
	wanderlust_gamepad * gamepad = item;

	if (! gamepad)
	{
		return;
	}

	if (gamepad -> gamepad)
	{
		SDL_GameControllerClose (gamepad -> gamepad);
	}

	if (gamepad -> buttons)
	{
		gamepad -> buttons = mdelete (gamepad -> buttons, SDL_free);
	}

	SDL_free (gamepad);
}

static nil rebuild_gamepad_list (nil * item)
{
	s32						c 		= 0;
	u16 					g 		= 0;
	wanderlust_gamepad	*	p 		= 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (msize (f_gamepads))
	{
		mclear (f_gamepads, delete_wanderlust_gamepad);
	}

	f_gamepad 	= 0;
	f_idcounter	= 0;

	if (! SDL_NumJoysticks ())
	{
		return;
	}

	for (c = 0; c < SDL_NumJoysticks (); c ++)
	{
		if ((p = create_wanderlust_gamepad (SDL_GameControllerOpen (c))))
		{
			madd (f_gamepads, p); f_gamepad = p;
		}
	}

	if (msize (f_gamepads))
	{
		f_gamepad = mfind
		(
			f_gamepads, &g, compare_wanderlust_gamepad_by_id, 0
		);
	}
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s32 gp_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized 	= e_configuration.success;
	f_gamepad		= e_configuration.null;
	f_gamepads		= e_configuration.null;
	f_idcounter		= e_configuration.zero;

	if (! L)
	{
		goto FAILURE;
	}

	lua_register (L, WL_LOAD_CONTROLLER_MAPPING, gp_load_mapping_from_file);

	if (! (f_gamepads = mcreate (e_configuration.morph.doublelinkedlist)))
	{
		goto FAILURE;
	}

	rebuild_gamepad_list (0);

	task_register (TASK_GAMEPAD_REBUILD, rebuild_gamepad_list);

	return e_configuration.success;

	FAILURE:

		gp_terminate ();

		return e_configuration.failure;
}

nil gp_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (f_gamepads)
	{
		f_gamepads = mdelete (f_gamepads, delete_wanderlust_gamepad);
	}

	f_gamepad		= 0;
	f_idcounter		= 0;
	f_initialized 	= e_configuration.failure;
}

nil gp_device_insert ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	task_enqueue (TASK_GAMEPAD_REBUILD, 0);
}

nil gp_device_remove (s32 device)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	task_enqueue (TASK_GAMEPAD_REBUILD, 0);
}

nil gp_set_axis (u08 axis, s16 value)
{
	if (! f_gamepad)
	{
		return;
	}

	switch (axis)
	{

	case SDL_CONTROLLER_AXIS_LEFTX:

		if ((f_gamepad -> x = value) > 0)
		{
			if (value > f_deadzone_p)
			{
				gp_set_button (SDL_CONTROLLER_BUTTON_DPAD_RIGHT, 0xFF);
			}
			else
			{
				gp_set_button (SDL_CONTROLLER_BUTTON_DPAD_RIGHT, 0x00);
			}
		}
		else
		{
			if (value < f_deadzone_n)
			{
				gp_set_button (SDL_CONTROLLER_BUTTON_DPAD_LEFT, 0xFF);
			}
			else
			{
				gp_set_button (SDL_CONTROLLER_BUTTON_DPAD_LEFT, 0x00);
			}
		}

		break;

	case SDL_CONTROLLER_AXIS_LEFTY:

		if ((f_gamepad -> y = value) > 0)
		{
			if (f_gamepad -> y > f_deadzone_p)
			{
				gp_set_button (SDL_CONTROLLER_BUTTON_DPAD_DOWN, 0xFF);
			}
			else
			{
				gp_set_button (SDL_CONTROLLER_BUTTON_DPAD_DOWN, 0x00);
			}
		}
		else
		{
			if (f_gamepad -> y < f_deadzone_n)
			{
				gp_set_button (SDL_CONTROLLER_BUTTON_DPAD_UP, 0xFF);
			}
			else
			{
				gp_set_button (SDL_CONTROLLER_BUTTON_DPAD_UP, 0x00);
			}
		}

		break;
	}

}

nil gp_set_button (u16 button, u08 state)
{
	wanderlust_button * b = 0;

	if (! f_gamepad)
	{
		return;
	}

	if ((b = mfind (f_gamepad -> buttons, & button, compare_wanderlust_button_by_id, 0)))
	{
		if (state)
		{
			if (b -> pressed)
			{
				b -> buffer = 0xFF;
			}

			b -> pressed		= 0xFF;
			f_gamepad -> input	= f_gamepad -> input + 1;
		}
		else
		{
			b -> buffer			= 0x00;
			b -> pressed		= 0x00;
			f_gamepad -> input	= f_gamepad -> input - 1;
		}

		return;
	}

	if (! (b = create_wanderlust_button (button)))
	{
		return;
	}

	if (state)
	{
		f_gamepad -> input	= f_gamepad -> input + 1;
		b -> pressed		= 0xFF;
	}
	else
	{
		b -> pressed 		= 0x00;
	}

	madd (f_gamepad -> buttons, b);
}

u08 gp_get_button (u16 button)
{
	wanderlust_button * b = 0;

	if (! f_gamepad)
	{
		goto FAILURE;
	}

	b = mfind (f_gamepad -> buttons, & button, compare_wanderlust_button_by_id, 0);

	if (! b)
	{
		goto FAILURE;
	}

	return b -> pressed;

	FAILURE:

		return e_configuration.zero;
}

u08 gp_get_buffered_button (u16 button)
{
	wanderlust_button * b = 0;

	if (! f_gamepad)
	{
		goto FAILURE;
	}

	b = mfind (f_gamepad -> buttons, & button, compare_wanderlust_button_by_id, 0);

	if (! b)
	{
		goto FAILURE;
	}

	if (b -> pressed)
	{
		if (b -> buffer)
		{
			return 0;
		}

		return (b -> buffer = 0xFF);
	}

	return (b -> buffer = 0x00);

	FAILURE:

		return e_configuration.zero;
}

#define GP_LOAD_MAPPING_FROM_FILE_ARG_COUNT		1
#define GP_LOAD_MAPPING_FROM_FILE_ARS_POS_FILE	1

s32 gp_load_mapping_from_file (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < GP_LOAD_MAPPING_FROM_FILE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, GP_LOAD_MAPPING_FROM_FILE_ARS_POS_FILE))
	{
		goto FAILURE;
	}

	SDL_GameControllerAddMappingsFromFile
	(
		lua_tostring (L, GP_LOAD_MAPPING_FROM_FILE_ARS_POS_FILE)
	);

	task_enqueue (TASK_GAMEPAD_REBUILD, 0);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}
