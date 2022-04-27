/*
 * Wanderlust is an engine for rpgs rendered in top-down perspective, which
 * where common in the consoles of the 8- and 16-Bit Era.
 * Copyright (C) 2022 Stefan Reich
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "type.h"
#include "config.h"
#include "font.h"
#include "entity.h"
#include "video.h"
#include "input.h"
#include "task.h"
#include "map.h"
#include "transition.h"
#include "textbox.h"
#include "menu.h"
#include "property.h"
#include "engine.h"
#include "terminal.h"
#include "state.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ************************************************************************/

static s08 				f_initialized 	= WANDERLUST_FAILURE;

static u08 				f_running		= 0;

static lua_State	*	f_lua			= 0;

static u08				f_ver_a			= 0;

static u08				f_ver_b			= 1;

static u08				f_ver_c			= 13;

static u08				f_ver_d			= 5;

static u32				f_cycle			= 0;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ************************************************************************/

static nil direct ()
{
	state_direct ();
}

static nil handle ()
{
	SDL_Event event;

	if (transition_pending ())
	{
		return;
	}

	input_axis_set_motion (0, 0);
	input_axis_set_motion (1, 0);

	while (SDL_PollEvent (& event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			f_running = 0x00;
			break;

		case SDL_KEYDOWN:
			input_button_down (event.key.keysym.scancode, 0x00);
			break;

		case SDL_KEYUP:
			input_button_up (event.key.keysym.scancode, 0x00);
			break;

		case SDL_CONTROLLERAXISMOTION:
			input_axis_set_motion (event.caxis.axis, event.caxis.value);
			break;

		case SDL_CONTROLLERDEVICEADDED:
			input_device_insert ();
			break;

		case SDL_CONTROLLERDEVICEREMOVED:
			input_device_remove (event.cdevice.which);
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			input_button_down (event.cbutton.button, 0xFF);
			break;

		case SDL_CONTROLLERBUTTONUP:
			input_button_up (event.cbutton.button, 0xFF);
			break;
		}
	}

	state_handle ();
}

static nil update ()
{
	if (transition_pending ())
	{
		transition_update ();

		return;
	}

	state_update ();
}

static nil render ()
{
	video_render_clear ();

	state_render ();

	if (transition_pending ())
	{
		transition_render ();
	}

	video_render_present ();
}

static nil adjust ()
{
	state_adjust ();
}

static nil detain (s32 delay)
{
	if (delay < e_configuration.fps)
	{
		SDL_Delay (e_configuration.fps - delay);
	}
}

static nil task_engine_halt (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	f_running = 0;
}

/* ************************************************************************
 *
 * INTERFACE FUNCTIONS
 *
 * ************************************************************************/

s08 engine_bootstrap (const char * script)
{
	if (f_initialized == WANDERLUST_SUCCESS)
	{
		goto SUCCESS;
	}

	if (! script)
	{
		goto FAILURE;
	}

	if (configuration_bootstrap () == WANDERLUST_FAILURE)
	{
		goto FAILURE;
	}

	if (! (f_lua = luaL_newstate()))
	{
		goto FAILURE;
	}

	luaL_openlibs (f_lua);

	if (property_initialize (f_lua) == WANDERLUST_FAILURE)
	{
		goto FAILURE;
	}

	luaL_loadfile(f_lua, script);

	lua_pcall (f_lua, 0, 0, 0);

	lua_getglobal (f_lua, WL_INIT_PROPERTIES);

	lua_pcall (f_lua, 0, 0, 0);

	SUCCESS:
		return WANDERLUST_SUCCESS;

	FAILURE:
		return WANDERLUST_FAILURE;
}

s08 engine_initialize (const char * script)
{
	if (f_initialized == WANDERLUST_SUCCESS)
	{
		goto SUCCESS;
	}

	f_initialized = WANDERLUST_SUCCESS;

	if (configuration_initialize (f_lua) == WANDERLUST_FAILURE)
	{
		goto FAILURE;
	}

	task_register (TASK_ENGINE_HALT, task_engine_halt);

	lua_getglobal (f_lua, WL_INIT_DEFAULT);

	lua_pcall (f_lua, 0, 0, 0);

	f_cycle	= 0;

	SUCCESS:

		return WANDERLUST_SUCCESS;

	FAILURE:

		return engine_terminate ();
}

s08 engine_terminate ()
{
	if (f_initialized == WANDERLUST_FAILURE)
	{
		return f_initialized;
	}

	if (f_lua)
	{
		lua_close (f_lua);
	}

	configuration_terminate ();
	property_terminate ();

	f_initialized	= WANDERLUST_FAILURE;
	f_cycle			= 0;

	return WANDERLUST_SUCCESS;
}

s08 engine_execute ()
{
	s32 ticks = 0;

	if (f_initialized == e_configuration.failure)
	{
		return f_initialized;
	}

	if (f_running)
	{
		goto FAILURE;
	}

	f_running = 0xFF;

	while (f_running)
	{
		ticks = SDL_GetTicks ();

		adjust ();
		direct ();
		handle ();
		update ();
		adjust ();
		render ();

		if (task_pending ())
		{
			task_execute ();
		}

		detain (SDL_GetTicks () - ticks);

		f_cycle += 1;
	}

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

nil engine_print_version ()
{
	printf ("wanderlust v. %i.%i.%i.%i\n", f_ver_a, f_ver_b, f_ver_c, f_ver_d);
}

u32 engine_get_cycle ()
{
	return f_cycle;
}

s32 engine_quit (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_ENGINE_HALT, 0);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}
