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
#include "task.h"
#include "entity.h"
#include "morph.h"
#include "transition.h"
#include "video.h"
#include "textbox.h"
#include "texture.h"
#include "menu.h"
#include "state.h"
#include "terminal.h"

#include "state.game.h"
#include "state.menu.h"
#include "state.grid.h"
#include "state.list.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

static s08		f_initialized	= WANDERLUST_FAILURE;

static nil *	f_states		= 0;

/* ************************************************************************
 *
 * BANKS
 *
 * ***********************************************************************/

static wanderlust_state * (* create_state [STATE_TYPE_COUNT]) (lua_State *) =
{
	state_game_create,
	state_menu_create,
	state_grid_create,
	state_list_create
};

static nil (* delete_state [STATE_TYPE_COUNT]) (wanderlust_state *) =
{
	state_game_delete,
	state_menu_delete,
	state_grid_delete,
	state_list_delete
};

static nil (* direct_state [STATE_TYPE_COUNT]) (wanderlust_state *) =
{
	state_game_direct,
	state_menu_direct,
	state_grid_direct,
	state_list_direct
};

static nil (* handle_state [STATE_TYPE_COUNT]) (wanderlust_state *) =
{
	state_game_handle,
	state_menu_handle,
	state_grid_handle,
	state_list_handle,
};

static nil (* update_state [STATE_TYPE_COUNT]) (wanderlust_state *) =
{
	state_game_update,
	state_menu_update,
	state_grid_update,
	state_list_update,
};

static nil (* render_state [STATE_TYPE_COUNT]) (wanderlust_state *) =
{
	state_game_render,
	state_menu_render,
	state_grid_render,
	state_list_render,
};

static nil (* adjust_state [STATE_TYPE_COUNT]) (wanderlust_state *) =
{
	state_game_adjust,
	state_menu_adjust,
	state_grid_adjust,
	state_list_adjust,
};

/* ************************************************************************
 *
 * COMPARATOR
 *
 * ***********************************************************************/

static u08 compare_entity_by_hash (nil * item, nil * value)
{
	if (! item)
	{
		goto FAILURE;
	}

	if (! value)
	{
		goto FAILURE;
	}

	return ((wanderlust_entity *) item)->hash == *((u32 *)value);

	FAILURE:

		return e_configuration.zero;
}

static u08 compare_entities_by_hash (nil * item, nil * value)
{
	wanderlust_entity * u = item;
	wanderlust_entity * v = value;

	if (! u)
	{
		goto FAILURE;
	}

	if (! v)
	{
		goto FAILURE;
	}

	return (u->hash == v->hash);

	FAILURE:

		return e_configuration.zero;
}

static u08 compare_entity_position_by_point (nil * item, nil * value)
{
	wanderlust_entity 	* e = item;
	SDL_Point 			* p = value;

	if (! e)
	{
		goto FAILURE;
	}

	if (! p)
	{
		goto FAILURE;
	}

	return (e->row == p->y) && (e->column == p->x);

	FAILURE:

		return e_configuration.zero;
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ***********************************************************************/

#define CREATE_WANDERLUST_STATE_ARG_COUNT		2
#define CREATE_WANDERLUST_STATE_ARG_POS_TYPE	1

static wanderlust_state * create_wanderlust_state (lua_State * L)
{
	u08 type = lua_gettop (L);

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < CREATE_WANDERLUST_STATE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, CREATE_WANDERLUST_STATE_ARG_POS_TYPE))
	{
		goto FAILURE;
	}

	type = lua_tointeger (L, CREATE_WANDERLUST_STATE_ARG_POS_TYPE);

	if (type >= STATE_TYPE_COUNT)
	{
		goto FAILURE;
	}

	return create_state [type] (L);

	FAILURE:

		return e_configuration.null;
}

static nil delete_wanderlust_state (nil * item)
{
	wanderlust_state * s = item;

	if (! item)
	{
		return;
	}

	delete_state [s->type] (s);
}

/* ************************************************************************
 *
 * TASKS
 *
 * ***********************************************************************/

