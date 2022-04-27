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

#ifndef WANDERLUST_EVENT
#define WANDERLUST_EVENT

/* ************************************************************************
 *
 * CONSTS
 *
 * ***********************************************************************/

#define EVENT_DOOR_N	0x00

#define EVENT_DOOR_E	0x01

#define EVENT_DOOR_S	0x02

#define EVENT_DOOR_W	0x03

#define EVENT_DOOR		0x04

#define EVENT_SIGN		0x05

#define EVENT_JUMP_N	0x10

#define EVENT_JUMP_E	0x11

#define EVENT_JUMP_S	0x12

#define EVENT_JUMP_W	0x13

#define EVENT_JUMP		0x14

#define EVENT_SURF		0x25

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 event_initialize (lua_State *);

nil event_terminate ();

nil event_trigger (u08, wanderlust_entity *);

nil event_attach (u08, nil (*)(nil *));

nil event_deattach (u08 nil);

s32 event_attach_from_lua (lua_State *);

s32 event_deattach_from_lua (lua_State *);

#endif
