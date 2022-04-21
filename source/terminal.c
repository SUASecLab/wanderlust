#include "type.h"
#include "config.h"
#include "font.h"
#include "task.h"
#include "input.h"
#include "frame.h"
#include "video.h"
#include "state.h"
#include "texture.h"
#include "textbox.h"
#include "entity.h"
#include "engine.h"
#include "brainfuck.h"
#include "menu.h"
#include "morph.h"
#include "property.h"
#include "terminal.h"

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
 * ************************************************************************/

static s08						f_initialized	=	WANDERLUST_FAILURE;

static nil					*	f_terminals		=	0;

static u08						f_mode			=	0;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - INITIALISATION, CREATION
 *
 * ************************************************************************/

nil wanderlust_terminal_delete (nil * t)
{
	wanderlust_terminal * terminal = t;

	if (! terminal)
	{
		return;
	}

	if (terminal->frame)
	{
		frame_delete_wanderlust_frame (terminal->frame);
	}

	if (terminal->script)
	{
		lua_close (terminal->script);
	}

	if (terminal->buffer.data)
	{
		SDL_free (terminal->buffer.data);
	}

	SDL_free (terminal);
}


#define WANDERLUST_TERMINAL_CREATE_ARG_COUNT				6

#define WANDERLUST_TERMINAL_CREATE_ARG_POS_TEXTURE			1

#define WANDERLUST_TERMINAL_CREATE_ARG_POS_SCRIPT 			2

#define WANDERLUST_TERMINAL_CREATE_ARG_POS_FONT_ACTIVE 		3

#define WANDERLUST_TERMINAL_CREATE_ARG_POS_FONT_INACTIVE 	4

#define WANDERLUST_TERMINAL_CREATE_ARG_POS_ROWS				6

#define WANDERLUST_TERMINAL_CREATE_ARG_POS_COLUMNS			5

wanderlust_terminal * wanderlust_terminal_create (lua_State * L)
{
	wanderlust_terminal 	* 	t				= 0;
	wanderlust_font			*	f 				= 0;
	int 						x 				= 0;
	int							y 				= 0;
	int							r 				= 0;
	int							c 				= 0;

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < WANDERLUST_TERMINAL_CREATE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, WANDERLUST_TERMINAL_CREATE_ARG_POS_TEXTURE))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, WANDERLUST_TERMINAL_CREATE_ARG_POS_SCRIPT))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, WANDERLUST_TERMINAL_CREATE_ARG_POS_FONT_ACTIVE))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, WANDERLUST_TERMINAL_CREATE_ARG_POS_FONT_INACTIVE))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, WANDERLUST_TERMINAL_CREATE_ARG_POS_ROWS))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, WANDERLUST_TERMINAL_CREATE_ARG_POS_COLUMNS))
	{
		goto FAILURE;
	}

	if (! (t = SDL_malloc (sizeof (wanderlust_terminal))))
	{
		goto FAILURE;
	}

	t->cursor.r		= 0;
	t->cursor.c		= 0;
	t->cursor.index	= 0;
	t->buffer.data	= 0;
	t->frame		= 0;
	t->script		= 0;

	SET_U08_CONST
	(
		t->buffer.cols,
		lua_tointeger (L, WANDERLUST_TERMINAL_CREATE_ARG_POS_COLUMNS)
	);

	SET_U08_CONST
	(
		t->buffer.rows,
		lua_tointeger 
		(
			L, 
			WANDERLUST_TERMINAL_CREATE_ARG_POS_ROWS
		)
	);

	SET_U16_CONST
	(
		t->buffer.size,
		t->buffer.rows * t->buffer.cols
	);

	r = t->buffer.rows + 2;
	c = t->buffer.cols + 2;
	x = property_get_integer ((const s08*)PROPERTY_WINDOW_RES_W);
	x = (x - (c * e_configuration.font.dimension)) / 2;
	y = property_get_integer ((const s08*)PROPERTY_WINDOW_RES_H);
	y = (y - (r * e_configuration.font.dimension)) / 2;

	t->frame = frame_create_wanderlust_frame
	(
		x,
		y,
		r,
		c,
		e_configuration.font.dimension,
		texture_get_by_name (lua_tostring
		(
			L,
			WANDERLUST_TERMINAL_CREATE_ARG_POS_TEXTURE)
		)
	);

	if (! t)
	{
		goto FAILURE;
	}

	t->buffer.data = SDL_calloc
	(
		t->buffer.rows * t->buffer.cols,
		t->buffer.rows * t->buffer.cols
	);

	if (! t)
	{
		goto FAILURE;
	}

	t->buffer.data [0] = '_';

	f = font_get
	(
		mhash (lua_tostring (L, WANDERLUST_TERMINAL_CREATE_ARG_POS_FONT_ACTIVE))
	);

	if (! f)
	{
		goto FAILURE;
	}

	t->cursor.active = f->texture;

	f = font_get
	(
		mhash (lua_tostring (L, WANDERLUST_TERMINAL_CREATE_ARG_POS_FONT_INACTIVE))
	);

	if (! f)
	{
		goto FAILURE;
	}

	t->cursor.inactive = f->texture;

	if (! (t->script = lua_open ()))
	{
		goto FAILURE;
	}

	luaL_openlibs (t->script);

	if (luaL_loadfile (t->script, lua_tostring (L, WANDERLUST_TERMINAL_CREATE_ARG_POS_SCRIPT)))
	{
		goto FAILURE;
	}

	SET_S08_CONST (t->active, 0x00);

	lua_settop (L, 0);

	return t;

	FAILURE:

		if (t)
		{
			wanderlust_terminal_delete (t);
		}

		lua_settop (L, 0);

		return e_configuration.null;
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - TASKS
 *
 * ************************************************************************/

