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

#ifndef WANDERLUST_GAMEPAD
#define WANDERLUST_GAMEPAD

s32 gp_initialize (lua_State *);

nil gp_terminate ();

nil gp_device_insert ();

nil gp_device_remove (s32);

nil gp_set_axis (u08, s16);

nil gp_set_button (u16, u08);

u08 gp_get_button (u16);

u08 gp_get_buffered_button (u16);

s32 gp_load_mapping_from_file (lua_State *);

#endif