static nil task_state_enter (nil * item)
{
	wanderlust_state * s = item;

	if (! s)
	{
		return;
	}

	SET_U08_CONST (s->locked, 0x00);

	mpush (f_states, item);

	if (s->lua)
	{
		if (lua_pcall (s->lua, 0, 0, 0))
		{
			SET_U08_CONST (s->locked, 0xFF);
		}
	}
}

static nil task_state_leave (nil * item)
{
	delete_wanderlust_state (mpop (f_states));

	if (! msize (f_states))
	{
		task_enqueue (TASK_ENGINE_HALT, 0);
	}
}

static nil task_state_fetch (nil * item)
{
	wanderlust_state * s = item;

	if (! s)
	{
		return;
	}

	entity_fetch (s->entities);
}

static nil task_state_clear (nil * item)
{
	wanderlust_state * s = item;

	if (! s)
	{
		return;
	}

	mclear (s->entities, entity_delete);
}

static nil task_state_entity_select (nil * item)
{
	wanderlust_state	* 	s = mpeek (f_states, 0);
	wanderlust_entity 	* 	e = item;

	if (! e)
	{
		return;
	}

	if (! s)
	{
		return;
	}

	mbatch (s->entities, entity_select, (nil*)&e->hash);
}

static nil task_state_entity_remove (nil * item)
{
	wanderlust_state	* 	s = mpeek (f_states, 0);
	wanderlust_entity 	* 	e = item;

	u32						h = 0;

	if (! e)
	{
		return;
	}

	if (! s)
	{
		return;
	}

	h = e->hash;

	mremove (s->entities, & h, compare_entity_by_hash, entity_delete);
}

static nil task_state_set_background (nil * item)
{
	wanderlust_state	* 	s = mpeek (f_states, 0);
	wanderlust_entity 	* 	e = item;

	if (! e)
	{
		return;
	}

	if (! s)
	{
		return;
	}

	s->background = item;
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 state_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized = e_configuration.success;

	if (! L)
	{
		goto FAILURE;
	}

	if (! (f_states = mcreate (e_configuration.morph.stack)))
	{
		goto FAILURE;
	}

	lua_register (L, WL_ENTER_STATE, state_enter);
	lua_register (L, WL_LEAVE_STATE, state_leave);
	lua_register (L, WL_ENTER_STATE_BY_TRANSITION, state_enter_by_transition);
	lua_register (L, WL_LEAVE_STATE_BY_TRANSITION, state_leave_by_transition);
	lua_register (L, WL_FETCH_STATE, state_fetch);
	lua_register (L, WL_CLEAR_STATE, state_clear);
	lua_register (L, WL_CREATE_TRANSITION, video_enqueue_transition);
	lua_register (L, WL_SET_BACKGROUND, state_set_background);
	lua_register (L, WL_RUNDOWN_TASK, task_rundown);

	task_register (TASK_STATE_ENTER, task_state_enter);
	task_register (TASK_STATE_LEAVE, task_state_leave);
	task_register (TASK_STATE_FETCH, task_state_fetch);
	task_register (TASK_STATE_CLEAR, task_state_clear);
	task_register (TASK_STATE_SELECT, task_state_entity_select);
	task_register (TASK_STATE_REMOVE, task_state_entity_remove);
	task_register (TASK_STATE_SET_BG, task_state_set_background);

	return e_configuration.success;

	FAILURE:

		state_terminate ();

		return e_configuration.failure;
}

nil state_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	f_states		= mdelete (f_states, delete_wanderlust_state);
	f_initialized	= e_configuration.failure;
}

s32 state_enter (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_STATE_ENTER, create_wanderlust_state (L));

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32	state_leave (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_STATE_LEAVE, 0);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define STATE_ENTER_BY_TRANSITION_ARG_COUNT			4
#define STATE_ENTER_BY_TRANSITION_ARG_POS_TYPE		1
#define STATE_ENTER_BY_TRANSITION_ARG_POS_SCRIPT	2
#define STATE_ENTER_BY_TRANSITION_ARG_POS_TEXTURE	3
#define STATE_ENTER_BY_TRANSITION_ARG_POS_DURATION	4

