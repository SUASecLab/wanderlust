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

#ifndef WANDERLUST_PROPERTY
#define WANDERLUST_PROPERTY

s08		 		property_initialize (lua_State *);

nil 			property_terminate ();

nil				property_create_integer (const s08 *, u32);

nil				property_create_string (const s08 *, const s08 *);

nil				property_create_point (const s08 *, u32, u32);

nil				property_create_rectangle (const s08 *, u32, u32, u32, u32);

s32				property_delete_wanderlust_property (lua_State *);

s32				property_get_wanderlust_property (lua_State *);

s32				property_set_wanderlust_property (lua_State *);

u32				property_get_integer (const s08 *);

nil				property_set_integer (const s08 *, int);

const s08	*	property_get_string (const s08 *);

nil				property_set_string (const s08 *, const s08 *);

SDL_Point		property_get_point (const s08 *);

nil				property_set_point (const s08 *, SDL_Point *);

SDL_Rect		property_get_rectangle (const s08 *);

nil				property_set_rectangle (const s08 *, SDL_Rect *);

#endif
