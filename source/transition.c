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
#include "morph.h"
#include "texture.h"
#include "video.h"
#include "property.h"
#include "transition.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * TYPEDEFINTIONS
 *
 * ************************************************************************/

typedef struct
{
	u08				duration;

	SDL_Rect		source;

	SDL_Rect		target;

	SDL_Texture *	texture;

	nil				(* callback) (nil*);

	nil			*	argument;
} wanderlust_transition;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ************************************************************************/

static u08 							f_initialized	= WANDERLUST_FAILURE;

static nil						*	f_transitions	= 0;

static wanderlust_transition	*	f_transition	= 0;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTION
 *
 * ************************************************************************/

static wanderlust_transition * create_wanderlust_transition
(
	u08 			duration,
	SDL_Texture * 	texture,
	nil 			(* callback) (nil *),
	nil 			* argument
) {
	wanderlust_transition * t = 0;

	if (! duration)
	{
		goto FAILURE;
	}

	if (! texture)
	{
		goto FAILURE;
	}

	if (! (t = SDL_malloc (sizeof (wanderlust_transition))))
	{
		goto FAILURE;
	}

	t -> duration	= duration;
	t -> texture	= texture;
	t -> callback	= callback;
	t -> argument	= argument;

	t -> source.x	= 0;
	t -> source.y	= 0;
	t -> source.w	= e_configuration.transition.width;
	t -> source.h	= e_configuration.transition.height;

	t -> target.x	= 0;
	t -> target.y	= 0;
	t -> target.w	= property_get_integer ((const s08*)PROPERTY_WINDOW_RES_W);
	t -> target.h	= property_get_integer ((const s08*)PROPERTY_WINDOW_RES_H);

	return t;

	FAILURE:

		return e_configuration.null;
}

static nil delete_wanderlust_transition (nil * item)
{
	wanderlust_transition * t = item;

	if (! t)
	{
		return;
	}

	SDL_free (t);
}

static nil task_transition_enqueue (nil * item)
{
	if (! item)
	{
		return;
	}

	menqueue (f_transitions, item);
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

s08 transition_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized 	= e_configuration.success;
	f_transition	= e_configuration.null;
	f_transitions	= e_configuration.null;

	if (! (f_transitions = mcreate (e_configuration.morph.queue)))
	{
		goto FAILURE;
	}

	task_register (TASK_TRANSITION_ENQUEUE, task_transition_enqueue);

	return e_configuration.success;

	FAILURE:

		transition_terminate ();

		return e_configuration.failure;
}

nil transition_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (f_transitions)
	{
		f_transitions = mdelete (f_transitions, delete_wanderlust_transition);
	}

	if (f_transition)
	{
		SDL_free (f_transition);
	}

	f_transition	= 0;
	f_initialized	= e_configuration.failure;
}

nil transition_enqueue
(
	u08 			duration,
	SDL_Texture * 	texture,
	nil 			(* callback)(nil *),
	nil 		* 	argument
) {
	wanderlust_transition * t = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	t = create_wanderlust_transition (duration, texture, callback, argument);

	if (! t)
	{
		return;
	}

	task_enqueue (TASK_TRANSITION_ENQUEUE, t);
}

u08 transition_pending ()
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (f_transition)
	{
		goto SUCCESS;
	}

	if (! (f_transition = mdequeue (f_transitions)))
	{
		goto FAILURE;
	}

	SUCCESS:

		return e_configuration.null != f_transition;

	FAILURE:

		return e_configuration.zero;
}

nil transition_update ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! f_transition)
	{
		return;
	}

	if (f_transition -> duration == 1)
	{
		if (f_transition -> callback)
		{
			task_register 	(TASK_TRANSITION_FINISH, f_transition -> callback);
			task_enqueue	(TASK_TRANSITION_FINISH, f_transition -> argument);
		}
	}

	if ((-- f_transition -> duration) > 0)
	{
		return;
	}

	delete_wanderlust_transition (f_transition);

	f_transition = 0;
}

nil transition_render ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! f_transition)
	{
		return;
	}

	video_draw_sprite
	(
		f_transition -> texture, &f_transition -> source, &f_transition -> target
	);

	f_transition -> source.y += e_configuration.transition.height;
}
