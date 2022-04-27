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
#include "texture.h"
#include "video.h"
#include "entity.h"
#include "state.h"
#include "property.h"
#include "map.h"
#include "textbox.h"
#include "morph.h"
#include "scout.h"

#include "entity.npc.h"

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

typedef enum
{
	IDLE_NORTH = 0,
	WALK_NORTH_A,
	WALK_NORTH_B,
	IDLE_EAST,
	WALK_EAST_A,
	IDLE_SOUTH,
	WALK_SOUTH_A,
	WALK_SOUTH_B,
	IDLE_WEST,
	WALK_WEST_A,
	JUMP_SHADOW,
	RIDE_IDLE_N,
	RIDE_MOVE_N_A,
	RIDE_MOVE_N_B,
	RIDE_IDLE_E,
	RIDE_MOVE_E_A,
	RIDE_IDLE_S,
	RIDE_MOVE_S_A,
	RIDE_MOVE_S_B,
	RIDE_IDLE_W,
	RIDE_MOVE_W_A,
	NPC_SPRITE_LOCATIONS
} npc_sprite_location;

typedef enum
{
	IDLE_N,
	IDLE_E,
	IDLE_S,
	IDLE_W,
	WALK_N,
	WALK_E,
	WALK_S,
	WALK_W,
	WAIT_N,
	WAIT_E,
	WAIT_S,
	WAIT_W,
	ABSENT = 0xFF
} npc_input_types;

typedef struct
{
	u08 			state;
	SDL_Point		origin;
	u16 			steps;
	u32 			counter;
	char		*	callback;
	lua_State	*	lua;
	nil			*	directions;
	struct
	{
		u08 search;
		u08 row;
		u08 column;
	} destination;
} npc_property;

typedef enum
{
	NPC_WALK_SEQA_N, NPC_WALK_SEQA_E, NPC_WALK_SEQA_S, NPC_WALK_SEQA_W,
	NPC_WALK_SEQB_N, NPC_WALK_SEQB_E, NPC_WALK_SEQB_S, NPC_WALK_SEQB_W,
	NPC_WALK_SEQC_N, NPC_WALK_SEQC_E, NPC_WALK_SEQC_S, NPC_WALK_SEQC_W,
	NPC_WALK_SEQD_N, NPC_WALK_SEQD_E, NPC_WALK_SEQD_S, NPC_WALK_SEQD_W,
	NPC_WALK_WAIT_N, NPC_WALK_WAIT_E, NPC_WALK_WAIT_S, NPC_WALK_WAIT_W,
	NPC_STATES
} npc_state;

typedef enum
{
	MODIFY_TITLE,
	MODIFY_ROW,
	MODIFY_COLUMN,
	MODIFY_DIRECTION,
	MODIFY_TEXTURE,
	MODIFY_LOCATION_ROW,
	MODIFY_LOCATION_COLUMN,
	MODIFIER_COUNT
}
npc_modifier;

/* ************************************************************************
 *
 * BANKS
 *
 * ***********************************************************************/

static SDL_Rect f_sprite_sources [NPC_SPRITE_LOCATIONS];

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - HELPING HANDS
 *
 * ***********************************************************************/

static nil npc_delete_property (npc_property *);

#define NPC_CREATE_PROPERTIES_ARG_POS_SCRIPT 6

static npc_property * npc_create_properties (lua_State * L)
{
	npc_property * p = 0;

	if (! L)
	{
		goto FAILURE;
	}

	if (! (p = SDL_malloc (sizeof (npc_property))))
	{
		goto FAILURE;
	}

	if (! (p->lua = lua_open ()))
	{
		goto FAILURE;
	}

	lua_register (p->lua, WL_MODIFY_STATE_ENTITY,	state_entity_modify);
	lua_register (p->lua, WL_GET_PROPERTY, 		property_get_wanderlust_property);
	lua_register (p->lua, WL_SET_PROPERTY,		property_set_wanderlust_property);
	lua_register (p->lua, WL_CREATE_TEXTBOX,		textbox_enqueue);

	if (luaL_loadfile (p->lua, lua_tostring (L, NPC_CREATE_PROPERTIES_ARG_POS_SCRIPT)))
	{
		goto FAILURE;
	}

	if (lua_pcall (p->lua, 0, 0, 0))
	{
		goto FAILURE;
	}

	p->steps				= 0;
	p->counter			= 0;
	p->origin.x			= 0;
	p->origin.y			= 0;
	p->directions			= 0;

	p->destination.search	= 0;
	p->destination.row	= 0;
	p->destination.column	= 0;

	return p;

	FAILURE:

		if (p)
		{
			npc_delete_property (p);
		}

		return e_configuration.null;
}

static nil npc_delete_property (npc_property * p)
{
	if (! p)
	{
		return;
	}

	if (p->lua)
	{
		lua_close (p->lua);
	}

	if (p->directions)
	{
		p->directions = mdelete (p->directions, SDL_free);
	}

	SDL_free (p);
}

static nil adjust_position (wanderlust_entity * e, npc_property * p)
{
	e->target.x = e_configuration.tileset.width * (e->column - map_get_area_column ());
	e->target.x = e->target.x + map_get_origin_x () + p->origin.x;

	e->target.y = e_configuration.tileset.height * (e->row - map_get_area_row ());
	e->target.y = e->target.y + map_get_origin_y () + p->origin.y;
}

#define PATHFINDING_CALLBACK_NEXT	"next"

static nil pathfinding (wanderlust_entity * e, npc_property * p)
{
	wanderlust_scout scout = {0, 0, 0, 0, 0, {0, 0}, {0, 0}, 0, 0};

	if (p->directions)
	{
		goto end;
	}

	lua_getglobal	(p->lua, PATHFINDING_CALLBACK_NEXT);

	lua_pushinteger (p->lua, e->row);
	lua_pushinteger (p->lua, e->column);

	if (lua_pcall (p->lua, 2, 1, 0) != 0)
	{
		printf ("Message from dr. light: unable to call script by %s\n", e->title);

		goto end;
	}

	if (! lua_isnumber (p->lua, 1))
	{
		printf ("lua: '%s' is not a number\n", lua_tostring (p->lua, -1));

		goto end;
	}

	if (lua_tointeger (p->lua, 1) == e_configuration.failure)
	{
		printf ("lua: failure in next() by %s\n", e->title);

		goto end;
	}

	scout.direction = e->direction;
	scout.n			= e_configuration.direction.n;
	scout.e			= e_configuration.direction.e;
	scout.s			= e_configuration.direction.s;
	scout.w			= e_configuration.direction.w;
	scout.source.r	= e->row;
	scout.source.c	= e->column;
	scout.target.r	= p->destination.row;
	scout.target.c	= p->destination.column;
	scout.terrain	= e_configuration.map.terrain.open;

	p->directions = scout_next_b (& scout);

	end:
		lua_pop (p->lua, lua_gettop (p->lua));
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - SETUP
 *
 * ***********************************************************************/

static nil setup_walk_seqa_n (wanderlust_entity * e, npc_property * p)
{
	e->source		= & f_sprite_sources [IDLE_NORTH];
	e->direction	= e_configuration.direction.n;
	p->origin.y	= 0;

	if
	(
		e->row 	== p->destination.row &&
		e->column	== p->destination.column
	)
	{
		p->destination.search	= 0x00;
		p->destination.column	= 0x00;
		p->destination.row		= 0x00;
	}
}

static nil setup_walk_seqa_e (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_EAST];
	e->direction	= e_configuration.direction.e;
	p->origin.x	= 0;

	if
	(
		e->row 	== p->destination.row &&
		e->column	== p->destination.column
	)
	{
		p->destination.search	= 0x00;
		p->destination.column	= 0x00;
		p->destination.row	= 0x00;
	}
}

