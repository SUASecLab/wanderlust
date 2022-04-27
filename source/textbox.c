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
#include "morph.h"
#include "input.h"
#include "texture.h"
#include "video.h"
#include "frame.h"
#include "textbox.h"

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
	s08					* 	text;
	u32						cursor;
	u32						page;
	u32						pages;
	u32						capacity;
	u32						length;
	u08						wait;
	SDL_Texture			* 	texture;
	wanderlust_frame	*	frame;
	u08						columns;
	u08						rows;
} wanderlust_textbox;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

static s08							f_initialized	= WANDERLUST_FAILURE;

static wanderlust_textbox		*	f_textbox		= 0;

static nil						*	f_messages		= 0;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ***********************************************************************/

static nil delete_wanderlust_textbox (nil * item)
{
	wanderlust_textbox * b = item;

	if (! b)
	{
		return;
	}

	if (b->text)
	{
		SDL_free (b->text);
	}

	if (b->frame)
	{
		frame_delete_wanderlust_frame (b->frame);
	}

	SDL_free (b);
}

#define CREATE_WANDERLUST_TEXTBOX_ARG_COUNT			3

#define CREATE_WANDERLUST_TEXTBOX_POS_ARG_FONT		1

#define CREATE_WANDERLUST_TEXTBOX_POS_ARG_CONTENT	2

#define CREATE_WANDERLUST_TEXTBOX_POS_ARG_TEXTURE	3

static wanderlust_textbox * create_wanderlust_textbox (lua_State * L)
{
	wanderlust_textbox	* b = 0;
	wanderlust_font		* f = 0;
	SDL_Texture			* t = 0;

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < CREATE_WANDERLUST_TEXTBOX_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, CREATE_WANDERLUST_TEXTBOX_POS_ARG_FONT))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, CREATE_WANDERLUST_TEXTBOX_POS_ARG_CONTENT))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, CREATE_WANDERLUST_TEXTBOX_POS_ARG_TEXTURE))
	{
		goto FAILURE;
	}

	if (! (b = SDL_malloc (sizeof (wanderlust_textbox))))
	{
		goto FAILURE;
	}

	b->text		= 0;
	b->texture 	= 0;
	b->frame 	= 0;

	f = font_get
	(
		mhash (lua_tostring (L, CREATE_WANDERLUST_TEXTBOX_POS_ARG_FONT))
	);

	if (! f)
	{
		goto FAILURE;
	}

	b->texture 	= f->texture;
	b->text		= (s08*)type_secure_copy_string
	(
		lua_tostring (L, CREATE_WANDERLUST_TEXTBOX_POS_ARG_CONTENT)
	);

	if (! b->text)
	{
		goto FAILURE;
	}

	t = texture_get_by_name
	(
		lua_tostring (L, CREATE_WANDERLUST_TEXTBOX_POS_ARG_TEXTURE)
	);

	if (! t)
	{
		goto FAILURE;
	}

	b->frame = frame_create_wanderlust_frame
	(
		e_configuration.textbox.frame.x,
		e_configuration.textbox.frame.y,
		e_configuration.textbox.frame.rows,
		e_configuration.textbox.frame.columns,
		e_configuration.textbox.frame.dimension,
		t
	);

	if (! b->frame)
	{
		goto FAILURE;
	}

	b->length	= SDL_strlen ((const char*)b->text);
	b->cursor	= 0;
	b->rows		= e_configuration.textbox.content.rows;
	b->columns	= e_configuration.textbox.content.columns;
	b->capacity	= b->rows * b->columns;
	b->page		= 0;
	b->pages	= ceil ((1.0 *b->length) / b->capacity);
	b->wait		= 0;

	lua_settop (L, 0);

	return b;

	FAILURE:

		if (b)
		{
			delete_wanderlust_textbox (b);
		}

		lua_settop (L, 0);

		return e_configuration.null;
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 textbox_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized	= e_configuration.success;
	f_messages		= e_configuration.null;
	f_textbox 		= e_configuration.null;

	if (! (f_messages = mcreate (e_configuration.morph.queue)))
	{
		goto FAILURE;
	}

	return e_configuration.success;

	FAILURE:

		textbox_terminate ();

		return e_configuration.failure;
}

nil textbox_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	f_messages 		= mdelete (f_messages, delete_wanderlust_textbox);
	f_textbox		= 0;
	f_initialized 	= e_configuration.failure;
}

s08 textbox_pending ()
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (f_textbox)
	{
		goto SUCCESS;
	}

	if (! msize (f_messages))
	{
		goto FAILURE;
	}

	if (! (f_textbox = mdequeue (f_messages)))
	{
		goto FAILURE;
	}

	SUCCESS:

		return f_textbox != e_configuration.null;

	FAILURE:

		return e_configuration.zero;
}

s32 textbox_enqueue (lua_State * L)
{
	wanderlust_textbox * b = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (! (b = create_wanderlust_textbox (L)))
	{
		goto FAILURE;
	}

	menqueue (f_messages, b);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

static nil
handle_textbox_wait ()
{
	if (! input_get_buffered_button (BUTTON_A))
	{
		return;
	}

	f_textbox->page		= f_textbox->page + 1;
	f_textbox->cursor 	= 0;
	
	if (f_textbox->page >= f_textbox->pages)
	{
		delete_wanderlust_textbox (f_textbox);

		f_textbox = 0;

		return;
	} else {
		f_textbox->wait = 0;
	}
}

nil	textbox_handle ()
{
	if (! f_textbox)
	{
		return;
	}

	if (f_textbox->wait)
	{
		handle_textbox_wait ();
	}
}

nil textbox_update ()
{
	u32 index = 0;

	if (! f_textbox)
	{
		return;
	}

	if (f_textbox->wait)
	{
		return;
	}

	if (f_textbox->cursor < f_textbox->capacity)
	{
		index = f_textbox->cursor + (f_textbox->capacity * f_textbox->page);

		if (index < f_textbox->length)
		{
			f_textbox->cursor = f_textbox->cursor + 1;
		}
		else 
		{
			f_textbox->wait = 1;
		}
	}
	else
	{
		f_textbox->wait = 1;
	}
}

nil textbox_render ()
{
	if (! f_textbox)
	{
		return;
	}

	video_draw_frame (f_textbox->frame);

	video_draw_textbox
	(
		f_textbox->texture,
		f_textbox->text + (f_textbox->capacity * f_textbox->page),
		f_textbox->cursor,
		f_textbox->columns,
		f_textbox->frame
	);
}
