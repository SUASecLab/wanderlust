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

#ifndef WANDERLUST_TERMINAL
#define WANDERLUST_TERMINAL

/* ************************************************************************
 *
 * TYPE DEFINITIONS
 *
 * ************************************************************************/

typedef struct
{
	const	s08 					active;

			wanderlust_frame	*	frame;
			lua_State			*	script;

	struct {
					u08 *	data;
			const	u08		rows;
			const	u08		cols;
			const	u16		size;
	} buffer;

	struct {
		u08				r;
		u08				c;
		u16				index;
		SDL_Texture	*	active;
		SDL_Texture	*	inactive;
	} cursor;
} wanderlust_terminal;

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

s08 terminal_initialize (lua_State *);

nil terminal_terminate ();

s08 terminal_active ();

s32 terminal_create (lua_State *);

s32 terminal_delete (lua_State *);

s32 terminal_show (lua_State *);

s32 terminal_hide (lua_State *);

s32 terminal_push_byte (lua_State *);

s32 terminal_undo_byte (lua_State *);

s32 terminal_increment_cursor (lua_State *);

s32 terminal_decrement_cursor (lua_State *);

s32 terminal_text_mode (lua_State *);

s32 terminal_data_mode (lua_State *);

s32 terminal_run (lua_State *);

s32 terminal_clear (lua_State *);

nil terminal_handle ();

nil terminal_update ();

nil terminal_render ();

#endif