static nil task_create_wanderlust_terminal (nil * item)
{
	wanderlust_terminal * 	terminal 	= item;
	int						x			= 0;
	int						y			= 0;

	if (! terminal)
	{
		return;
	}

	mpush (f_terminals, terminal);

	x = terminal->frame->llc.dst.x;
	y = terminal->frame->llc.dst.y + terminal->frame->llc.dst.h;

	lua_register (terminal->script, WL_RUNDOWN_TASK,				task_rundown);

	lua_register (terminal->script, WL_LOAD_IMAGE,				texture_load_image);

	lua_register (terminal->script, WL_CREATE_ENTITY,				entity_enqueue);

	lua_register (terminal->script, WL_SELECT_STATE_ENTITY,		state_entity_select);

	lua_register (terminal->script, WL_MODIFY_STATE_ENTITY,		state_entity_modify);

	lua_register (terminal->script, WL_HALT_ENGINE,				engine_quit);

	lua_register (terminal->script, WL_ENTER_STATE,				state_enter);

	lua_register (terminal->script, WL_LEAVE_STATE,				state_leave);

	lua_register (terminal->script, WL_ENTER_STATE_BY_TRANSITION,	state_enter_by_transition);

	lua_register (terminal->script, WL_LEAVE_STATE_BY_TRANSITION,	state_leave_by_transition);

	lua_register (terminal->script, WL_CREATE_TRANSITION,			video_enqueue_transition);

	lua_register (terminal->script, WL_CREATE_FONT, 				font_create);

	lua_register (terminal->script, WL_CREATE_TEXTBOX,			textbox_enqueue);

	lua_register (terminal->script, WL_CREATE_MENU,				menu_create);

	lua_register (terminal->script, WL_SELECT_MENU_ENTITY,		menu_select);

	lua_register (terminal->script, WL_REPEAL_MENU,				menu_repeal);

	lua_register (terminal->script, WL_APPEND_MENU,				menu_append);

	lua_register (terminal->script, WL_PUSH_BYTE_TERMINAL, 	terminal_push_byte);
	lua_register (terminal->script, WL_UNDO_BYTE_TERMINAL, 	terminal_undo_byte);

	if (lua_pcall (terminal->script, 0, 0, 0))
	{
		printf ("%s\n", lua_tostring (terminal->script, -1));
	}

	lua_getglobal (terminal->script, "init");

	lua_pushnumber (terminal->script, x);
	lua_pushnumber (terminal->script, y);

	if (lua_pcall (terminal->script, 2, 0, 0))
	{
		printf ("%s\n", lua_tostring (terminal->script, -1));
	}
}

