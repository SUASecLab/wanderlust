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
#include "map.h"
#include "task.h"
#include "morph.h"
#include "textbox.h"
#include "property.h"
#include "core.h"
#include "assembler.h"

#include "entity.primitive.h"

#include "entity.npc.h"

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
 * ************************************************************************/

typedef enum
{
	PRIMITIVE_IDLE,
	PRIMITIVE_FADE,
	PRIMITIVE_STATES
}
primitive_state;

typedef struct
{
	u08				state;
	u08				steps;
	lua_State 	* 	lua;
	s08			* 	callback;
}
primitive_property;

typedef enum
{
	MODIFY_TITLE,
	MODIFY_ROW,
	MODIFY_COLUMN,
	MODIFY_DIRECTION,
	MODIFY_TEXTURE,
	MODIFIER_COUNT
}
primitive_modifier;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - SETUP
 *
 * ************************************************************************/

static nil setup_idle (wanderlust_entity * e, primitive_property * p)
{
	e->source->x 	= 0;
	e->source->y 	= 0;
	p->steps			= 0;
}

static nil setup_fade (wanderlust_entity * e, primitive_property * p)
{
	e->source->x 	= 0;
	e->source->y 	= 0;
	p->steps			= e_configuration.tileset.width;
}

/* ************************************************************************
 *
 * BANK - SETUP
 *
 * ************************************************************************/

static nil (* setup [PRIMITIVE_STATES]) (wanderlust_entity *, primitive_property *) =
{
	setup_idle, setup_fade
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

	if (! (title = (s08*)type_secure_copy_string (lua_tostring (L, index))))
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

static nil modify_direction (wanderlust_entity * e, lua_State * L, u16 index)
{
	u08 direction = 0;

	if (! lua_isnumber (L, index))
	{
		return;
	}

	if ((direction = (u08) lua_tointeger (L, index)) < WANDERLUST_DIRECTIONS)
	{
		e->direction = direction;
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
 * FILE SCOPE FUNCTIONS - UPDATE
 *
 * ************************************************************************/

static nil update_idle (wanderlust_entity * e, primitive_property * p) { }

static nil update_fade (wanderlust_entity * e, primitive_property * p)
{
	if (p->steps)
	{
		-- p->steps; e->source->x += e_configuration.tileset.width;
	}
	else
	{
		task_enqueue (TASK_STATE_REMOVE, e);
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - HELPING HANDS
 *
 * ************************************************************************/

static nil primitive_delete_property (primitive_property * p)
{
	if (! p)
	{
		return;
	}

	if (p->callback)
	{
		SDL_free (p->callback);
	}

	if (p->lua)
	{
		lua_close (p->lua);
	}

	SDL_free (p);
}

#define PRIMTIVE_CREATE_PROPERTY_ARG_POS_SCRIPT		10
#define PRIMTIVE_CREATE_PROPERTY_ARG_POS_CALLBACK	11

static primitive_property * primitive_create_property (lua_State * L)
{
	primitive_property * p = 0;

	if (! L)
	{
		goto FAILURE;
	}

	if (! (p = SDL_malloc (sizeof (primitive_property))))
	{
		goto FAILURE;
	}

	p->lua		= 0;
	p->callback	= 0;
	p->steps	= 0;
	p->state	= 0;

	if (! (p->lua = lua_open ()))
	{
		goto FAILURE;
	}

	luaL_openlibs (p->lua);

	if (luaL_loadfile (p->lua, lua_tostring (L, PRIMTIVE_CREATE_PROPERTY_ARG_POS_SCRIPT)))
	{
		goto FAILURE;
	}

	lua_register (p->lua, WL_CREATE_ENTITY,			entity_enqueue);
	lua_register (p->lua, WL_SELECT_STATE_ENTITY,	state_entity_select);
	lua_register (p->lua, WL_REMOVE_STATE_ENTITY,	state_entity_remove);
	lua_register (p->lua, WL_EXPIRE_STATE_ENTITY,	state_entity_expire);
	lua_register (p->lua, WL_MODIFY_STATE_ENTITY,	state_entity_modify);
	lua_register (p->lua, WL_CREATE_TEXTBOX,		textbox_enqueue);
	lua_register (p->lua, WL_GET_PROPERTY, 			property_get_wanderlust_property);
	lua_register (p->lua, WL_SET_PROPERTY,			property_set_wanderlust_property);
	lua_register (
		p->lua,
		WL_CORE_GET_OUTPUT,
		core_get_output
	);
	lua_register (
		p->lua,
		WL_ASSEMBLER_LOAD_ROM,
		assembler_load_rom
	);


	if (lua_pcall (p->lua, 0, 0, 0))
	{
		goto FAILURE;
	}

	p->callback = (s08*)type_secure_copy_string
	(
		lua_tostring (L, PRIMTIVE_CREATE_PROPERTY_ARG_POS_CALLBACK)
	);

	if (! p->callback)
	{
		goto FAILURE;
	}

	return p;

	FAILURE:

		primitive_delete_property (p);

		return e_configuration.null;
}

static nil primitive_adjust_position (wanderlust_entity * e)
{
	e->target.x = e_configuration.tileset.width * (e->column - map_get_area_column ());
	e->target.x = e->target.x + map_get_origin_x ();
	e->target.y = e_configuration.tileset.height * (e->row - map_get_area_row ());
	e->target.y = e->target.y + map_get_origin_y ();
}

/* ************************************************************************
 *
 * BANKS - UPDATE, MODIFY
 *
 * ************************************************************************/

static nil (* update [PRIMITIVE_STATES]) (wanderlust_entity *, primitive_property *) =
{
	update_idle, update_fade
};

static nil (* modify [MODIFIER_COUNT]) (wanderlust_entity *, lua_State *, u16) =
{
	modify_title,
	modify_row,
	modify_column,
	modify_direction,
	modify_texture
};

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

#define ENTITY_PRIMITIVE_CREATE_ARG_COUNT		11
#define ENTITY_PRIMITIVE_CREATE_ARG_POS_TITLE	2
#define ENTITY_PRIMITIVE_CREATE_ARG_POS_TEXTURE	3
#define ENTITY_PRIMITIVE_CREATE_ARG_POS_COLUMN	4
#define ENTITY_PRIMITIVE_CREATE_ARG_POS_ROW		5
#define ENTITY_PRIMITIVE_CREATE_ARG_POS_X		6
#define ENTITY_PRIMITIVE_CREATE_ARG_POS_Y		7
#define ENTITY_PRIMITIVE_CREATE_ARG_POS_W		8
#define ENTITY_PRIMITIVE_CREATE_ARG_POS_H		9

wanderlust_entity * entity_primitive_create (lua_State * L)
{
	wanderlust_entity	* e = 0;
	primitive_property	* p = 0;

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < ENTITY_PRIMITIVE_CREATE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_TITLE))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_TEXTURE))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_COLUMN))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_ROW))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_X))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_Y))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_W))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_H))
	{
		goto FAILURE;
	}

	if (! (e = SDL_malloc (sizeof (wanderlust_entity))))
	{
		goto FAILURE;
	}

	e->properties	= 0;
	e->source		= 0;
	e->title		= (s08*)type_secure_copy_string
	(
		lua_tostring (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_TITLE)
	);

	if (! e->title)
	{
		goto FAILURE;
	}

	SET_U32_CONST
	(
		e->hash,
		(u32) mhash (lua_tostring (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_TITLE))
	);

	e->texture = texture_get_by_name
	(
		lua_tostring (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_TEXTURE)
	);

	if (! e->texture)
	{
		goto FAILURE;
	}

	if (! (e->source = SDL_malloc (sizeof (SDL_Rect))))
	{
		goto FAILURE;
	}

	e->column 		= lua_tointeger (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_COLUMN);
	e->row			= lua_tointeger (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_ROW);
	e->source->x	= lua_tointeger (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_X);
	e->source->y	= lua_tointeger (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_Y);
	e->source->w	= lua_tointeger (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_W);
	e->source->h	= lua_tointeger (L, ENTITY_PRIMITIVE_CREATE_ARG_POS_H);
	e->target.w		= e->source->w;
	e->target.h		= e->source->h;
	e->direction	= e_configuration.direction.s;

	primitive_adjust_position (e);

	if (! (p = primitive_create_property (L)))
	{
		goto FAILURE;
	}

	setup [(p->state = PRIMITIVE_IDLE)] (e, p); e->properties = p;

	SET_U08_CONST (e->type, 	PRIMITIVE_ENTITY);
	SET_U08_CONST (e->locked,	0x00);

	lua_settop (L, 0);

	return e;

	FAILURE:

		if (e)
		{
			entity_primitive_delete (e);
		}

		lua_settop (L, 0);

		return e_configuration.null;
}

