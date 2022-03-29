#include "type.h"
#include "config.h"
#include "font.h"
#include "input.h"
#include "video.h"
#include "state.h"
#include "entity.h"
#include "texture.h"
#include "textbox.h"
#include "menu.h"
#include "task.h"
#include "property.h"
#include "engine.h"
#include "terminal.h"
#include "morph.h"
#include "mnemonic.h"
#include "assembler.h"

#include "entity.retrobutton.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * MACRO
 *
 * ***********************************************************************/

#define CALLBACK_INPUT_START	"on_key_start"

#define CALLBACK_INPUT_BACK		"on_key_back"

#define CALLBACK_INPUT_UP		"on_key_up"

#define CALLBACK_INPUT_DOWN 	"on_key_down"

#define CALLBACK_INPUT_LEFT		"on_key_left"

#define CALLBACK_INPUT_RIGHT	"on_key_right"

#define CALLBACK_INPUT_A		"on_key_a"

#define CALLBACK_INPUT_B		"on_key_b"

#define CALLBACK_INPUT_X		"on_key_x"

#define CALLBACK_INPUT_Y		"on_key_y"

/* ************************************************************************
 *
 * TYPE DEFINITIONS
 *
 * ***********************************************************************/

typedef struct
{
	u08				state;

	u16				length;

	lua_State	*	lua;

	struct
	{
		SDL_Texture * idle;
		SDL_Texture * hover;
		SDL_Texture * click;
		SDL_Texture * inactive;

	} textures;
}
retrobutton_property;

typedef enum
{
	RETROBUTTON_IDLE,
	RETROBUTTON_HOVER,
	RETROBUTTON_CLICK,
	RETROBUTTON_INACTIVE,
	RETROBUTTON_STATES
}
retrobutton_state;

typedef enum
{
	MODIFY_TITLE,
	MODIFY_ROW,
	MODIFY_COLUMN,
	MODIFY_DIRECTION,
	MODIFY_TEXTURE_IDLE,
	MODIFY_TEXTURE_HOVER,
	MODIFY_TEXTURE_CLICK,
	MODIFY_TEXTURE_INACTIVE,
	MODIFIER_COUNT
}
retrobutton_modifier;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - SETUP
 *
 * ***********************************************************************/

static nil setup_idle (wanderlust_entity * e, retrobutton_property * p)
{
	e->texture = p->textures.idle;
}

static nil setup_hover (wanderlust_entity * e, retrobutton_property * p)
{
	e->texture = p->textures.hover;
}

static nil setup_click (wanderlust_entity * e, retrobutton_property * p)
{
	e->texture = p->textures.click;
}

static nil setup_inactive (wanderlust_entity * e, retrobutton_property * p)
{
	e->texture = p->textures.inactive;
}

/* ************************************************************************
 *
 * BANK - SETUP
 *
 * ***********************************************************************/

static nil (* setup [RETROBUTTON_STATES]) (wanderlust_entity *, retrobutton_property *) =
{
	setup_idle, setup_hover, setup_click, setup_inactive
};

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - MODIFIY
 *
 * ***********************************************************************/