static nil task_delete_wanderlust_terminal (nil * item)
{
	if (msize (f_terminals))
	{
		wanderlust_terminal_delete (mpop (f_terminals));
	}
}

static nil task_show_wanderlust_terminal (nil * item)
{
	if (item)
	{
		SET_S08_CONST (((wanderlust_terminal *) item)->active, 0xFF);
	}
}

static nil task_hide_wanderlust_terminal (nil * item)
{
	if (item)
	{
		SET_S08_CONST (((wanderlust_terminal *) item)->active, 0x00);
	}
}

static nil task_mode_wanderlust_terminal_text (nil * item)
{
	f_mode = 0;
}

static nil task_mode_wanderlust_terminal_data (nil * item)
{
	f_mode = 1;
}

static nil task_push_byte (nil * item)
{
	wanderlust_terminal * 	t		= 0;
	u08 					byte 	= 0;
	u16						index 	= 0;

	if (! item)
	{
		return;
	}

	byte = * (u08*) item;

	free (item);

	if (! msize (f_terminals))
	{
		return;
	}

	if (! (t = mpeek (f_terminals, 0)))
	{
		return;
	}

	if (! ((index = t->cursor.r * t->buffer.cols + t->cursor.c) < t->buffer.size))
	{
		return;
	}

	t->buffer.data [index] = byte;

	if (index + 1 < t->buffer.size)
	{
		t->buffer.data [++ index] = '_';
	}

	t->cursor.r = index / t->buffer.cols;
	t->cursor.c = index % t->buffer.cols;
}

nil task_undo_byte (nil * item)
{
	wanderlust_terminal 	* 	t 		= 0;
	u16							index 	= 0;

	if (! msize (f_terminals))
	{
		return;
	}

	if (! (t = mpeek (f_terminals, 0)))
	{
		return;
	}

	if (! (index = t->cursor.r * t->buffer.cols + t->cursor.c))
	{
		return;
	}

	if (t->buffer.data [index] != '_')
	{
		if (index == t->buffer.size - 1)
		{
			t->buffer.data [  index] 	= '_';
		}
		else
		{
			t->buffer.data [--index] 	= 0;
		}
	}
	else
	{
		t->buffer.data [ index] 	= 0;
		t->buffer.data [--index] 	= '_';
	}

	t->cursor.r = index / t->buffer.cols;
	t->cursor.c = index % t->buffer.cols;
}

nil task_cursor_increment (nil * item)
{
	wanderlust_terminal * 	t		= 0;
	u16						index 	= 0;

	if (! msize (f_terminals))
	{
		return;
	}

	if (! (t = mpeek (f_terminals, 0)))
	{
		return;
	}

	index = t->cursor.r * t->buffer.cols + t->cursor.c;

	if (index >= t->buffer.size)
	{
		return;
	}

	if (t->buffer.data [index] == '_')
	{
		return;
	}

	++ index;

	if (index < t->buffer.size)
	{
		if (t->buffer.data [index] == 0)
		{
			t->buffer.data [index] = '_';
		}
	}
	else
	{
		return;
	}

	t->cursor.r = index / t->buffer.cols;
	t->cursor.c = index % t->buffer.cols;
}

