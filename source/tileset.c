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
#include "task.h"
#include "video.h"
#include "tileset.h"

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
 * ***********************************************************************/

typedef struct
{
	SDL_Rect	source;

	s32			min_x;
	s32			min_y;
	s32			max_x;
	s32			max_y;

	u08			rate;
	u08			counter;

} wanderlust_tile;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

static s08 					f_initialized 	= WANDERLUST_FAILURE;

static wanderlust_tile	*	f_tiles 		= 0;

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 tileset_initialize (lua_State * L)
{
	u08 r, c;
	u16 index = 0;

	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized = e_configuration.success;

	f_tiles = SDL_malloc
	(
		sizeof (wanderlust_tile) * e_configuration.tileset.rows * e_configuration.tileset.columns
	);

	if (! f_tiles)
	{
		goto FAILURE;
	}

	for (r = 0; r < e_configuration.tileset.rowsDynamic2; r++)
	{
		for (c = 0; c < e_configuration.tileset.columns; c++)
		{
			index = r * e_configuration.tileset.columns + c;

			f_tiles [index].source.x 	= e_configuration.tileset.width * c;
			f_tiles [index].source.y	= e_configuration.tileset.height * r;
			f_tiles [index].source.w	= e_configuration.tileset.width;
			f_tiles [index].source.h	= e_configuration.tileset.height;
			f_tiles [index].rate		= 0;
			f_tiles [index].counter		= 0;
			f_tiles [index].max_x		= f_tiles [index].source.x;
			f_tiles [index].max_y		= f_tiles [index].source.y;
			f_tiles [index].min_x		= f_tiles [index].source.x;
			f_tiles [index].min_y		= f_tiles [index].source.y;
		}
	}

	for (r = e_configuration.tileset.rowsDynamic2; r < e_configuration.tileset.rowsDynamic4; r++)
	{
		for (c = 0; c < e_configuration.tileset.columns; c+= e_configuration.tileset.rowsDynamic2Iteration)
		{
			index = r * e_configuration.tileset.columns + c;

			f_tiles [index].source.x	= e_configuration.tileset.width * c;
			f_tiles [index].source.y	= e_configuration.tileset.height * r;
			f_tiles [index].source.w	= e_configuration.tileset.width;
			f_tiles [index].source.h	= e_configuration.tileset.height;
			f_tiles [index].rate		= e_configuration.tileset.rate;
			f_tiles [index].counter		= 0;
			f_tiles [index].min_x		= f_tiles [index].source.x;
			f_tiles [index].min_y		= f_tiles [index].source.y;
			f_tiles [index].max_x		= f_tiles [index].source.x + e_configuration.tileset.width;
			f_tiles [index].max_y		= f_tiles [index].source.y;
		}
	}

	for (r = e_configuration.tileset.rowsDynamic4; r < e_configuration.tileset.rows; r++)
	{
		for (c = 0; c < e_configuration.tileset.columns; c+= e_configuration.tileset.rowsDynamic4Iteration)
		{
			index = r * e_configuration.tileset.columns + c;

			f_tiles [index].source.x 	= e_configuration.tileset.width * c;
			f_tiles [index].source.y 	= e_configuration.tileset.height * r;
			f_tiles [index].source.w 	= e_configuration.tileset.width;
			f_tiles [index].source.h 	= e_configuration.tileset.height;
			f_tiles [index].rate		= e_configuration.tileset.rate;
			f_tiles [index].counter		= 0;
			f_tiles [index].min_x		= f_tiles [index].source.x;
			f_tiles [index].min_y		= f_tiles [index].source.y;
			f_tiles [index].max_x		= f_tiles [index].source.x + e_configuration.tileset.width * 3;
			f_tiles [index].max_y		= f_tiles [index].source.y;
		}
	}

	return e_configuration.success;

	FAILURE:

		tileset_terminate ();

		return e_configuration.failure;
}

nil tileset_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (f_tiles)
	{
		SDL_free (f_tiles);
	}

	f_initialized = e_configuration.failure;

	return;
}

nil tileset_update ()
{
	u08 r, c;
	u16	index = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	for (r = e_configuration.tileset.rowsDynamic2; r < e_configuration.tileset.rowsDynamic4; r++)
	{
		for (c = 0; c < e_configuration.tileset.columns; c+= e_configuration.tileset.rowsDynamic2Iteration)
		{
			index = r * e_configuration.tileset.columns + c;

			if (f_tiles [index].counter < f_tiles [index].rate)
			{
				f_tiles [index].counter += e_configuration.iteration;
			}
			else
			{
				if (f_tiles [index].source.x < f_tiles [index].max_x)
				{
					f_tiles [index].source.x += e_configuration.tileset.width;
				}
				else
				{
					f_tiles [index].source.x = f_tiles [index].min_x;
				}

				f_tiles [index].counter = 0;
			}
		}
	}

	for (r = e_configuration.tileset.rowsDynamic4; r < e_configuration.tileset.rows; r++)
	{
		for (c = 0; c < e_configuration.tileset.columns; c+= e_configuration.tileset.rowsDynamic4Iteration)
		{
			index = r * e_configuration.tileset.columns + c;

			if (f_tiles [index].counter < f_tiles [index].rate)
			{
				f_tiles [index].counter += e_configuration.iteration;
			}
			else
			{
				if (f_tiles [index].source.x < f_tiles [index].max_x)
				{
					f_tiles [index].source.x += e_configuration.tileset.width;
				}
				else
				{
					f_tiles [index].source.x = f_tiles [index].min_x;
				}

				f_tiles [index].counter = 0;
			}
		}
	}
}

nil tileset_render_distinct_tile (SDL_Texture * texture, u08 l, u08 r, u08 c, SDL_Rect * target)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	video_draw_tile
	(
		texture,
		target,
		&f_tiles [r * e_configuration.tileset.columns + c].source
	);
}
