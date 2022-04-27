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
#include "font.h"
#include "frame.h"
#include "entity.h"
#include "morph.h"
#include "texture.h"
#include "task.h"
#include "video.h"

#include "menu.h"

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

typedef struct
{
	const	u32 					hash;
			nil					*	entities;
			wanderlust_frame	*	frame;
}
wanderlust_menu;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

static s08 		f_initialized 	= WANDERLUST_FAILURE;

static nil	*	f_menus			= 0;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - ADJUST
 *
 * ***********************************************************************/

static nil adjust_rectangle_w (SDL_Rect * src, SDL_Rect * dst)
{
	if (! src)
	{
		return;
	}

	if (! dst)
	{
		return;
	}

	if ((dst -> x + dst -> w) < (src -> x + src -> w))
	{
		dst -> w += ((src -> x + src -> w) - (dst -> x + dst -> w));
	}
}

static nil adjust_rectangle_h (SDL_Rect * src, SDL_Rect * dst)
{
	if (! src)
	{
		return;
	}

	if (! dst)
	{
		return;
	}

	if ((dst -> y + dst -> h) < (src -> y + dst -> h))
	{
		dst -> h += ((src -> y + src -> h) - (dst -> y + dst -> h));
	}
}

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

	return ((wanderlust_entity *) item) -> hash == *((u32 *)value);

	FAILURE:
		return e_configuration.zero;
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - BATCH
 *
 * ***********************************************************************/

static nil batch_process_region (nil * item, nil * value)
{
	wanderlust_entity	* e = item;
	SDL_Rect 			* r = value;

	if (! e)
	{
		return;
	}

	if (! r)
	{
		return;
	}

	if (r -> x > e -> target.x)
	{
		r -> x = e -> target.x; adjust_rectangle_w (& e -> target, r);
	}
	else
	{
		if (r -> x == e -> target.x)
		{
			if (r -> w < e -> target.w)
			{
				r -> w = e -> target.w;
			}
		}
		else
		{
			adjust_rectangle_w (& e -> target, r);
		}
	}

	if (r -> y > e -> target.y)
	{
		r -> y = e -> target.y; adjust_rectangle_h (& e -> target, r);
	}
	else
	{
		if (r -> y == e -> target.y)
		{
			if (r -> h < e -> target.h)
			{
				r -> h = e -> target.h;
			}
		}
		else
		{
			adjust_rectangle_h (& e -> target, r);
		}
	}
}

static nil batch_render_wanderlust_menu (nil * item, nil * value)
{
	wanderlust_menu * m = item;

	if (! m)
	{
		return;
	}

	video_draw_frame (m -> frame);

	if (msize (m -> entities))
	{
		mbatch (m -> entities, entity_render, 0);
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ***********************************************************************/

static nil menu_delete_wanderlust_menu (nil * item)
{
	wanderlust_menu * m = item;

	if (! m)
	{
		return;
	}

	if (m -> entities)
	{
		m -> entities = mdelete (m -> entities, entity_delete);
	}

	if (m -> frame)
	{
		frame_delete_wanderlust_frame (m -> frame);
	}

	SDL_free (m);
}

#define MENU_CREATE_WANDERLUST_MENU_ARGUMENT_COUNT 			4
#define MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_TITLE		1
#define MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_X			2
#define MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_Y			3
#define MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_TEXTURE 	4
#define MENU_CREATE_WANDERLUST_MENU_ROWS					3
#define MENU_CREATE_WANDERLUST_MENU_COLUMNS					3

static wanderlust_menu * menu_create_wanderlust_menu (lua_State * L)
{
	wanderlust_menu * m = 0;
	SDL_Texture		* t = 0;

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < MENU_CREATE_WANDERLUST_MENU_ARGUMENT_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_TITLE))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_X))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_Y))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_TEXTURE))
	{
		goto FAILURE;
	}

	if (! (m = SDL_malloc (sizeof (wanderlust_menu))))
	{
		goto FAILURE;
	}

	m -> entities	= 0;
	m -> frame		= 0;

	if (! (m -> entities = mcreate (e_configuration.morph.doublelinkedlist)))
	{
		goto FAILURE;
	}

	t = texture_get_by_name
	(
		lua_tostring (L, MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_TEXTURE)
	);

	if (! t)
	{
		goto FAILURE;
	}

	m -> frame = frame_create_wanderlust_frame
	(
		lua_tointeger (L, MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_X),
		lua_tointeger (L, MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_Y),
		MENU_CREATE_WANDERLUST_MENU_ROWS,
		MENU_CREATE_WANDERLUST_MENU_COLUMNS,
		e_configuration.font.dimension,
		t
	);

	if (! m->frame)
	{
		goto FAILURE;
	}

	SET_U32_CONST
	(
		m -> hash,
		mhash (lua_tostring (L, MENU_CREATE_WANDERLUST_MENU_ARGUMENT_POS_TITLE))
	);

	lua_settop (L, 0);

	return m;

	FAILURE:

		menu_delete_wanderlust_menu (m);

		lua_settop (L, 0);

		return e_configuration.null;
}

