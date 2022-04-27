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
#include "video.h"
#include "morph.h"
#include "task.h"
#include "texture.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * TYPE DEFINTIONS
 *
 * ************************************************************************/

typedef struct
{
	const 	u32 			hash;
			SDL_Texture	*	texture;
} wanderlust_texture;

/* ************************************************************************
 *
 * COMPARATOR
 *
 * ************************************************************************/

static u08 compare_texture_by_hash (nil * item, nil * value)
{
	if (! item)
	{
		goto FAILURE;
	}

	if (! value)
	{
		goto FAILURE;
	}

	return ((wanderlust_texture *) item) -> hash == * ((u32 *) value);

	FAILURE:

		return e_configuration.zero;
}

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ************************************************************************/

static s08 		f_initialized 	= WANDERLUST_FAILURE;

static nil	*	f_textures 		= 0;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ************************************************************************/

static wanderlust_texture * create_wanderlust_texture (u32 hash, SDL_Texture * texture)
{
	wanderlust_texture * u = 0;

	if (! texture)
	{
		goto FAILURE;
	}

	if (! (u = SDL_malloc (sizeof (wanderlust_texture))))
	{
		goto FAILURE;
	}

	u -> texture = texture;

	SET_U32_CONST (u -> hash, hash);

	return u;

	FAILURE:

		return e_configuration.null;
}

static nil delete_wanderlust_texture (nil * item)
{
	wanderlust_texture * u = item;

	if (! u)
	{
		return;
	}

	if (u -> texture)
	{
		SDL_DestroyTexture (u -> texture);
	}

	SDL_free (u);
}

static nil task_image_load (nil * item)
{
	if (! f_initialized)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	madd (f_textures, item);
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

s08 texture_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized 	= e_configuration.success;
	f_textures		= e_configuration.null;

	if (! L)
	{
		goto FAILURE;
	}

	lua_register (L, WL_LOAD_IMAGE, texture_load_image);

	if (! IMG_Init (IMG_INIT_PNG))
	{
		goto FAILURE;
	}

	if (! (f_textures = mcreate (e_configuration.morph.singlelinkedlist)))
	{
		goto FAILURE;
	}

	task_register (TASK_IMAGE_LOAD, task_image_load);

	return e_configuration.success;

	FAILURE:

		texture_terminate ();

		return e_configuration.failure;
}

nil texture_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (f_textures)
	{
		f_textures = mdelete (f_textures, delete_wanderlust_texture);
	}

	IMG_Quit ();

	f_initialized = e_configuration.failure;
}

SDL_Texture * texture_get_by_hash (u32 hash)
{
	wanderlust_texture * u = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! msize (f_textures))
	{
		goto FAILURE;
	}

	if (! (u = mfind (f_textures, &hash, compare_texture_by_hash, 0)))
	{
		goto FAILURE;
	}

	return u -> texture;

	FAILURE:

		return e_configuration.null;
}

SDL_Texture * texture_get_by_name (const char * name)
{
	wanderlust_texture	*	u 		= 0;
	u32						hash	= 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! name)
	{
		goto FAILURE;
	}

	if (! msize (f_textures))
	{
		goto FAILURE;
	}

	hash = mhash (name);

	if (! (u = mfind (f_textures, &hash, compare_texture_by_hash, 0)))
	{
		goto FAILURE;
	}

	return u->texture;

	FAILURE:

		return e_configuration.null;
}

#define TEXTURE_LOAD_IMAGE_ARG_COUNT 	2

#define TEXTURE_LOAD_IMAGE_ARG_POS_NAME	1

#define TEXTURE_LOAD_IMAGE_ARG_POS_FILE	2


s32 texture_load_image (lua_State * L)
{
	SDL_Surface 			* 	surface = 0;
	SDL_Texture 			* 	u 		= 0;
	SDL_Texture				*	v		= 0;
	wanderlust_texture 		*	w		= 0;
	u32							hash	= 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < TEXTURE_LOAD_IMAGE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, TEXTURE_LOAD_IMAGE_ARG_POS_NAME))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, TEXTURE_LOAD_IMAGE_ARG_POS_FILE))
	{
		goto FAILURE;
	}

	hash 	= mhash (lua_tostring (L, TEXTURE_LOAD_IMAGE_ARG_POS_NAME));
	u		= texture_get_by_hash (hash);

	if (u)
	{
		goto FAILURE;
	}

	surface = IMG_Load (lua_tostring (L, TEXTURE_LOAD_IMAGE_ARG_POS_FILE));

	if (! surface)
	{
		goto FAILURE;
	}

	v = video_create_texture_from_surface (surface);

	SDL_FreeSurface (surface);

	if (! v)
	{
		goto FAILURE;
	}

	w = create_wanderlust_texture (hash, v);

	if (! w)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_IMAGE_LOAD, w);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		if (surface)
		{
			SDL_FreeSurface (surface);
		}

		if (! w && v)
		{
			SDL_DestroyTexture (v);
		}

		lua_settop (L, 0);

		return e_configuration.failure;
}