s32 state_enter_by_transition (lua_State * L)
{
	wanderlust_state 	* 	s = 0;
	SDL_Texture			* 	t = 0;
	u08						d = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < STATE_ENTER_BY_TRANSITION_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, STATE_ENTER_BY_TRANSITION_ARG_POS_TYPE))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, STATE_ENTER_BY_TRANSITION_ARG_POS_SCRIPT))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, STATE_ENTER_BY_TRANSITION_ARG_POS_TEXTURE))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, STATE_ENTER_BY_TRANSITION_ARG_POS_DURATION))
	{
		goto FAILURE;
	}

	t = texture_get_by_name
	(
		lua_tostring (L, STATE_ENTER_BY_TRANSITION_ARG_POS_TEXTURE)
	);

	if (! t)
	{
		goto FAILURE;
	}

	d = (u08) lua_tointeger (L, STATE_ENTER_BY_TRANSITION_ARG_POS_DURATION);

	if (! (s = create_wanderlust_state (L)))
	{
		goto FAILURE;
	}

	transition_enqueue (d, t, task_state_enter, s);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define STATE_LEAVE_BY_TRANSITION_ARG_COUNT			2
#define STATE_LEAVE_BY_TRANSITION_ARG_POS_TEXTURE	1
#define STATE_LEAVE_BY_TRANSITION_ARG_POS_DURATION	2

s32 state_leave_by_transition (lua_State * L)
{
	SDL_Texture	* 	t = 0;
	u08				d = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < STATE_LEAVE_BY_TRANSITION_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, STATE_LEAVE_BY_TRANSITION_ARG_POS_TEXTURE))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, STATE_LEAVE_BY_TRANSITION_ARG_POS_DURATION))
	{
		goto FAILURE;
	}

	t = texture_get_by_name
	(
		lua_tostring (L, STATE_LEAVE_BY_TRANSITION_ARG_POS_TEXTURE)
	);

	if (! t)
	{
		goto FAILURE;
	}

	d = (u08) lua_tointeger (L, STATE_LEAVE_BY_TRANSITION_ARG_POS_DURATION);

	transition_enqueue (d, t, task_state_leave, 0);

	lua_settop (L, 0);

	return e_configuration.failure;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32 state_fetch (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_STATE_FETCH, mpeek (f_states, 0));

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32 state_clear (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_STATE_FETCH, mpeek (f_states, 0));

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define STATE_ENTITY_SELECT_ARG_COUNT		1
#define STATE_ENTITY_SELECT_ARG_POS_NAME	1

s32	state_entity_select (lua_State * L)
{
	wanderlust_state	*	s = 0;
	wanderlust_entity	*	e = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < STATE_ENTITY_SELECT_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, STATE_ENTITY_SELECT_ARG_POS_NAME))
	{
		goto FAILURE;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		goto FAILURE;
	}

	h = mhash (lua_tostring (L, STATE_ENTITY_SELECT_ARG_POS_NAME));
	e = mfind (s->entities, &h, compare_entity_by_hash, 0);

	if (! e)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_STATE_SELECT, e);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define STATE_ENTITY_REMOVE_ARG_COUNT		1
#define STATE_ENTITY_REMOVE_ARG_POS_NAME	1

s32	state_entity_remove (lua_State * L)
{
	wanderlust_state	*	s = 0;
	wanderlust_entity	*	e = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < STATE_ENTITY_REMOVE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, STATE_ENTITY_REMOVE_ARG_POS_NAME))
	{
		goto FAILURE;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		goto FAILURE;
	}

	h = mhash (lua_tostring (L, STATE_ENTITY_REMOVE_ARG_POS_NAME));
	e = mfind (s->entities, &h, compare_entity_by_hash, 0);

	if (! e)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_STATE_REMOVE, e);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define STATE_ENTITY_EXPIRE_ARG_COUNT		1
#define STATE_ENTITY_EXPIRE_ARG_POS_NAME	1

