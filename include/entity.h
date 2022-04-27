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

#ifndef WANDERLUST_ENTITY
#define WANDERLUST_ENTITY

/* ************************************************************************
 *
 * TYPE DEFINITIONS
 *
 * ***********************************************************************/

typedef struct
{
	const 	u08 						type;

	const	u08							locked;

	const 	u16 						id;

	const 	u32							hash;

			wanderlust_direction		direction;

			u08							row;

			u08							column;

			s08						*	title;

			nil						*	properties;

			SDL_Texture				*	texture;

			SDL_Rect				*	source;

			SDL_Rect					target;

} wanderlust_entity;

typedef struct
{
	u08 row;
	u08 column;
	u08 direction;
	u32 hash;
} wanderlust_warp_point;

typedef enum
{
	HERO_ENTITY,
	NPC_ENTITY,
	RETROBUTTON_ENTITY,
	PRIMITIVE_ENTITY,
	ENTITY_TYPE_COUNT
} wanderlust_entity_types;

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08					entity_initialize (lua_State *);

nil					entity_terminate ();

nil					entity_fetch (nil *);

s32					entity_enqueue (lua_State *);

wanderlust_entity * entity_create (lua_State *);

nil 				entity_delete (nil *);

nil 				entity_direct (nil *, nil *);

nil 				entity_handle (nil *, nil *);

nil 				entity_update (nil *, nil *);

nil 				entity_render (nil *, nil *);

nil					entity_adjust (nil *, nil *);

nil					entity_select (nil *, nil *);

nil					entity_recall (nil *, nil *);

nil					entity_excite (nil *, nil *);

nil					entity_expire (nil *, nil *);

nil					entity_modify (nil *, lua_State *, u16);

#endif