static nil modify_title (wanderlust_entity * e, lua_State * L, u16 index)
{
	retrobutton_property 	* p 	= e->properties;
	s08 					* title = 0;

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
	p->length	= SDL_strlen ((const char*)e->title);
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

static nil modify_texture_idle (wanderlust_entity * e, lua_State * L, u16 index)
{
	retrobutton_property 	* 	p	= e->properties;
	wanderlust_font			* 	f	= 0;

	if (! lua_isstring (L, index))
	{
		return;
	}

	if (! (f = font_get (mhash (lua_tostring (L, index)))))
	{
		return;
	}

	p->textures.idle = f->texture;

	setup [p->state] (e, p);
}

static nil modify_texture_hover (wanderlust_entity * e, lua_State * L, u16 index)
{
	retrobutton_property 	* 	p			= e->properties;
	wanderlust_font			* 	f			= 0;

	if (! lua_isstring (L, index))
	{
		return;
	}

	if (! (f = font_get (mhash (lua_tostring (L, index)))))
	{
		return;
	}

	p->textures.hover = f->texture;

	setup [p->state] (e, p);
}

static nil modify_texture_click (wanderlust_entity * e, lua_State * L, u16 index)
{
	retrobutton_property 	* 	p			= e->properties;
	wanderlust_font			* 	f			= 0;

	if (! lua_isstring (L, index))
	{
		return;
	}

	if (! (f = font_get (mhash (lua_tostring (L, index)))))
	{
		return;
	}

	p->textures.click = f->texture;

	setup [p->state] (e, p);
}

static nil modify_texture_inactive (wanderlust_entity * e, lua_State * L, u16 index)
{
	retrobutton_property 	* 	p		= e->properties;
	wanderlust_font			* 	f		= 0;

	if (! lua_isstring (L, index))
	{
		return;
	}

	if (! (f = font_get (mhash (lua_tostring (L, index)))))
	{
		return;
	}

	p->textures.inactive = f->texture;

	setup [p->state] (e, p);
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - ADJUST
 *
 * ***********************************************************************/

static nil adjust_idle (wanderlust_entity * e, retrobutton_property * p)
{

}

static nil adjust_hover (wanderlust_entity * e, retrobutton_property * p)
{

}

static nil adjust_click (wanderlust_entity * e, retrobutton_property * p)
{

}

static nil adjust_inactive (wanderlust_entity * e, retrobutton_property * p)
{

}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - DIRECT
 *
 * ***********************************************************************/

static nil direct_idle (wanderlust_entity * e, retrobutton_property * p)
{

}

static nil direct_hover (wanderlust_entity * e, retrobutton_property * p)
{

}

static nil direct_click (wanderlust_entity * e, retrobutton_property * p)
{

}

static nil direct_inactive (wanderlust_entity * e, retrobutton_property * p)
{

}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - HANDLE
 *
 * ***********************************************************************/

static nil handle_idle (wanderlust_entity * e, retrobutton_property * p)
{

}

static nil handle_hover (wanderlust_entity * e, retrobutton_property * p)
{
	if (input_get_buffered_button (e_configuration.button.up))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_UP);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.down))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_DOWN);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.left))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_LEFT);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.right))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_RIGHT);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		setup [(p->state = RETROBUTTON_CLICK)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_B);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.x))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_X);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.y))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_Y);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.start))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_START);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.back))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_BACK);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}
}

static nil handle_click (wanderlust_entity * e, retrobutton_property * p)
{
	lua_getglobal (p->lua, CALLBACK_INPUT_A);

	if (lua_pcall (p->lua, 0, 0, 0))
	{
		return;
	}

	lua_pop (p->lua, lua_gettop (p->lua));

	setup [(p->state = RETROBUTTON_HOVER)] (e, p);
}