/* ************************************************************************
 *
 * TASKS
 *
 * ***********************************************************************/

static nil task_menu_create (nil * item)
{
	if (! item)
	{
		return;
	}

	mpush (f_menus, item);
}

static nil task_menu_append (nil * item)
{
	static wanderlust_menu	* 	m = 0;
	static SDL_Rect				r = {0, 0, 0, 0};

	if (! (m = mpeek (f_menus, 0)))
	{
		return;
	}

	entity_fetch (m -> entities);

	if (! msize (m -> entities))
	{
		return;
	}

	r.x = m -> frame -> ulc.dst.x - m -> frame -> dimension;
	r.y = m -> frame -> ulc.dst.y - m -> frame -> dimension;
	r.w = 0;
	r.h = 0;

	mbatch (m -> entities, batch_process_region, & r);

	r.h = (r.h + m -> frame -> dimension) / m -> frame -> dimension;
	r.w = (r.w + m -> frame -> dimension) / m -> frame -> dimension;

	frame_resize_wanderlust_frame (m -> frame, (u16) r.y, (u16) r.x, (u16) r.h, (u16) r.w);
}

static nil task_menu_select (nil * item)
{
	wanderlust_entity 	* e = item;
	wanderlust_menu		* m = 0;

	if (! (m = mpeek (f_menus, 0)))
	{
		return;
	}

	if (! (e = item))
	{
		return;
	}

	mbatch (m->entities, entity_select, (u32*)&e->hash);
}

static nil task_menu_repeal (nil * item)
{
	if (! msize (f_menus))
	{
		return;
	}

	menu_delete_wanderlust_menu (mpop (f_menus));
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 menu_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized 	= e_configuration.success;
	f_menus			= e_configuration.null;

	if (! (f_menus = mcreate (e_configuration.morph.stack)))
	{
		goto FAILURE;
	}

	task_register (TASK_MENU_CREATE, task_menu_create);
	task_register (TASK_MENU_APPEND, task_menu_append);
	task_register (TASK_MENU_SELECT, task_menu_select);
	task_register (TASK_MENU_REPEAL, task_menu_repeal);

	return e_configuration.success;

	FAILURE:

		menu_terminate ();

		return e_configuration.failure;
}

nil menu_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (f_menus)
	{
		f_menus = mdelete (f_menus, menu_delete_wanderlust_menu);
	}

	f_initialized = e_configuration.failure;
}

s08 menu_active ()
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	return msize (f_menus) ? e_configuration.success : e_configuration.failure;

	FAILURE:

		return e_configuration.zero;
}

s32 menu_create (lua_State * L)
{
	wanderlust_menu * m = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (! (m = menu_create_wanderlust_menu (L)))
	{
		goto FAILURE;
	}

	task_enqueue (TASK_MENU_CREATE, m);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define MENU_SELECT_ARGUMENT_COUNT	 	1
#define MENU_SELECT_ARGUMENT_POS_TITLE 	1


s32 menu_select (lua_State * L)
{
	wanderlust_menu		*	m = 0;
	wanderlust_entity	* 	e = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (! lua_gettop (L))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, MENU_SELECT_ARGUMENT_POS_TITLE))
	{
		goto FAILURE;
	}

	if (! (m = mpeek (f_menus, 0)))
	{
		goto FAILURE;
	}

	h = mhash (lua_tostring (L, MENU_SELECT_ARGUMENT_POS_TITLE));

	e = mfind (m -> entities, &h, compare_entity_by_hash, 0);

	if (e)
	{
		task_enqueue (TASK_MENU_SELECT, e);
	}

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32 menu_repeal (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_MENU_REPEAL, 0);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 menu_append (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_MENU_APPEND, 0);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

nil menu_handle ()
{
	wanderlust_menu * m = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! (m = mpeek (f_menus, 0)))
	{
		return;
	}

	if (msize (m -> entities))
	{
		mbatch (m -> entities, entity_handle, 0);
	}
}

nil menu_update ()
{
	wanderlust_menu * m = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! (m = mpeek (f_menus, 0)))
	{
		return;
	}

	if (msize (m -> entities))
	{
		mbatch (m -> entities, entity_update, 0);
	}
}

nil menu_render ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	mbatch (f_menus, batch_render_wanderlust_menu, 0);
}
