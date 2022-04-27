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
#include "entity.h"
#include "texture.h"
#include "morph.h"
#include "map.h"
#include "task.h"
#include "font.h"
#include "textbox.h"
#include "property.h"
#include "menu.h"
#include "video.h"
#include "config.h"

#include "state.h"
#include "state.menu.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

#define STATE_MENU_CREATE_ARG_COUNT			2
#define STATE_MENU_CREATE_ARG_POS_SCRIPT	2

wanderlust_state * state_menu_create (lua_State * L)
{
	wanderlust_state * s = e_configuration.null;

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < STATE_MENU_CREATE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, STATE_MENU_CREATE_ARG_POS_SCRIPT))
	{
		goto FAILURE;
	}

	if (! (s = SDL_malloc (sizeof (wanderlust_state))))
	{
		goto FAILURE;
	}

	s->background 	= e_configuration.null;
	s->lua			= e_configuration.null;
	s->entities		= e_configuration.null;

	if (! (s -> entities = mcreate (e_configuration.morph.doublelinkedlist)))
	{
		goto FAILURE;
	}

	SET_U08_CONST (s -> type, 	MENU_STATE);
	SET_U08_CONST (s -> locked,	0xFF);

	if (! (s -> lua = lua_open ()))
	{
		goto FAILURE;
	}

	if (luaL_loadfile (s -> lua, lua_tostring (L, STATE_MENU_CREATE_ARG_POS_SCRIPT)))
	{
		goto FAILURE;
	}

	lua_register (s -> lua, WL_LOAD_IMAGE,					texture_load_image);
	lua_register (s -> lua, WL_RUNDOWN_TASK,				task_rundown);
	lua_register (s -> lua, WL_LOAD_MAP,					map_load_segment);
	lua_register (s -> lua, WL_SELECT_MAP,					map_select_segment);
	lua_register (s -> lua, WL_ENTER_STATE,					state_enter);
	lua_register (s -> lua, WL_LEAVE_STATE,					state_leave);
	lua_register (s -> lua, WL_ENTER_STATE_BY_TRANSITION,	state_enter_by_transition);
	lua_register (s -> lua, WL_ENTER_STATE_BY_TRANSITION,	state_leave_by_transition);
	lua_register (s -> lua, WL_FETCH_STATE,					state_fetch);
	lua_register (s -> lua, WL_CLEAR_STATE,					state_clear);
	lua_register (s -> lua, WL_CREATE_ENTITY,				entity_enqueue);
	lua_register (s -> lua, WL_SELECT_STATE_ENTITY,			state_entity_select);
	lua_register (s -> lua, WL_CREATE_FONT, 				font_create);
	lua_register (s -> lua, WL_CREATE_TRANSITION,			video_enqueue_transition);
	lua_register (s -> lua, WL_CREATE_MENU,					menu_create);
	lua_register (s -> lua, WL_SELECT_MENU_ENTITY,			menu_select);
	lua_register (s -> lua, WL_REPEAL_MENU,					menu_repeal);
	lua_register (s -> lua, WL_APPEND_MENU,					menu_append);
	lua_register (s -> lua, WL_GET_PROPERTY, 				property_get_wanderlust_property);
	lua_register (s -> lua, WL_SET_PROPERTY,				property_set_wanderlust_property);
	lua_register (s -> lua, WL_SET_BACKGROUND, 				state_set_background);

	lua_pop (L, lua_gettop (L));

	lua_settop (L, 0);

	return s;

	FAILURE:

		if (s)
		{
			state_menu_delete (s);
		}

		lua_settop (L, 0);

		return e_configuration.null;
}

nil state_menu_direct (wanderlust_state * s)
{
	if (msize (s -> entities))
	{
		mbatch (s -> entities, entity_direct, 0);
	}
}

nil	state_menu_delete (wanderlust_state * s)
{
	if (! s)
	{
		return;
	}

	if (s -> entities)
	{
		s -> entities = mdelete (s -> entities, entity_delete);
	}

	if (s -> lua)
	{
		lua_close (s -> lua);
	}

	SDL_free (s);
}

nil	state_menu_handle (wanderlust_state * s)
{
	if (msize (s -> entities))
	{
		mbatch (s -> entities, entity_handle, 0);
	}
}

nil	state_menu_update (wanderlust_state * s)
{
	if (msize (s -> entities))
	{
		mbatch (s -> entities, entity_update, 0);
	}
}

nil	state_menu_render (wanderlust_state * s)
{
	if (s -> background)
	{
		video_draw_background (s -> background);
	}

	if (msize (s -> entities))
	{
		mbatch (s -> entities, entity_render, 0);
	}
}

nil	state_menu_adjust (wanderlust_state * s)
{
	if (msize (s -> entities))
	{
		mbatch (s -> entities, entity_adjust, 0);
	}
}
