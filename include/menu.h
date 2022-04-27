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

#ifndef WANDERLUST_MENU
#define WANDERLUST_MENU

s08 menu_initialize (lua_State *);

nil menu_terminate ();

s32 menu_select (lua_State *);

s32 menu_repeal (lua_State *);

s32 menu_create (lua_State *);

s32 menu_append (lua_State *);

s08 menu_active ();

nil menu_handle ();

nil menu_update ();

nil menu_render ();

#endif