nil task_cursor_decrement (nil * item)
{
	wanderlust_terminal * 	t		= 0;
	u16						index 	= 0;

	if (! msize (f_terminals))
	{
		return;
	}

	if (! (t = mpeek (f_terminals, 0)))
	{
		return;
	}

	if (! (index = t->cursor.r * t->buffer.cols + t->cursor.c))
	{
		return;
	}

	if (t->buffer.data [index] == '_')
	{
		t->buffer.data [index] = 0;
	}

	-- index;

	t->cursor.r = index / t->buffer.cols;
	t->cursor.c = index % t->buffer.cols;
}

nil task_run_terminal (nil * item)
{
	wanderlust_terminal * t;

	if (! msize (f_terminals))
	{
		return;
	}

	if (! (t = mpeek (f_terminals, 0)))
	{
		return;
	}
}

nil task_clear_terminal (nil * item)
{
	u16 					i = 0;
	wanderlust_terminal * 	t = 0;

	if (! msize (f_terminals))
	{
		return;
	}

	if (! (t = mpeek (f_terminals, 0)))
	{
		return;
	}

	for (i = 0; i < t->buffer.size; i++)
	{
		t->buffer.data [i] = 0;
	}

	t->buffer.data [0] 	= '_';
	t->cursor.r 			= 0;
	t->cursor.c 			= 0;
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

s08 terminal_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized 	= e_configuration.success;
	f_terminals		= e_configuration.null;

	if (! (f_terminals = mcreate (e_configuration.morph.stack)))
	{
		goto FAILURE;
	}

	task_register (TASK_TERMINAL_CREATE,			task_create_wanderlust_terminal);
	task_register (TASK_TERMINAL_DELETE,			task_delete_wanderlust_terminal);
	task_register (TASK_TERMINAL_SHOW, 				task_show_wanderlust_terminal);
	task_register (TASK_TERMINAL_HIDE, 				task_hide_wanderlust_terminal);
	task_register (TASK_TERMINAL_PUSH_BYTE, 		task_push_byte);
	task_register (TASK_TERMINAL_UNDO_BYTE, 		task_undo_byte);
	task_register (TASK_TERMINAL_CURSOR_INC,		task_cursor_increment);
	task_register (TASK_TERMINAL_CURSOR_DEC,		task_cursor_decrement);
	task_register (TASK_TERMINAL_RUN,				task_run_terminal);
	task_register (TASK_TERMINAL_CLEAR,				task_clear_terminal);
	task_register (TASK_TERMINAL_MODE_TEXT,			task_mode_wanderlust_terminal_text);
	task_register (TASK_TERMINAL_MODE_DATA,			task_mode_wanderlust_terminal_data);

	lua_register (L, WL_CREATE_TERMINAL, 			terminal_create);
	lua_register (L, WL_DELETE_TERMINAL, 			terminal_delete);
	lua_register (L, WL_PUSH_BYTE_TERMINAL, 		terminal_push_byte);
	lua_register (L, WL_UNDO_BYTE_TERMINAL, 		terminal_undo_byte);
	lua_register (L, WL_SHOW_TERMINAL, 				terminal_show);
	lua_register (L, WL_HIDE_TERMINAL, 				terminal_hide);
	lua_register (L, WL_INCREMENT_CURSOR_TERMINAL,	terminal_increment_cursor);
	lua_register (L, WL_DECREMENT_CURSOR_TERMINAL,	terminal_decrement_cursor);
	lua_register (L, WL_RUN_TERMINAL,				terminal_run);
	lua_register (L, WL_CLEAR_TERMINAL,				terminal_clear);

	return e_configuration.success;

	FAILURE:

		terminal_terminate ();

		return e_configuration.failure;
}

nil terminal_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (f_terminals)
	{
		f_terminals = mdelete (f_terminals, wanderlust_terminal_delete);
	}

	f_initialized = e_configuration.failure;
}

s08 terminal_active ()
{
	wanderlust_terminal * terminal	= 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! msize (f_terminals))
	{
		goto FAILURE;
	}

	if (! (terminal = mpeek (f_terminals, 0)))
	{
		goto FAILURE;
	}

	return terminal->active;

	FAILURE:

		return e_configuration.zero;
}

