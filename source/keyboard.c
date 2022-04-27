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
#include "morph.h"
#include "keyboard.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * TYPE DEFINITIONS
 *
 * ***********************************************************************/

typedef struct
{
	u16	scancode;
	u08	pressed;
	u08	buffered;
} wanderlust_key;

/* ************************************************************************
 *
 * COMPARATOR
 *
 * ***********************************************************************/

static u08 compare_wanderlust_key_by_scancode (nil * item, nil * value)
{
	if (! item)
	{
		return 0;
	}

	if (! value)
	{
		return 0;
	}

	return (((wanderlust_key *)item) -> scancode) == *((u16 *) value);
}

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

static s08 		f_initialized	= WANDERLUST_FAILURE;

static nil	*	f_keys			= 0;

static u08		f_keys_down		= 0;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ***********************************************************************/

static wanderlust_key * create_wanderlust_key (u16 scancode)
{
	wanderlust_key * key = SDL_malloc (sizeof (wanderlust_key));

	if (! key)
	{
		return 0;
	}

	SET_U16_CONST (key -> scancode, scancode);

	key -> pressed 	= 0;
	key -> buffered	= 0;

	return key;
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 kb_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized 	= e_configuration.success;
	f_keys			= e_configuration.null;
	f_keys_down		= e_configuration.zero;

	if (! (f_keys = mcreate (e_configuration.morph.singlelinkedlist)))
	{
		goto FAILURE;
	}

	return e_configuration.success;

	FAILURE:

		kb_terminate ();

		return e_configuration.failure;
}

nil kb_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (f_keys)
	{
		f_keys = mdelete (f_keys, SDL_free);
	}

	f_keys_down		= 0;
	f_initialized 	= e_configuration.failure;
}

nil kb_set_button (u16 code, u08 state)
{
	wanderlust_key * key = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	key = mfind (f_keys, & code, compare_wanderlust_key_by_scancode, 0);

	if (key)
	{
		if (state)
		{
			if (key -> pressed)
			{
				key -> buffered = 0xFF;
			}

			key -> pressed = 0xFF;

			++ f_keys_down;
		}
		else
		{
			key -> pressed = 0x00;

			-- f_keys_down;
		}

		return;
	}

	key = create_wanderlust_key (code);

	if (! key)
	{
		return;
	}

	key -> pressed = state;

	++ f_keys_down;

	madd (f_keys, key);
}

u08 kb_any_key_down ()
{
	return f_keys_down;
}

s08 kb_get_button (u16 code)
{
	wanderlust_key * key = 0;

	if (f_initialized == e_configuration.failure)
	{
		return 0;
	}

	key = mfind (f_keys, &code, compare_wanderlust_key_by_scancode, 0);

	if (! key)
	{
		goto FAILURE;
	}

	return key -> pressed;

	FAILURE:

		return e_configuration.zero;
}

s08 kb_get_buffered_button (u16 code)
{
	wanderlust_key * key = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	key = mfind (f_keys, &code, compare_wanderlust_key_by_scancode, 0);

	if (! key)
	{
		goto FAILURE;
	}

	if (key -> pressed)
	{
		if (key -> buffered)
		{
			return 0;
		}

		return (key -> buffered = 1);
	}

	return (key -> buffered = 0);

	FAILURE:

		return e_configuration.zero;
}