s32	state_entity_expire (lua_State * L)
{
	wanderlust_state	*	s = 0;
	wanderlust_entity	*	e = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < STATE_ENTITY_EXPIRE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, STATE_ENTITY_EXPIRE_ARG_POS_NAME))
	{
		goto FAILURE;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		goto FAILURE;;
	}

	h = mhash (lua_tostring (L, STATE_ENTITY_EXPIRE_ARG_POS_NAME));

	if (! (e = mfind (s->entities, &h, compare_entity_by_hash, 0)))
	{
		goto FAILURE;
	}

	task_enqueue (TASK_ENTITY_EXPIRE, e);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define STATE_ENTITY_MODIFY_ARG_COUNT_MIN	3
#define STATE_ENTITY_MODIFY_ARG_POS_NAME	1
#define STATE_ENTITY_MODIFY_ARG_OFFSET		2

s32	state_entity_modify (lua_State * L)
{
	wanderlust_state	*	s = 0;
	wanderlust_entity	*	e = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < STATE_ENTITY_MODIFY_ARG_COUNT_MIN)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, STATE_ENTITY_MODIFY_ARG_POS_NAME))
	{
		goto FAILURE;
	}

	h = mhash (lua_tostring (L, STATE_ENTITY_MODIFY_ARG_POS_NAME));

	if (! (e = mfind (s->entities, &h, compare_entity_by_hash, 0)))
	{
		goto FAILURE;
	}

	entity_modify (e, L, STATE_ENTITY_MODIFY_ARG_OFFSET);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32	state_entity_modify_prior_transition (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	lua_settop (L, 0);

	return state_entity_modify (L);

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32	state_entity_modify_after_transition (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	lua_settop (L, 0);

	return state_entity_modify (L);

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define STATE_ENTITY_BY_HASH_GET_DIRECTION_ARG_COUNT	1
#define STATE_ENTITY_BY_HASH_GET_DIRECTION_ARG_POS_HASH	1

s32	state_entity_by_hash_get_direction (lua_State * L)
{
	wanderlust_state 	* 	s = 0;
	wanderlust_entity	* 	e = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < STATE_ENTITY_BY_HASH_GET_DIRECTION_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, STATE_ENTITY_BY_HASH_GET_DIRECTION_ARG_POS_HASH))
	{
		goto FAILURE;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		goto FAILURE;
	}

	h = lua_tointeger (L, STATE_ENTITY_BY_HASH_GET_DIRECTION_ARG_POS_HASH);

	if(! (e = mfind (s->entities, &h, compare_entity_by_hash, 0)))
	{
		goto FAILURE;
	}

	lua_pushnumber (L, e->direction);

	return e_configuration.failure;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define ENTITY_BY_HASH_GET_ROW_ARG_COUNT	1
#define ENTITY_BY_HASH_GET_ROW_ARG_POS_HASH	1

s32	state_entity_by_hash_get_row (lua_State * L)
{
	wanderlust_state 	* 	s = 0;
	wanderlust_entity	* 	e = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < ENTITY_BY_HASH_GET_ROW_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_BY_HASH_GET_ROW_ARG_POS_HASH))
	{
		goto FAILURE;
	}

	h = lua_tointeger (L, ENTITY_BY_HASH_GET_ROW_ARG_POS_HASH);

	if (! (e = mfind (s->entities, &h, compare_entity_by_hash, 0)))
	{
		goto FAILURE;
	}

	lua_pushnumber(L, e->row);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define ENTITY_BY_HASH_GET_COLUMN_ARG_COUNT	1
#define ENTITY_BY_HASH_GET_COLUMN_ARG_POS_HASH	1

s32 state_entity_by_hash_get_column (lua_State * L)
{
	wanderlust_state 	* 	s = 0;
	wanderlust_entity	* 	e = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < ENTITY_BY_HASH_GET_COLUMN_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_BY_HASH_GET_COLUMN_ARG_POS_HASH))
	{
		goto FAILURE;
	}

	h = lua_tointeger (L, ENTITY_BY_HASH_GET_COLUMN_ARG_POS_HASH);

	if (! (e = mfind (s->entities, &h, compare_entity_by_hash, 0)))
	{
		goto FAILURE;
	}

	lua_pushnumber(L, e->column);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define STATE_SET_BACKGROUND_ARG_COUNT			1