static nil setup_walk_seqa_s (wanderlust_entity * e, npc_property * p)
{
	e->source		= & f_sprite_sources [IDLE_SOUTH];
	e->direction	= e_configuration.direction.s;
	p->origin.y	= 0;

	if
	(
		e->row 	== p->destination.row &&
		e->column	== p->destination.column
	)
	{
		p->destination.search	= 0x00;
		p->destination.column	= 0x00;
		p->destination.row	= 0x00;
	}
}

static nil setup_walk_seqa_w (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_WEST];
	e->direction	= e_configuration.direction.w;
	p->origin.x	= 0;

	if
	(
		e->row 	== p->destination.row &&
		e->column	== p->destination.column
	)
	{
		p->destination.search	= 0x00;
		p->destination.column	= 0x00;
		p->destination.row	= 0x00;
	}
}

static nil setup_walk_seqb_n (wanderlust_entity * e, npc_property * p)
{
	e->source		= & f_sprite_sources [IDLE_NORTH];
	e->direction	= e_configuration.direction.n;
	p->origin.y	= 0;

	if
	(
		e->row 	== p->destination.row &&
		e->column	== p->destination.column
	) {
		p->destination.search	= 0x00;
		p->destination.column	= 0x00;
		p->destination.row	= 0x00;
	}
}

static nil setup_walk_seqb_e (wanderlust_entity * e, npc_property * p)
{
	e->source		= & f_sprite_sources [IDLE_EAST];
	e->direction	= e_configuration.direction.e;
	p->origin.x	= 0;

	if
	(
		e->row 	== p->destination.row &&
		e->column	== p->destination.column
	) {
		p->destination.search	= 0x00;
		p->destination.column	= 0x00;
		p->destination.row	= 0x00;
	}
}

static nil setup_walk_seqb_s (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_SOUTH];
	e->direction	= e_configuration.direction.s;

	p->origin.y	= 0;

	if
	(
		e->row 	== p->destination.row &&
		e->column	== p->destination.column
	) {
		p->destination.search	= 0x00;
		p->destination.column	= 0x00;
		p->destination.row	= 0x00;
	}
}

static nil setup_walk_seqb_w (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_WEST];
	e->direction	= e_configuration.direction.w;

	p->origin.x	= 0;

	if
	(
		e->row 	== p->destination.row &&
		e->column	== p->destination.column
	) {
		p->destination.search	= 0x00;
		p->destination.column	= 0x00;
		p->destination.row	= 0x00;
	}
}

static nil setup_walk_seqc_n (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_NORTH];
	e->direction	= e_configuration.direction.n;
	p->steps		= e_configuration.tileset.height;
}

static nil setup_walk_seqc_e (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_EAST];
	e->direction	= e_configuration.direction.e;
	p->steps		= e_configuration.tileset.width;
}

static nil setup_walk_seqc_s (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_SOUTH];
	e->direction	= e_configuration.direction.s;
	p->steps		= e_configuration.tileset.height;
}

static nil setup_walk_seqc_w (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_WEST];
	e->direction	= e_configuration.direction.w;
	p->steps		= e_configuration.tileset.width;
}

static nil setup_walk_seqd_n (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_NORTH];
	e->direction	= e_configuration.direction.n;
	p->steps		= e_configuration.tileset.height;
}

static nil setup_walk_seqd_e (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_EAST];
	e->direction	= e_configuration.direction.e;
	p->steps		= e_configuration.tileset.width;
}

static nil setup_walk_seqd_s (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_SOUTH];
	e->direction	= e_configuration.direction.s;
	p->steps		= e_configuration.tileset.height;
}

static nil setup_walk_seqd_w (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_WEST];
	e->direction	= e_configuration.direction.w;
	p->steps		= e_configuration.tileset.width;
}

static nil setup_walk_wait_n (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_NORTH];
	e->direction	= e_configuration.direction.n;
	p->steps		= e_configuration.tileset.height;
}

static nil setup_walk_wait_e (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_EAST];
	e->direction	= e_configuration.direction.e;
	p->steps		= e_configuration.tileset.width;
}

static nil setup_walk_wait_s (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_SOUTH];
	e->direction	= e_configuration.direction.s;
	p->steps		= e_configuration.tileset.height;
}

static nil setup_walk_wait_w (wanderlust_entity * e, npc_property * p)
{
	e->source 	= & f_sprite_sources [IDLE_WEST];
	e->direction	= e_configuration.direction.w;
	p->steps		= e_configuration.tileset.width;
}

/* ************************************************************************
 *
 * BANK  - SETUP
 *
 * ***********************************************************************/

static nil (* setup [NPC_STATES]) (wanderlust_entity *, npc_property *) =
{
	setup_walk_seqa_n, setup_walk_seqa_e, setup_walk_seqa_s, setup_walk_seqa_w,
	setup_walk_seqb_n, setup_walk_seqb_e, setup_walk_seqb_s, setup_walk_seqb_w,
	setup_walk_seqc_n, setup_walk_seqc_e, setup_walk_seqc_s, setup_walk_seqc_w,
	setup_walk_seqd_n, setup_walk_seqd_e, setup_walk_seqd_s, setup_walk_seqd_w,
	setup_walk_wait_n, setup_walk_wait_e, setup_walk_wait_s, setup_walk_wait_w
};

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - MODIFIY
 *
 * ***********************************************************************/