static nil handle_inactive (wanderlust_entity * e, retrobutton_property * p)
{
	if (input_get_buffered_button (e_configuration.button.up))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_UP);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.down))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_DOWN);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.left))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_LEFT);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}

	if (input_get_buffered_button (e_configuration.button.right))
	{
		lua_getglobal (p->lua, CALLBACK_INPUT_RIGHT);

		if (lua_pcall (p->lua, 0, 0, 0))
		{
			return;
		}

		lua_pop (p->lua, lua_gettop (p->lua));

		return;
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - UPDATE
 *
 * ***********************************************************************/

static nil update_idle (wanderlust_entity * e, retrobutton_property * p)
{

}

static nil update_hover (wanderlust_entity * e, retrobutton_property * p)
{

}

static nil update_click (wanderlust_entity * e, retrobutton_property * p)
{

}

static nil update_inactive (wanderlust_entity * e, retrobutton_property * p)
{

}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - RENDER
 *
 * ***********************************************************************/

static nil render_idle (wanderlust_entity * e, retrobutton_property * p)
{
	video_draw_text
	(
		e->texture,
		e->title,
		p->length,
		e->target.x,
		e->target.y,
		e_configuration.font.dimension
	);
}

static nil render_hover (wanderlust_entity * e, retrobutton_property * p)
{
	video_draw_text
	(
		e->texture,
		e->title,
		p->length,
		e->target.x,
		e->target.y,
		e_configuration.font.dimension
	);
}

static nil render_click (wanderlust_entity * e, retrobutton_property * p)
{
	video_draw_text
	(
		e->texture,
		e->title,
		p->length,
		e->target.x,
		e->target.y,
		e_configuration.font.dimension
	);
}

static nil render_inactive (wanderlust_entity * e, retrobutton_property * p)
{
	video_draw_text
	(
		e->texture,
		e->title,
		p->length,
		e->target.x,
		e->target.y,
		e_configuration.font.dimension
	);
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - BANK
 *
 * ***********************************************************************/

static nil (* adjust [RETROBUTTON_STATES]) (wanderlust_entity *, retrobutton_property *) =
{
	adjust_idle, adjust_hover, adjust_click, adjust_inactive
};

static nil (* direct [RETROBUTTON_STATES]) (wanderlust_entity *, retrobutton_property *) =
{
	direct_idle, direct_hover, direct_click, direct_inactive
};

static nil (* handle [RETROBUTTON_STATES]) (wanderlust_entity *, retrobutton_property *) =
{
	handle_idle, handle_hover, handle_click, handle_inactive
};

static nil (* update [RETROBUTTON_STATES]) (wanderlust_entity *, retrobutton_property *) =
{
	update_idle, update_hover, update_click, update_inactive
};

static nil (* render [RETROBUTTON_STATES]) (wanderlust_entity *, retrobutton_property *) =
{
	render_idle, render_hover, render_click, render_inactive
};

static nil (* modify [MODIFIER_COUNT]) (wanderlust_entity *, lua_State *, u16) =
{
	modify_title,
	modify_row,
	modify_column,
	modify_direction,
	modify_texture_idle,
	modify_texture_hover,
	modify_texture_click,
	modify_texture_inactive
};

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ***********************************************************************/

static nil retrobutton_delete_properties (retrobutton_property * p)
{
	if (! p)
	{
		return;
	}

	if (p->lua)
	{
		lua_close (p->lua);
	}

	SDL_free (p);
}

#include "core.h"

#define RETROBUTTON_CREATE_PROPERTIES_ARG_POS_SCRIPT	5
#define RETROBUTTON_CREATE_PROPERTIES_ARG_POS_IDLE		6
#define RETROBUTTON_CREATE_PROPERTIES_ARG_POS_HOVER		7
#define RETROBUTTON_CREATE_PROPERTIES_ARG_POS_CLICK		8
#define RETROBUTTON_CREATE_PROPERTIES_ARG_POS_INACTIVE	9

static retrobutton_property * retrobutton_create_properties (lua_State * L)
{
	wanderlust_font			* f = 0;
	retrobutton_property 	* p = 0;

	if (! L)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, RETROBUTTON_CREATE_PROPERTIES_ARG_POS_SCRIPT))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, RETROBUTTON_CREATE_PROPERTIES_ARG_POS_IDLE))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, RETROBUTTON_CREATE_PROPERTIES_ARG_POS_HOVER))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, RETROBUTTON_CREATE_PROPERTIES_ARG_POS_CLICK))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, RETROBUTTON_CREATE_PROPERTIES_ARG_POS_INACTIVE))
	{
		goto FAILURE;
	}

	if (! (p = SDL_malloc (sizeof (retrobutton_property))))
	{
		goto FAILURE;
	}

	p->state				= 0;
	p->textures.idle		= 0;
	p->textures.hover		= 0;
	p->textures.click		= 0;
	p->textures.inactive	= 0;

	if (! (p->lua = lua_open ()))
	{
		goto FAILURE;
	}

	luaL_openlibs (p->lua);

	if (luaL_loadfile (p->lua, lua_tostring (L, 5)))
	{
		goto FAILURE;
	}

	lua_register (p->lua, WL_RUNDOWN_TASK,				task_rundown);
	lua_register (p->lua, WL_LOAD_IMAGE,				texture_load_image);
	lua_register (p->lua, WL_CREATE_ENTITY,				entity_enqueue);
	lua_register (p->lua, WL_SELECT_STATE_ENTITY,		state_entity_select);
	lua_register (p->lua, WL_MODIFY_STATE_ENTITY,		state_entity_modify);
	lua_register (p->lua, WL_HALT_ENGINE,				engine_quit);
	lua_register (p->lua, WL_ENTER_STATE,				state_enter);
	lua_register (p->lua, WL_LEAVE_STATE,				state_leave);
	lua_register (p->lua, WL_ENTER_STATE_BY_TRANSITION,	state_enter_by_transition);
	lua_register (p->lua, WL_LEAVE_STATE_BY_TRANSITION,	state_leave_by_transition);
	lua_register (p->lua, WL_CREATE_TRANSITION,			video_enqueue_transition);
	lua_register (p->lua, WL_CREATE_FONT, 				font_create);
	lua_register (p->lua, WL_CREATE_TEXTBOX,			textbox_enqueue);
	lua_register (p->lua, WL_CREATE_MENU,				menu_create);
	lua_register (p->lua, WL_SELECT_MENU_ENTITY,		menu_select);
	lua_register (p->lua, WL_REPEAL_MENU,				menu_repeal);
	lua_register (p->lua, WL_APPEND_MENU,				menu_append);
	lua_register (p->lua, WL_SET_SCALING_FACTOR,		video_set_scaling_factor);
	lua_register (p->lua, WL_SET_FULLSCREEN_MODE,		video_fullscreen_mode);
	lua_register (p->lua, WL_SET_WINDOW_MODE,			video_window_mode);
	lua_register (p->lua, WL_GET_PROPERTY, 				property_get_wanderlust_property);
	lua_register (p->lua, WL_SET_PROPERTY,				property_set_wanderlust_property);
	lua_register (p->lua, WL_CREATE_TERMINAL, 			terminal_create);
	lua_register (p->lua, WL_DELETE_TERMINAL, 			terminal_delete);
	lua_register (p->lua, WL_SHOW_TERMINAL, 			terminal_show);
	lua_register (p->lua, WL_HIDE_TERMINAL, 			terminal_hide);
	lua_register (p->lua, WL_PUSH_BYTE_TERMINAL, 		terminal_push_byte);
	lua_register (p->lua, WL_UNDO_BYTE_TERMINAL, 		terminal_undo_byte);
	lua_register (p->lua, WL_INCREMENT_CURSOR_TERMINAL,	terminal_increment_cursor);
	lua_register (p->lua, WL_DECREMENT_CURSOR_TERMINAL,	terminal_decrement_cursor);
	lua_register (p->lua, WL_RUN_TERMINAL,				terminal_run);
	lua_register (p->lua, WL_CLEAR_TERMINAL,			terminal_clear);
	lua_register (p->lua, WL_INCREMENT_WINDOW_SIZE,		video_increment_window_size);
	lua_register (p->lua, WL_DECREMENT_WINDOW_SIZE,		video_decrement_window_size);

	lua_register (
		p->lua, 
		WL_MNEMONIC_TARGET_REGISTER,	
		mnemonic_set_target_register
	);

	lua_register (
		p->lua, 
		WL_MNEMONIC_TARGET_REGISTER_PAIR,	
		mnemonic_set_target_register_pair
	);

	lua_register (
		p->lua, 
		WL_MNEMONIC_SOURCE_REGISTER,
		mnemonic_set_source_register
	);

	lua_register (
		p->lua, 
		WL_MNEMONIC_SOURCE_REGISTER_PAIR,
		mnemonic_set_source_register_pair
	);

	lua_register (
		p->lua, 
		WL_MNEMONIC_TARGET_CONSTANT,
		mnemonic_set_target_constant
	);

	lua_register (
		p->lua, 
		WL_MNEMONIC_SOURCE_BYTE_A,
		mnemonic_set_source_byte_na
	);

	lua_register (
		p->lua, 
		WL_MNEMONIC_SOURCE_BYTE_B,
		mnemonic_set_source_byte_nb
	);

	lua_register (
		p->lua, 
		WL_MNEMONIC_SOURCE_WORD_A,
		mnemonic_set_source_word_na
	);

	lua_register (
		p->lua, 
		WL_MNEMONIC_SOURCE_WORD_B,
		mnemonic_set_source_word_nb
	);

	lua_register (
		p->lua, 
		WL_MNEMONIC_SOURCE_WORD_C,
		mnemonic_set_source_word_nc
	);

	lua_register (
		p->lua, 
		WL_MNEMONIC_SOURCE_WORD_D,
		mnemonic_set_source_word_nd
	);

	lua_register (
		p->lua,
		WL_MNEMONIC_APPLY,
		mnemonic_set_apply
	);

	lua_register (
		p->lua,
		WL_MNEMONIC_BASE,
		mnemonic_set_base
	);

	lua_register (
		p->lua,
		WL_ASSEMBLER_INR_CURSOR,
		assembler_inr_cursor
	);

	lua_register (
		p->lua,
		WL_ASSEMBLER_DCR_CURSOR,
		assembler_dcr_cursor
	);

	lua_register (
		p->lua,
		WL_ASSEMBLER_PROGRAM_RUN,
		assembler_program_run
	);

	lua_register (
		p->lua,
		WL_CORE_GET_OUTPUT,
		core_get_output
	);

	lua_register (
		p->lua,
		WL_MNEMONIC_SOURCE_BYTE,
		mnemonic_set_source_byte
	);

	lua_register (
		p->lua,
		WL_TERMINAL_TEXT_MODE,
		terminal_text_mode
	);

	lua_register (
		p->lua,
		WL_TERMINAL_DATA_MODE,
		terminal_data_mode
	);

	if (lua_pcall (p->lua, 0, 0, 0))
	{
		goto FAILURE;
	}

	f = font_get (mhash
	(
		lua_tostring (L, RETROBUTTON_CREATE_PROPERTIES_ARG_POS_IDLE)
	));

	if (! f)
	{
		goto FAILURE;
	}

	p->textures.idle = f->texture;

	f = font_get (mhash
	(
		lua_tostring (L, RETROBUTTON_CREATE_PROPERTIES_ARG_POS_HOVER)
	));

	if (! f)
	{
		goto FAILURE;
	}

	p->textures.hover = f->texture;

	f = font_get (mhash
	(
		lua_tostring (L, RETROBUTTON_CREATE_PROPERTIES_ARG_POS_CLICK)
	));

	if (! f)
	{
		goto FAILURE;
	}

	p->textures.click = f->texture;


	f = font_get (mhash
	(
		lua_tostring (L, RETROBUTTON_CREATE_PROPERTIES_ARG_POS_INACTIVE)
	));

	if (! f)
	{
		goto FAILURE;
	}

	p->textures.inactive = f->texture;

	return p;

	FAILURE:

		if (p)
		{
			retrobutton_delete_properties (p);
		}

		return e_configuration.null;
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

