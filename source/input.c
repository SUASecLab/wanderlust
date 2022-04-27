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
#include "input.h"
#include "keyboard.h"
#include "gamepad.h"

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
 * ***********************************************************************/

static s08 f_initialized = WANDERLUST_FAILURE;

/* ************************************************************************
 *
 * BANKS
 *
 * ***********************************************************************/

static SDL_Scancode f_kb_buttons [BUTTON_COUNT] =
{
	SDL_SCANCODE_I, 	/* X */
	SDL_SCANCODE_K, 	/* A */
	SDL_SCANCODE_J, 	/* B */
	SDL_SCANCODE_U, 	/* Y */

	SDL_SCANCODE_Q, 	/* SELECT/BACK */
	SDL_SCANCODE_R,		/* GUIDE */
	SDL_SCANCODE_E,		/* START */

	SDL_SCANCODE_F, 	/* L */
	SDL_SCANCODE_H, 	/* R */

	SDL_SCANCODE_W, 	/* UP */
	SDL_SCANCODE_S, 	/* DOWN */
	SDL_SCANCODE_A, 	/* LEFT */
	SDL_SCANCODE_D, 	/* RIGHT */

	SDL_SCANCODE_F11,	/* TOGGLE FULLSCREEN */
	SDL_SCANCODE_F1,	/* DEC_WINDOW_SIZE */
	SDL_SCANCODE_F2,	/* INC_WINDOW_SIZE */
};

static SDL_Scancode f_gp_buttons [BUTTON_COUNT] =
{
	SDL_CONTROLLER_BUTTON_X,
	SDL_CONTROLLER_BUTTON_A,
	SDL_CONTROLLER_BUTTON_B,
	SDL_CONTROLLER_BUTTON_Y,

	SDL_CONTROLLER_BUTTON_BACK,
	SDL_CONTROLLER_BUTTON_GUIDE,
	SDL_CONTROLLER_BUTTON_START,

	SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
	SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,

	SDL_CONTROLLER_BUTTON_DPAD_UP,
	SDL_CONTROLLER_BUTTON_DPAD_DOWN,
	SDL_CONTROLLER_BUTTON_DPAD_LEFT,
	SDL_CONTROLLER_BUTTON_DPAD_RIGHT,

	SDL_CONTROLLER_BUTTON_INVALID,
	SDL_CONTROLLER_BUTTON_LEFTSTICK,
	SDL_CONTROLLER_BUTTON_RIGHTSTICK
};

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 input_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized = e_configuration.success;

	if (kb_initialize (L) == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (gp_initialize (L) == e_configuration.failure)
	{
		goto FAILURE;;
	}

	return e_configuration.success;

	FAILURE:

		input_terminate ();

		return e_configuration.failure;
}

nil input_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	kb_terminate ();

	gp_terminate ();

	f_initialized = e_configuration.failure;
}

nil input_button_up (u16 code, u08 gamepad)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (gamepad)
	{
		gp_set_button (code, 0x00);
	}
	else
	{
		kb_set_button (code, 0x00);
	}
}

nil input_button_down (u16 code, u08 gamepad)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (gamepad)
	{
		gp_set_button (code, 0xFF);
	}
	else
	{
		kb_set_button (code, 0xFF);
	}
}

nil input_device_insert (s32 device)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	gp_device_insert (device);
}

nil input_device_remove (s32 device)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	gp_device_remove (device);
}

nil input_axis_set_motion (u08 axis, s16 value)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	gp_set_axis (axis, value);
}

s16 input_axis_get_motion (u08 axis)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	FAILURE:

		return e_configuration.zero;
}

u08 input_any_key_down ()
{
	return kb_any_key_down ();
}

s08 input_get_button (wanderlust_button_id button)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	return
		kb_get_button (f_kb_buttons [button]) ||
		gp_get_button (f_gp_buttons [button]) ;

	FAILURE:

		return e_configuration.zero;
}

s08 input_get_buffered_button (wanderlust_button_id button)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	return
		kb_get_buffered_button (f_kb_buttons [button]) |
		gp_get_buffered_button (f_gp_buttons [button]) ;

	FAILURE:

		return e_configuration.zero;
}