static nil modify_title (wanderlust_entity * e, lua_State * L, u16 index)
{
	s08 * title = 0;

	if (! lua_isstring (L, index))
	{
		return;
	}

	if (! (title = (s08*)type_secure_copy_string ((const char*)lua_tostring (L, index))))
	{
		return;
	}

	SDL_free (e->title);

	e->title 	= title;
}

static nil modify_row (wanderlust_entity * e, lua_State * L, u16 index)
{
	if (! lua_isnumber (L, index))
	{
		return;
	}

	e->row = (u08) lua_tointeger (L, index);
}

static nil modify_column (wanderlust_entity * e, lua_State * L, u16 index)
{
	if (! lua_isnumber (L, index))
	{
		return;
	}

	e->column = (u08) lua_tointeger (L, index);
}

static nil modify_destination_row (wanderlust_entity * e, lua_State * L, u16 index)
{
	npc_property * p = e->properties;

	if (! lua_isnumber (L, index))
	{
		return;
	}

	p->destination.row	= (u08) lua_tointeger (L, index);
	p->destination.search = 0xFF;
}

static nil modify_destination_column (wanderlust_entity * e, lua_State * L, u16 index)
{
	npc_property * p = e->properties;

	if (! lua_isnumber (L, index))
	{
		return;
	}

	p->destination.column = (u08) lua_tointeger (L, index);
	p->destination.search = 0xFF;
}

static nil
modify_direction_n (wanderlust_entity * e, npc_property * p)
{
	switch (p->state)
	{
		case NPC_WALK_SEQA_E: case NPC_WALK_SEQA_S: case NPC_WALK_SEQA_W:

			setup [(p->state = NPC_WALK_SEQA_N)] (e, p);

			break;

		case NPC_WALK_SEQB_E: case NPC_WALK_SEQB_S: case NPC_WALK_SEQB_W:

			setup [(p->state = NPC_WALK_SEQB_N)] (e, p);

			break;

		default: break;
	}
}

static nil
modify_direction_e (wanderlust_entity * e, npc_property * p)
{
	switch (p->state)
	{
		case NPC_WALK_SEQA_N: case NPC_WALK_SEQA_S: case NPC_WALK_SEQA_W:

			setup [(p->state = NPC_WALK_SEQA_E)] (e, p);

			break;

		case NPC_WALK_SEQB_N: case NPC_WALK_SEQB_S: case NPC_WALK_SEQB_W:

			setup [(p->state = NPC_WALK_SEQB_E)] (e, p);

			break;

		default: break;
	}
}

static nil
modify_direction_s (wanderlust_entity * e, npc_property * p)
{
	switch (p->state)
	{
		case NPC_WALK_SEQA_N: case NPC_WALK_SEQA_E: case NPC_WALK_SEQA_W:

			setup [(p->state = NPC_WALK_SEQA_S)] (e, p);

			break;

		case NPC_WALK_SEQB_N: case NPC_WALK_SEQB_E: case NPC_WALK_SEQB_W:

			setup [(p->state = NPC_WALK_SEQB_S)] (e, p);

			break;

		default: break;
	}
}

static nil
modify_direction_w (wanderlust_entity * e, npc_property * p)
{
	switch (p->state)
	{
		case NPC_WALK_SEQA_N: case NPC_WALK_SEQA_E: case NPC_WALK_SEQA_W:

			setup [(p->state = NPC_WALK_SEQA_S)] (e, p);

			break;

		case NPC_WALK_SEQB_N: case NPC_WALK_SEQB_E: case NPC_WALK_SEQB_W:

			setup [(p->state = NPC_WALK_SEQB_S)] (e, p);

			break;

		default: break;
	}
}

static nil
modify_direction (wanderlust_entity * e, lua_State * L, u16 index)
{
	u08 				direction 	= 0;
	npc_property	* 	p			= e->properties;

	if (! lua_isnumber (L, index))
	{
		return;
	}

	if ((direction = (u08) lua_tointeger (L, index)) < WANDERLUST_DIRECTIONS)
	{
		e->direction = direction;
	}

	switch (direction)
	{
		case WANDERLUST_DIRECTION_N:

			modify_direction_n (e, p);

			break;

		case WANDERLUST_DIRECTION_E:

			modify_direction_e (e, p);

			break;

		case WANDERLUST_DIRECTION_S:

			modify_direction_s (e, p);

			break;

		case WANDERLUST_DIRECTION_W:

			modify_direction_w (e, p);

			break;

		default: break;
	}
}