#define ENTITY_RETROBUTTON_CREATE_ARG_COUNT 		9
#define ENTITY_RETROBUTTON_CREATE_ARG_POS_X			2
#define ENTITY_RETROBUTTON_CREATE_ARG_POS_Y			3
#define ENTITY_RETROBUTTON_CREATE_ARG_POS_TITLE		4

wanderlust_entity * entity_retrobutton_create (lua_State * L)
{
	wanderlust_entity 		*	e = 0;
	retrobutton_property	*	p = 0;

	if (lua_gettop (L) < ENTITY_RETROBUTTON_CREATE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_RETROBUTTON_CREATE_ARG_POS_X))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_RETROBUTTON_CREATE_ARG_POS_Y))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, ENTITY_RETROBUTTON_CREATE_ARG_POS_TITLE))
	{
		goto FAILURE;
	}

	if (! (e = SDL_malloc (sizeof (wanderlust_entity))))
	{
		goto FAILURE;
	}

	SET_U08_CONST (e->locked, 0x00);
	SET_U08_CONST (e->type,	RETROBUTTON_ENTITY);
	SET_U32_CONST (e->hash,	mhash (lua_tostring (L, 4)));

	e->texture 		= 0;
	e->properties	= 0;
	e->title		= 0;
	e->source		= 0;
	e->target.x 	= lua_tointeger (L, 2);
	e->target.y 	= lua_tointeger (L, 3);
	e->column		= e->target.x / e_configuration.tileset.width;
	e->row			= e->target.y / e_configuration.tileset.height;

	e->title = (s08*)type_secure_copy_string
	(
		lua_tostring (L, ENTITY_RETROBUTTON_CREATE_ARG_POS_TITLE)
	);

	if (! e->title)
	{
		goto FAILURE;
	}

	if (! (e->properties = retrobutton_create_properties (L)))
	{
		goto FAILURE;
	}

	p			= e->properties;
	p->length	= SDL_strlen((const char*)e->title);
	e->target.w	= e_configuration.font.dimension * p->length;
	e->target.h	= e_configuration.font.dimension;

	setup [(p->state = RETROBUTTON_IDLE)] (e, p);

	lua_settop (L, 0);

	return e;

	FAILURE:

		if (e)
		{
			entity_retrobutton_delete (e);
		}

		if (L)
		{
			lua_settop (L, 0);
		}

		return e_configuration.null;
}