nil	entity_primitive_direct (wanderlust_entity * e)
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

nil entity_primitive_delete (wanderlust_entity * e)
{
	if (! e)
	{
		return;
	}

	if (e->title)
	{
		SDL_free (e->title);
	}

	if (e->properties)
	{
		primitive_delete_property (e->properties);
	}

	if (e->source)
	{
		SDL_free (e->source);
	}

	SDL_free (e);
}

nil entity_primitive_handle (wanderlust_entity * e) { }

nil entity_primitive_update (wanderlust_entity * e)
{
	primitive_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	update [p->state] (e, p);
}

nil entity_primitive_render (wanderlust_entity * e)
{
	if (e->locked)
	{
		return;
	}

	video_draw_entity (e);
}

nil entity_primitive_adjust (wanderlust_entity * e)
{
	primitive_adjust_position (e);
}

nil entity_primitive_select (wanderlust_entity * e) { }

nil entity_primitive_recall (wanderlust_entity * e) { }

nil	entity_primitive_excite (wanderlust_entity * e)
{
	primitive_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	lua_getglobal (
		p->lua, (const char*)p->callback
	);
	
	lua_pushstring (
		p->lua, (const char*)e->title
	);

	lua_pcall (p->lua, 1, 0, 0);
}

nil entity_primitive_expire (wanderlust_entity * e)
{
	primitive_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	setup [(p->state = PRIMITIVE_FADE)] (e, p);
}

nil entity_primitive_modify (wanderlust_entity * e, lua_State * L, u16 index)
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

nil	entity_primitive_initialize () { }

nil	entity_primitive_terminate () { }