#define STATE_SET_BACKGROUND_ARG_POS_TEXTURE	1

s32 state_set_background (lua_State * L)
{
	wanderlust_state	* s = 0;
	SDL_Texture			* t = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < STATE_SET_BACKGROUND_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, STATE_SET_BACKGROUND_ARG_POS_TEXTURE))
	{
		goto FAILURE;
	}

	t = texture_get_by_name (lua_tostring (L, STATE_SET_BACKGROUND_ARG_POS_TEXTURE));

	if (! t)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_STATE_SET_BG, t);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		if (L)
		{
			lua_settop (L, 0);
		}

		return e_configuration.failure;
}

u08	state_entity_has_collision (SDL_Point * p)
{
	wanderlust_state * s = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! p)
	{
		goto FAILURE;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		goto FAILURE;
	}

	return (mfind (s->entities, p, compare_entity_position_by_point, 0) != 0);

	FAILURE:

		return e_configuration.failure;
}

nil state_entity_excite (SDL_Point * p)
{
	wanderlust_state 	* s = 0;
	wanderlust_entity	* e = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! p)
	{
		return;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		return;
	}

	if (! (e = mfind (s->entities, p, compare_entity_position_by_point, 0)))
	{
		return;
	}

	entity_excite (e, 0);
}

nil state_direct ()
{
	wanderlust_state * s = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		return;
	}

	if (s->locked)
	{
		return;
	}

	direct_state [s->type] (s);
}

nil state_handle ()
{
	wanderlust_state * s = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		return;
	}

	if (s->locked)
	{
		return;
	}

	if (textbox_pending ())
	{
		textbox_handle ();
	}
	else
	{
		if (menu_active ())
		{
			menu_handle ();
		}
		else
		{

			if (terminal_active ())
			{
				terminal_handle ();
			}
			else
			{
				handle_state [s->type] (s);
			}
		}
	}
}

nil state_update ()
{
	wanderlust_state * s = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		return;
	}

	if (s->locked)
	{
		return;
	}

	if (textbox_pending ())
	{
		textbox_update ();
	}
	else
	{
		if (menu_active ())
		{
			menu_update ();

			if (terminal_active ())
			{
				terminal_update ();
			}
		}
		else
		{

			if (terminal_active ())
			{
				terminal_update ();
			}
			else
			{
				update_state [s->type] (s);
			}
		}
	}
}

nil state_render ()
{
	wanderlust_state * s = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		return;
	}

	if (s->locked)
	{
		return;
	}

	render_state [s->type] (s);

	if (terminal_active ())
	{
		terminal_render ();
	}

	if (menu_active ())
	{
		menu_render ();
	}

	if (textbox_pending ())
	{
		textbox_render ();
	}
}

nil state_adjust ()
{
	wanderlust_state * s = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		return;
	}

	if (s->locked)
	{
		return;
	}

	adjust_state [s->type] (s);
}

nil * state_get_successor (nil * arg)
{
	wanderlust_state * s = 0;

	if (f_initialized == e_configuration.failure)
	{
		return e_configuration.null;
	}

	if (! arg)
	{
		return e_configuration.null;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		return e_configuration.null;
	}

	if (! msize (s->entities))
	{
		return e_configuration.null;
	}

	return msuccessor (s->entities, arg, compare_entities_by_hash);
}

nil * state_get_predecessor (nil * arg)
{
	wanderlust_state * s = 0;

	if (f_initialized == e_configuration.failure)
	{
		return e_configuration.null;
	}

	if (! arg)
	{
		return e_configuration.null;
	}

	if (! (s = mpeek (f_states, 0)))
	{
		return e_configuration.null;
	}

	if (! msize (s->entities))
	{
		return e_configuration.null;
	}

	return mpredecessor (s->entities, arg, compare_entities_by_hash);
}