s32 terminal_create (lua_State * L)
{
	wanderlust_terminal * terminal = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (! (terminal = wanderlust_terminal_create (L)))
	{
		goto FAILURE;
	}

	task_enqueue (TASK_TERMINAL_CREATE, terminal);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32 terminal_delete (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (! msize (f_terminals))
	{
		goto FAILURE;
	}

	task_enqueue (TASK_TERMINAL_DELETE, 0);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32 terminal_show (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (! msize (f_terminals))
	{
		goto FAILURE;
	}

	task_enqueue (TASK_TERMINAL_SHOW, mpeek (f_terminals, 0));

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32 terminal_hide (lua_State * L)
{
	if (msize (f_terminals))
	{
		task_enqueue (TASK_TERMINAL_HIDE, mpeek (f_terminals, 0));
	}

	return 0;
}

s32 terminal_push_byte (lua_State * L)
{
	u08 * byte = 0;

	if (! lua_gettop (L))
	{
		return 0;
	}

	if (! lua_isnumber (L, 1))
	{
		return 0;
	}

	if (! msize (f_terminals))
	{
		return 0;
	}

	if (! (byte = SDL_malloc (sizeof (u08))))
	{
		return 0;
	}

	*byte = lua_tointeger (L, 1);

	task_enqueue (TASK_TERMINAL_PUSH_BYTE, byte);

	return 0;
}

s32 terminal_undo_byte (lua_State * L)
{
	if (msize (f_terminals))
	{
		task_enqueue (TASK_TERMINAL_UNDO_BYTE, 0);
	}

	return 0;
}

s32 terminal_increment_cursor (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! msize (f_terminals))
	{
		goto FAILURE;
	}

	task_enqueue (TASK_TERMINAL_CURSOR_INC, 0);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 terminal_decrement_cursor (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! msize (f_terminals))
	{
		goto FAILURE;
	}

	task_enqueue (TASK_TERMINAL_CURSOR_DEC, 0);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 terminal_text_mode (lua_State * L)
{
	task_enqueue (TASK_TERMINAL_MODE_TEXT, 0);

	return e_configuration.success;
}

s32 terminal_data_mode (lua_State * L)
{
	task_enqueue (TASK_TERMINAL_MODE_DATA, 0);

	return e_configuration.success;
}

s32 terminal_run (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! msize (f_terminals))
	{
		goto FAILURE;
	}

	task_enqueue (TASK_TERMINAL_RUN, 0);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 terminal_clear (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! msize (f_terminals))
	{
		goto FAILURE;
	}

	task_enqueue (TASK_TERMINAL_CLEAR, 0);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

nil terminal_handle ()
{

}

#include "assembler.h"

nil terminal_update ()
{
	wanderlust_terminal * 	terminal 	= e_configuration.null;
	u08						r			= 0;
	u16						index		= 0;
	
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! msize (f_terminals))
	{
		return;
	}

	terminal = mpeek (f_terminals, 0);

	if (! terminal)
	{
		return;
	}

	if (! terminal->active)
	{
		return;
	}

	index = assembler_get_cursor ();

	if (f_mode)
	{
		for (r = 0; r < terminal->buffer.rows; r++) 
		{
			index += assembler_writeline_data (
				index,
				terminal->buffer.data + r * terminal->buffer.cols,
				terminal->buffer.cols
			);
		}
	}
	else
	{
		for (r = 0; r < terminal->buffer.rows; r++) 
		{
			index += assembler_writeline (
				index,
				terminal->buffer.data + r * terminal->buffer.cols,
				terminal->buffer.cols
			);
		}
	}

}

nil terminal_render ()
{
	wanderlust_terminal * terminal = e_configuration.null;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! msize (f_terminals))
	{
		return;
	}

	terminal = mpeek (f_terminals, 0);

	if (! terminal)
	{
		return;
	}

	if (! terminal->active)
	{
		return;
	}

	video_draw_terminal (terminal);
}