static nil modify_texture (wanderlust_entity * e, lua_State * L, u16 index)
{
	SDL_Texture	* t = 0;

	if (! lua_isstring (L, index))
	{
		return;
	}

	if (! (t = texture_get_by_name (lua_tostring (L, index))))
	{
		return;
	}

	e->texture = t;
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - ADJUST
 *
 * ***********************************************************************/

static nil adjust_walk_seqa_n (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_seqa_e (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_seqa_s (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_seqa_w (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_seqb_n (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_seqb_e (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_seqb_s (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_seqb_w (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_seqc_n (wanderlust_entity * e, npc_property * p) { }

static nil adjust_walk_seqc_e (wanderlust_entity * e, npc_property * p) { }

static nil adjust_walk_seqc_s (wanderlust_entity * e, npc_property * p) { }

static nil adjust_walk_seqc_w (wanderlust_entity * e, npc_property * p) { }

static nil adjust_walk_seqd_n (wanderlust_entity * e, npc_property * p) { }

static nil adjust_walk_seqd_e (wanderlust_entity * e, npc_property * p) { }

static nil adjust_walk_seqd_s (wanderlust_entity * e, npc_property * p) { }

static nil adjust_walk_seqd_w (wanderlust_entity * e, npc_property * p) { }

static nil adjust_walk_wait_n (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_wait_e (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_wait_s (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

static nil adjust_walk_wait_w (wanderlust_entity * e, npc_property * p)
{
	if (map_has_focus (e->row, e->column))
	{
		SET_U08_CONST (e->locked, (u08) 0x00);
	}
	else
	{
		SET_U08_CONST (e->locked, (u08) 0xFF);
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - DIRECT
 *
 * ***********************************************************************/

static nil direct_walk_seqa_n (wanderlust_entity * e, npc_property * p)
{
	if (p->directions)
	{
		return;
	}

	pathfinding (e, p);
}

static nil direct_walk_seqa_e (wanderlust_entity * e, npc_property * p)
{
	if (p->directions)
	{
		return;
	}

	pathfinding (e, p);
}

static nil direct_walk_seqa_s (wanderlust_entity * e, npc_property * p)
{
	if (p->directions)
	{
		return;
	}

	pathfinding (e, p);
}

static nil direct_walk_seqa_w (wanderlust_entity * e, npc_property * p)
{
	if (p->directions)
	{
		return;
	}

	pathfinding (e, p);
}

static nil direct_walk_seqb_n (wanderlust_entity * e, npc_property * p)
{
	if (p->directions)
	{
		return;
	}

	pathfinding (e, p);
}

static nil direct_walk_seqb_e (wanderlust_entity * e, npc_property * p)
{
	if (p->directions)
	{
		return;
	}

	pathfinding (e, p);
}

static nil direct_walk_seqb_s (wanderlust_entity * e, npc_property * p)
{
	if (p->directions)
	{
		return;
	}

	pathfinding (e, p);
}

static nil direct_walk_seqb_w (wanderlust_entity * e, npc_property * p)
{
	if (p->directions)
	{
		return;
	}

	pathfinding (e, p);
}

static nil direct_walk_seqc_n (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_seqc_e (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_seqc_s (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_seqc_w (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_seqd_n (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_seqd_e (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_seqd_s (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_seqd_w (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_wait_n (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_wait_e (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_wait_s (wanderlust_entity * e, npc_property * p) { }

static nil direct_walk_wait_w (wanderlust_entity * e, npc_property * p) { }

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - HANDLE
 *
 * ***********************************************************************/

static nil handle_walk_seqa_n (wanderlust_entity * e, npc_property * p)
{
	u08 		* 	direction 	= 0;
	SDL_Point		position	= {0, 0};

	if (! p->directions)
	{
		setup [(p->state = NPC_WALK_WAIT_N)] (e, p);

		return;
	}

	if (! (direction = mpeek (p->directions, 0)))
	{
		mdequeue (p->directions);

		setup [(p->state = NPC_WALK_WAIT_N)] (e, p);
	}
	else
	{

		if (* direction == WANDERLUST_DIRECTION_N)
		{
			position.x = e->column;
			position.y = e->row - 1;

			if (state_entity_has_collision (& position))
			{
				setup [(p->state = NPC_WALK_WAIT_N)] (e, p);

				return;
			}
			else
			{
				setup [(p->state = NPC_WALK_SEQC_N)] (e, p);
			}
		}
		else
		{
			switch (* direction)
			{
				case WANDERLUST_DIRECTION_E:
					setup [(p->state = NPC_WALK_SEQA_E)] (e, p);
					break;

				case WANDERLUST_DIRECTION_S:
					setup [(p->state = NPC_WALK_SEQA_S)] (e, p);
					break;

				case WANDERLUST_DIRECTION_W:
					setup [(p->state = NPC_WALK_SEQA_W)] (e, p);
					break;

				default:
					setup [(p->state = NPC_WALK_WAIT_N)] (e, p);
					break;
			}
		}

		SDL_free (mdequeue (p->directions));
	}

	if (! msize (p->directions))
	{
		p->directions = mdelete (p->directions, SDL_free);
	}
}

static nil handle_walk_seqa_e (wanderlust_entity * e, npc_property * p)
{
	u08 		* 	direction 	= 0;
	SDL_Point		position	= {0, 0};

	if (! p->directions)
	{
		setup [(p->state = NPC_WALK_WAIT_E)] (e, p);

		return;
	}

	if (! (direction = mpeek (p->directions, 0)))
	{
		mdequeue (p->directions);

		setup [(p->state = NPC_WALK_WAIT_E)] (e, p);
	}
	else
	{
		if (* direction == WANDERLUST_DIRECTION_E)
		{
			position.x = e->column + 1;
			position.y = e->row;

			if (state_entity_has_collision (& position))
			{
				setup [(p->state = NPC_WALK_WAIT_E)] (e, p);

				return;
			}
			else
			{
				setup [(p->state = NPC_WALK_SEQC_E)] (e, p);
			}
		}
		else
		{
			switch (* direction)
			{
				case WANDERLUST_DIRECTION_N:
					setup [(p->state = NPC_WALK_SEQA_N)] (e, p);
					break;

				case WANDERLUST_DIRECTION_S:
					setup [(p->state = NPC_WALK_SEQA_S)] (e, p);
					break;

				case WANDERLUST_DIRECTION_W:
					setup [(p->state = NPC_WALK_SEQA_W)] (e, p);
					break;

				default:
					setup [(p->state = NPC_WALK_WAIT_E)] (e, p);
					break;
			}
		}

		SDL_free (mdequeue (p->directions));
	}

	if (! msize (p->directions))
	{
		p->directions = mdelete (p->directions, SDL_free);
	}
}

static nil handle_walk_seqa_s (wanderlust_entity * e, npc_property * p)
{
	u08 		* 	direction 	= 0;
	SDL_Point		position	= {0, 0};

	if (! p->directions)
	{
		setup [(p->state = NPC_WALK_WAIT_S)] (e, p);

		return;
	}

	if (! (direction = mpeek (p->directions, 0)))
	{
		mdequeue (p->directions);

		setup [(p->state = NPC_WALK_WAIT_S)] (e, p);
	}
	else
	{
		if (* direction == WANDERLUST_DIRECTION_S)
		{
			position.x = e->column;
			position.y = e->row + 1;

			if (state_entity_has_collision (& position))
			{
				setup [(p->state = NPC_WALK_WAIT_S)] (e, p);

				return;
			}
			else
			{
				setup [(p->state = NPC_WALK_SEQC_S)] (e, p);
			}
		}
		else
		{
			switch (* direction)
			{
				case WANDERLUST_DIRECTION_N:
					setup [(p->state = NPC_WALK_SEQA_N)] (e, p);
					break;

				case WANDERLUST_DIRECTION_E:
					setup [(p->state = NPC_WALK_SEQA_E)] (e, p);
					break;

				case WANDERLUST_DIRECTION_W:
					setup [(p->state = NPC_WALK_SEQA_W)] (e, p);
					break;

				default:
					setup [(p->state = NPC_WALK_WAIT_S)] (e, p);
					break;
			}
		}

		SDL_free (mdequeue (p->directions));
	}

	if (! msize (p->directions))
	{
		p->directions = mdelete (p->directions, SDL_free);
	}
}

static nil handle_walk_seqa_w (wanderlust_entity * e, npc_property * p)
{
	u08 		* 	direction 	= 0;
	SDL_Point 		position	= {0, 0};

	if (! p->directions)
	{
		setup [(p->state = NPC_WALK_WAIT_W)] (e, p);

		return;
	}

	if (! (direction = mpeek (p->directions, 0)))
	{
		mdequeue (p->directions);

		setup [(p->state = NPC_WALK_WAIT_W)] (e, p);
	}
	else
	{
		if (* direction == WANDERLUST_DIRECTION_W)
		{
			position.x = e->column - 1;
			position.y = e->row;

			if (state_entity_has_collision (& position))
			{
				setup [(p->state = NPC_WALK_WAIT_W)] (e, p);

				return;
			}
			else
			{
				setup [(p->state = NPC_WALK_SEQC_W)] (e, p);
			}
		}
		else
		{
			switch (* direction)
			{
				case WANDERLUST_DIRECTION_N:
					setup [(p->state = NPC_WALK_SEQA_N)] (e, p);
					break;

				case WANDERLUST_DIRECTION_E:
					setup [(p->state = NPC_WALK_SEQA_E)] (e, p);
					break;

				case WANDERLUST_DIRECTION_S:
					setup [(p->state = NPC_WALK_SEQA_S)] (e, p);
					break;

				default:
					setup [(p->state = NPC_WALK_WAIT_W)] (e, p);
					break;
			}
		}

		SDL_free (mdequeue (p->directions));
	}

	if (! msize (p->directions))
	{
		p->directions = mdelete (p->directions, SDL_free);
	}
}

static nil handle_walk_seqb_n (wanderlust_entity * e, npc_property * p)
{
	u08 		* 	direction 	= 0;
	SDL_Point		position	= {0, 0};

	if (! p->directions)
	{
		setup [(p->state = NPC_WALK_WAIT_N)] (e, p);

		return;
	}

	if (! (direction = mpeek (p->directions, 0)))
	{
		mdequeue (p->directions);

		setup [(p->state = NPC_WALK_WAIT_N)] (e, p);
	}
	else
	{
		if (* direction == e_configuration.direction.n)
		{
			position.x = e->column;
			position.y = e->row - 1;

			if (state_entity_has_collision (& position))
			{
				setup [(p->state = NPC_WALK_WAIT_N)] (e, p);

				return;
			}
			else
			{
				setup [(p->state = NPC_WALK_SEQD_N)] (e, p);
			}
		}
		else
		{
			switch (* direction)
			{
				case WANDERLUST_DIRECTION_E:
					setup [(p->state = NPC_WALK_SEQB_E)] (e, p);
					break;

				case WANDERLUST_DIRECTION_S:
					setup [(p->state = NPC_WALK_SEQB_S)] (e, p);
					break;

				case WANDERLUST_DIRECTION_W:
					setup [(p->state = NPC_WALK_SEQB_W)] (e, p);
					break;

				default:
					setup [(p->state = NPC_WALK_WAIT_N)] (e, p);
					break;
			}
		}

		SDL_free (mdequeue (p->directions));
	}

	if (! msize (p->directions))
	{
		p->directions = mdelete (p->directions, SDL_free);
	}
}

static nil handle_walk_seqb_e (wanderlust_entity * e, npc_property * p)
{
	u08 		* 	direction 	= 0;
	SDL_Point		position	= {0, 0};

	if (! p->directions)
	{
		setup [(p->state = NPC_WALK_WAIT_E)] (e, p);

		return;
	}

	if (! (direction = mpeek (p->directions, 0)))
	{
		mdequeue (p->directions);

		setup [(p->state = NPC_WALK_WAIT_E)] (e, p);
	}
	else
	{
		if (* direction == e_configuration.direction.e)
		{
			position.x = e->column + 1;
			position.y = e->row;

			if (state_entity_has_collision (& position))
			{
				setup [(p->state = NPC_WALK_WAIT_E)] (e, p);

				return;
			}
			else
			{
				setup [(p->state = NPC_WALK_SEQD_E)] (e, p);
			}
		}
		else
		{
			switch (* direction)
			{
				case WANDERLUST_DIRECTION_N:
					setup [(p->state = NPC_WALK_SEQB_N)] (e, p);
					break;

				case WANDERLUST_DIRECTION_S:
					setup [(p->state = NPC_WALK_SEQB_S)] (e, p);
					break;

				case WANDERLUST_DIRECTION_W:
					setup [(p->state = NPC_WALK_SEQB_W)] (e, p);
					break;

				default:
					setup [(p->state = NPC_WALK_WAIT_E)] (e, p);
					break;
			}
		}

		SDL_free (mdequeue (p->directions));
	}

	if (! msize (p->directions))
	{
		p->directions = mdelete (p->directions, SDL_free);
	}
}

static nil handle_walk_seqb_s (wanderlust_entity * e, npc_property * p)
{
	u08 		* 	direction 	= 0;
	SDL_Point		position	= {0, 0};

	if (! p->directions)
	{
		setup [(p->state = NPC_WALK_WAIT_S)] (e, p);

		return;
	}

	if (! (direction = mpeek (p->directions, 0)))
	{
		mdequeue (p->directions);

		setup [(p->state = NPC_WALK_WAIT_S)] (e, p);
	}
	else
	{
		if (* direction == e_configuration.direction.s)
		{
			position.x = e->column;
			position.y = e->row + 1;

			if (state_entity_has_collision (& position))
			{
				setup [(p->state = NPC_WALK_WAIT_S)] (e, p);

				return;
			}
			else
			{
				setup [(p->state = NPC_WALK_SEQD_S)] (e, p);
			}
		}
		else
		{
			switch (* direction)
			{
				case WANDERLUST_DIRECTION_N:
					setup [(p->state = NPC_WALK_SEQB_N)] (e, p);
					break;

				case WANDERLUST_DIRECTION_E:
					setup [(p->state = NPC_WALK_SEQB_E)] (e, p);
					break;

				case WANDERLUST_DIRECTION_W:
					setup [(p->state = NPC_WALK_SEQB_W)] (e, p);
					break;

				default:
					setup [(p->state = NPC_WALK_WAIT_S)] (e, p);
					break;
			}
		}

		SDL_free (mdequeue (p->directions));
	}

	if (! msize (p->directions))
	{
		p->directions = mdelete (p->directions, SDL_free);
	}
}

static nil handle_walk_seqb_w (wanderlust_entity * e, npc_property * p)
{
	u08 		* 	direction 	= 0;
	SDL_Point		position	= {0, 0};

	if (! p->directions)
	{
		setup [(p->state = NPC_WALK_WAIT_W)] (e, p);

		return;
	}

	if (! (direction = mpeek (p->directions, 0)))
	{
		mdequeue (p->directions);

		setup [(p->state = NPC_WALK_WAIT_W)] (e, p);
	}
	else
	{
		if (* direction == e_configuration.direction.w)
		{
			position.x = e->column - 1;
			position.y = e->row;

			if (state_entity_has_collision (& position))
			{
				setup [(p->state = NPC_WALK_WAIT_W)] (e, p);

				return;
			}
			else
			{
				setup [(p->state = NPC_WALK_SEQD_W)] (e, p);
			}
		}
		else
		{
			switch (* direction)
			{
				case WANDERLUST_DIRECTION_N:
					setup [(p->state = NPC_WALK_SEQB_N)] (e, p);
					break;

				case WANDERLUST_DIRECTION_E:
					setup [(p->state = NPC_WALK_SEQB_E)] (e, p);
					break;

				case WANDERLUST_DIRECTION_S:
					setup [(p->state = NPC_WALK_SEQB_S)] (e, p);
					break;

				default:
					setup [(p->state = NPC_WALK_WAIT_W)] (e, p);
					break;
			}
		}

		SDL_free (mdequeue (p->directions));
	}

	if (! msize (p->directions))
	{
		p->directions = mdelete (p->directions, SDL_free);
	}
}

static nil handle_walk_seqc_n (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_seqc_e (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_seqc_s (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_seqc_w (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_seqd_n (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_seqd_e (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_seqd_s (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_seqd_w (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_wait_n (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_wait_e (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_wait_s (wanderlust_entity * e, npc_property * p) { }

static nil handle_walk_wait_w (wanderlust_entity * e, npc_property * p) { }

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - UPDATE
 *
 * ***********************************************************************/

static nil update_walk_seqa_n (wanderlust_entity * e, npc_property * p) { }

static nil update_walk_seqa_e (wanderlust_entity * e, npc_property * p) { }

static nil update_walk_seqa_s (wanderlust_entity * e, npc_property * p) { }

static nil update_walk_seqa_w (wanderlust_entity * e, npc_property * p) { }

static nil update_walk_seqb_n (wanderlust_entity * e, npc_property * p) { }

static nil update_walk_seqb_e (wanderlust_entity * e, npc_property * p) { }

static nil update_walk_seqb_s (wanderlust_entity * e, npc_property * p) { }

static nil update_walk_seqb_w (wanderlust_entity * e, npc_property * p) { }

static nil update_walk_seqc_n (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		if (p->steps < (e_configuration.tileset.height / 2))
		{
			e->source = & f_sprite_sources [WALK_NORTH_A];
		}

		-- p->origin.y;
		-- p->steps;
	}
	else
	{
		-- e->row; setup [(p->state = NPC_WALK_SEQB_N)] (e, p);
	}
}

static nil update_walk_seqc_e (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		if (p->steps < (e_configuration.tileset.width / 2))
		{
			e->source = & f_sprite_sources [WALK_EAST_A];
		}

		++ p->origin.x;
		-- p->steps;
	}
	else
	{
		++ e->column; setup [(p->state = NPC_WALK_SEQB_E)] (e, p);
	}
}

static nil update_walk_seqc_s (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		if (p->steps < (e_configuration.tileset.height / 2))
		{
			e->source = & f_sprite_sources [WALK_SOUTH_A];
		}

		++ p->origin.y;
		-- p->steps;
	}
	else
	{
		++ e->row; setup [(p->state = NPC_WALK_SEQB_S)] (e, p);
	}
}

static nil update_walk_seqc_w (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		if (p->steps < (e_configuration.tileset.width / 2))
		{
			e->source = & f_sprite_sources [WALK_WEST_A];
		}

		-- p->origin.x;
		-- p->steps;
	}
	else
	{
		-- e->column; setup [(p->state = NPC_WALK_SEQB_W)] (e, p);
	}
}

static nil update_walk_seqd_n (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		if (p->steps < (e_configuration.tileset.height / 2))
		{
			e->source = & f_sprite_sources [WALK_NORTH_B];
		}

		-- p->origin.y;
		-- p->steps;
	}
	else
	{
		-- e->row; setup [(p->state = NPC_WALK_SEQA_N)] (e, p);
	}
}

static nil update_walk_seqd_e (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		if (p->steps < (e_configuration.tileset.width / 2))
		{
			e->source = & f_sprite_sources [WALK_EAST_A];
		}

		++ p->origin.x;
		-- p->steps;
	}
	else
	{
		++ e->column; setup [(p->state = NPC_WALK_SEQA_E)] (e, p);
	}
}

static nil update_walk_seqd_s (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		if (p->steps < (e_configuration.tileset.height / 2))
		{
			e->source = & f_sprite_sources [WALK_SOUTH_B];
		}

		++ p->origin.y;
		-- p->steps;
	}
	else
	{
		++ e->row; setup [(p->state = NPC_WALK_SEQA_S)] (e, p);
	}
}

static nil update_walk_seqd_w (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		if (p->steps < (e_configuration.tileset.width / 2))
		{
			e->source = & f_sprite_sources [WALK_WEST_A];
		}

		-- p->origin.x;
		-- p->steps;
	}
	else
	{
		-- e->column; setup [(p->state = NPC_WALK_SEQA_W)] (e, p);
	}
}

static nil update_walk_wait_n (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		-- p->steps;
	}
	else
	{
		setup [(p->state = NPC_WALK_SEQA_N)] (e, p);
	}
}

static nil update_walk_wait_e (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		-- p->steps;
	}
	else
	{
		setup [(p->state = NPC_WALK_SEQA_E)] (e, p);
	}
}

static nil update_walk_wait_s (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		-- p->steps;
	}
	else
	{
		setup [(p->state = NPC_WALK_SEQA_S)] (e, p);
	}
}

static nil update_walk_wait_w (wanderlust_entity * e, npc_property * p)
{
	if (p->steps)
	{
		-- p->steps;
	}
	else
	{
		setup [(p->state = NPC_WALK_SEQA_W)] (e, p);
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - RENDER
 *
 * ***********************************************************************/

static nil render_walk_seqa_n (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqa_e (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqa_s (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqa_w (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqb_n (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqb_e (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqb_s (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqb_w (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqc_n (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqc_e (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqc_s (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqc_w (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqd_n (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqd_e (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqd_s (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqd_w (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_wait_n (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_wait_e (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_wait_s (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_wait_w (wanderlust_entity * e, npc_property * p)
{
	video_draw_entity (e);
}

/* ************************************************************************
 *
 * BANK
 *
 * ***********************************************************************/

static nil (* adjust [NPC_STATES]) (wanderlust_entity *, npc_property *) =
{
	adjust_walk_seqa_n, adjust_walk_seqa_e, adjust_walk_seqa_s, adjust_walk_seqa_w,
	adjust_walk_seqb_n, adjust_walk_seqb_e, adjust_walk_seqb_s, adjust_walk_seqb_w,
	adjust_walk_seqc_n, adjust_walk_seqc_e, adjust_walk_seqc_s, adjust_walk_seqc_w,
	adjust_walk_seqd_n, adjust_walk_seqd_e, adjust_walk_seqd_s, adjust_walk_seqd_w,
	adjust_walk_wait_n, adjust_walk_wait_e, adjust_walk_wait_s, adjust_walk_wait_w
};

static nil (* direct [NPC_STATES]) (wanderlust_entity *, npc_property *) =
{
	direct_walk_seqa_n, direct_walk_seqa_e, direct_walk_seqa_s, direct_walk_seqa_w,
	direct_walk_seqb_n, direct_walk_seqb_e, direct_walk_seqb_s, direct_walk_seqb_w,
	direct_walk_seqc_n, direct_walk_seqc_e, direct_walk_seqc_s, direct_walk_seqc_w,
	direct_walk_seqd_n, direct_walk_seqd_e, direct_walk_seqd_s, direct_walk_seqd_w,
	direct_walk_wait_n, direct_walk_wait_e, direct_walk_wait_s, direct_walk_wait_w
};

static nil (* handle [NPC_STATES]) (wanderlust_entity *, npc_property *) =
{
	handle_walk_seqa_n, handle_walk_seqa_e, handle_walk_seqa_s, handle_walk_seqa_w,
	handle_walk_seqb_n, handle_walk_seqb_e, handle_walk_seqb_s, handle_walk_seqb_w,
	handle_walk_seqc_n, handle_walk_seqc_e, handle_walk_seqc_s, handle_walk_seqc_w,
	handle_walk_seqd_n, handle_walk_seqd_e, handle_walk_seqd_s, handle_walk_seqd_w,
	handle_walk_wait_n, handle_walk_wait_e, handle_walk_wait_s, handle_walk_wait_w
};


static nil (* update [NPC_STATES]) (wanderlust_entity *, npc_property *) =
{
	update_walk_seqa_n, update_walk_seqa_e, update_walk_seqa_s, update_walk_seqa_w,
	update_walk_seqb_n, update_walk_seqb_e, update_walk_seqb_s, update_walk_seqb_w,
	update_walk_seqc_n, update_walk_seqc_e, update_walk_seqc_s, update_walk_seqc_w,
	update_walk_seqd_n, update_walk_seqd_e, update_walk_seqd_s, update_walk_seqd_w,
	update_walk_wait_n, update_walk_wait_e, update_walk_wait_s, update_walk_wait_w
};


static nil (* render [NPC_STATES]) (wanderlust_entity *, npc_property *) =
{
	render_walk_seqa_n, render_walk_seqa_e, render_walk_seqa_s, render_walk_seqa_w,
	render_walk_seqb_n, render_walk_seqb_e, render_walk_seqb_s, render_walk_seqb_w,
	render_walk_seqc_n, render_walk_seqc_e, render_walk_seqc_s, render_walk_seqc_w,
	render_walk_seqd_n, render_walk_seqd_e, render_walk_seqd_s, render_walk_seqd_w,
	render_walk_wait_n, render_walk_wait_e, render_walk_wait_s, render_walk_wait_w
};

static nil (* modify [MODIFIER_COUNT]) (wanderlust_entity *, lua_State *, u16) =
{
	modify_title,
	modify_row,
	modify_column,
	modify_direction,
	modify_texture,
	modify_destination_row,
	modify_destination_column
};

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

#define ENTITY_NPC_CREATE_ARG_COUNT 		6

#define ENTITY_NPC_CREATE_ARG_POS_TITLE 	2

#define ENTITY_NPC_CREATE_ARG_POS_TEXTURE	3

#define ENTITY_NPC_CREATE_ARG_POS_ROW		4

#define ENTITY_NPC_CREATE_ARG_POS_COLUMN	5

wanderlust_entity * entity_npc_create (lua_State * L)
{
	wanderlust_entity 	* e = 0;
	npc_property		* p = 0;

	if (lua_gettop (L) < ENTITY_NPC_CREATE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, ENTITY_NPC_CREATE_ARG_POS_TITLE))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, ENTITY_NPC_CREATE_ARG_POS_TEXTURE))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_NPC_CREATE_ARG_POS_ROW))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_NPC_CREATE_ARG_POS_COLUMN))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, NPC_CREATE_PROPERTIES_ARG_POS_SCRIPT))
	{
		goto FAILURE;
	}

	if (! (e = SDL_malloc (sizeof (wanderlust_entity))))
	{
		goto FAILURE;
	}

	e->texture 	= 0;
	e->properties = 0;
	e->title		= 0;
	e->direction	= e_configuration.direction.s;

	if (! (p = npc_create_properties (L)))
	{
		goto FAILURE;
	}

	e->properties 	= p;
	e->title		= (s08*)type_secure_copy_string
	(
		(const char*)lua_tostring (L, ENTITY_NPC_CREATE_ARG_POS_TITLE)
	);

	if (! e->title)
	{
		goto FAILURE;
	}

	SET_U32_CONST
	(
		e->hash,
		(u32) mhash (lua_tostring (L, ENTITY_NPC_CREATE_ARG_POS_TITLE))
	);

	e->texture = texture_get_by_name
	(
		lua_tostring (L, ENTITY_NPC_CREATE_ARG_POS_TEXTURE)
	);

	if (! e->texture)
	{
		goto FAILURE;
	}

	e->row		= (u08) lua_tointeger (L, ENTITY_NPC_CREATE_ARG_POS_ROW);
	e->column		= (u08) lua_tointeger (L, ENTITY_NPC_CREATE_ARG_POS_COLUMN);

	e->target.w	= e_configuration.tileset.width;
	e->target.h	= e_configuration.tileset.height;

	adjust_position (e, e->properties);

	SET_U08_CONST (e->type, 	NPC_ENTITY);
	SET_U08_CONST (e->locked,	0x00);

	setup [(p->state = NPC_WALK_SEQA_S)] (e, p);

	lua_settop (L, 0);

	return e;

	FAILURE:

		if (e)
		{
			entity_npc_delete (e);
		}

		lua_settop (L, 0);

		return e_configuration.null;
}

nil	entity_npc_delete (wanderlust_entity * e)
{
	if (! e)
	{
		return;
	}

	if (e->properties)
	{
		npc_delete_property (e->properties);
	}

	if (e->title)
	{
		SDL_free (e->title);
	}

	SDL_free (e);
}

nil	entity_npc_adjust (wanderlust_entity * e)
{
	adjust [((npc_property *) e->properties)->state] (e, e->properties);

	if (e->locked)
	{
		return;
	}

	adjust_position (e, e->properties);
}

nil entity_npc_direct (wanderlust_entity * e)
{
	npc_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	direct [p->state] (e, p);
}

nil	entity_npc_handle (wanderlust_entity * e)
{
	npc_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	handle [p->state] (e, p);
}

nil	entity_npc_update (wanderlust_entity * e)
{
	npc_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	update [p->state] (e, p);
}

nil	entity_npc_render (wanderlust_entity * e)
{
	npc_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	render [p->state] (e, p);
}

nil	entity_npc_select (wanderlust_entity * e) { }

nil	entity_npc_recall (wanderlust_entity * e) { }

nil entity_npc_excite (wanderlust_entity * e)
{
	npc_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	lua_getglobal (p->lua, "talk");

	if (lua_pcall (p->lua, 0, 0, 0) != 0)
	{
		printf ("Message from dr. light: unable to call script by %s\n", e->title);
	}
}

nil entity_npc_expire (wanderlust_entity * e)
{
	if (e)
	{
		SET_U08_CONST (e->locked, 0xFF);
	}
}

nil entity_npc_modify (wanderlust_entity * e, lua_State * L, u16 index)
{
	u08	modifier 	= 0;
	u16	m 			= lua_gettop (L);
	u16	n 			= 0;

	for (n = index; n < m; n += 2)
	{
		if (! lua_isnumber (L, n))
		{
			return;
		}

		if ((n + 1) > m)
		{
			return;
		}

		if ((modifier = lua_tointeger (L, n)) < MODIFIER_COUNT)
		{
			modify [modifier] (e, L, n + 1);
		}
	}
}

nil entity_npc_initialize ()
{
	f_sprite_sources [IDLE_NORTH].x 	= 0;
	f_sprite_sources [IDLE_NORTH].y 	= 0;
	f_sprite_sources [IDLE_NORTH].w 	= e_configuration.tileset.width;
	f_sprite_sources [IDLE_NORTH].h		= e_configuration.tileset.height;

	f_sprite_sources [WALK_NORTH_A].x 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_NORTH_A].y 	= 0;
	f_sprite_sources [WALK_NORTH_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_NORTH_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_NORTH_B].x 	= e_configuration.tileset.width * 2;
	f_sprite_sources [WALK_NORTH_B].y 	= 0;
	f_sprite_sources [WALK_NORTH_B].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_NORTH_B].h 	= e_configuration.tileset.height;

	f_sprite_sources [IDLE_EAST].x 		= 0;
	f_sprite_sources [IDLE_EAST].y 		= e_configuration.tileset.height;
	f_sprite_sources [IDLE_EAST].w 		= e_configuration.tileset.width;
	f_sprite_sources [IDLE_EAST].h 		= e_configuration.tileset.height;

	f_sprite_sources [WALK_EAST_A].x 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_EAST_A].y	= e_configuration.tileset.height;
	f_sprite_sources [WALK_EAST_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_EAST_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [IDLE_SOUTH].x 	= 0;
	f_sprite_sources [IDLE_SOUTH].y 	= e_configuration.tileset.height * 2;
	f_sprite_sources [IDLE_SOUTH].w 	= e_configuration.tileset.width;
	f_sprite_sources [IDLE_SOUTH].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_SOUTH_A].x 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_SOUTH_A].y 	= e_configuration.tileset.height * 2;
	f_sprite_sources [WALK_SOUTH_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_SOUTH_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_SOUTH_B].x 	= e_configuration.tileset.width * 2;
	f_sprite_sources [WALK_SOUTH_B].y 	= e_configuration.tileset.height * 2;
	f_sprite_sources [WALK_SOUTH_B].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_SOUTH_B].h 	= e_configuration.tileset.height;

	f_sprite_sources [IDLE_WEST].x 		= 0;
	f_sprite_sources [IDLE_WEST].y 		= e_configuration.tileset.height * 3;
	f_sprite_sources [IDLE_WEST].w 		= e_configuration.tileset.width;
	f_sprite_sources [IDLE_WEST].h 		= e_configuration.tileset.height;

	f_sprite_sources [WALK_WEST_A].x 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_WEST_A].y	= e_configuration.tileset.height * 3;
	f_sprite_sources [WALK_WEST_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_WEST_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [JUMP_SHADOW].x 	= e_configuration.tileset.width * 5;
	f_sprite_sources [JUMP_SHADOW].y	= e_configuration.tileset.height * 4;
	f_sprite_sources [JUMP_SHADOW].w 	= e_configuration.tileset.width;
	f_sprite_sources [JUMP_SHADOW].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_IDLE_N].x 	= e_configuration.tileset.width * 3;
	f_sprite_sources [RIDE_IDLE_N].y	= 0;
	f_sprite_sources [RIDE_IDLE_N].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_IDLE_N].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_N_A].x 	= e_configuration.tileset.width * 4;
	f_sprite_sources [RIDE_MOVE_N_A].y	= 0;
	f_sprite_sources [RIDE_MOVE_N_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_N_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_N_B].x 	= e_configuration.tileset.width * 5;
	f_sprite_sources [RIDE_MOVE_N_B].y	= 0;
	f_sprite_sources [RIDE_MOVE_N_B].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_N_B].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_IDLE_E].x 	= e_configuration.tileset.width * 3;
	f_sprite_sources [RIDE_IDLE_E].y	= e_configuration.tileset.height;
	f_sprite_sources [RIDE_IDLE_E].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_IDLE_E].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_E_A].x 	= e_configuration.tileset.width * 4;
	f_sprite_sources [RIDE_MOVE_E_A].y	= e_configuration.tileset.height;
	f_sprite_sources [RIDE_MOVE_E_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_E_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_IDLE_S].x 	= e_configuration.tileset.width * 3;
	f_sprite_sources [RIDE_IDLE_S].y	= e_configuration.tileset.height * 2;
	f_sprite_sources [RIDE_IDLE_S].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_IDLE_S].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_S_A].x 	= e_configuration.tileset.width * 4;
	f_sprite_sources [RIDE_MOVE_S_A].y	= e_configuration.tileset.height * 2;
	f_sprite_sources [RIDE_MOVE_S_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_S_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_S_B].x 	= e_configuration.tileset.width * 5;
	f_sprite_sources [RIDE_MOVE_S_B].y	= e_configuration.tileset.height * 2;
	f_sprite_sources [RIDE_MOVE_S_B].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_S_B].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_IDLE_W].x 	= e_configuration.tileset.width * 3;
	f_sprite_sources [RIDE_IDLE_W].y	= e_configuration.tileset.height * 3;
	f_sprite_sources [RIDE_IDLE_W].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_IDLE_W].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_W_A].x 	= e_configuration.tileset.width * 4;
	f_sprite_sources [RIDE_MOVE_W_A].y	= e_configuration.tileset.height * 3;
	f_sprite_sources [RIDE_MOVE_W_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_W_A].h 	= e_configuration.tileset.height;
}

nil entity_npc_terminate ()
{

}