nil entity_retrobutton_direct (wanderlust_entity * e)
{
	retrobutton_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	direct [p->state] (e, p);
}

nil entity_retrobutton_delete (wanderlust_entity * e)
{
	if (! e)
	{
		return;
	}

	if (e->properties)
	{
		retrobutton_delete_properties (e->properties);
	}

	if (e->title)
	{
		SDL_free (e->title);
	}

	SDL_free (e);
}

nil entity_retrobutton_handle (wanderlust_entity * e)
{
	retrobutton_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	handle [p->state] (e, p);
}

nil entity_retrobutton_update (wanderlust_entity * e)
{
	retrobutton_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	update [p->state] (e, p);
}

nil entity_retrobutton_render (wanderlust_entity * e)
{
	retrobutton_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	render [p->state] (e, p);
}

nil	entity_retrobutton_adjust (wanderlust_entity * e)
{
	retrobutton_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	adjust [p->state] (e, p);
}

nil	entity_retrobutton_select (wanderlust_entity * e)
{
	retrobutton_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	setup [(p->state = RETROBUTTON_HOVER)] (e, p);
}

nil	entity_retrobutton_recall (wanderlust_entity * e)
{
	retrobutton_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	setup [(p->state = RETROBUTTON_IDLE)] (e, p);
}

nil entity_retrobutton_excite (wanderlust_entity * e) { }

nil entity_retrobutton_expire (wanderlust_entity * e) { }

nil entity_retrobutton_modify (wanderlust_entity * e, lua_State * L, u16 index)
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

nil	entity_retrobutton_initialize ()
{

}

nil	entity_retrobutton_terminate ()
{

}
