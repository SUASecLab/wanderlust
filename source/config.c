#include "type.h"
#include "task.h"
#include "video.h"
#include "input.h"
#include "transition.h"
#include "state.h"
#include "entity.h"
#include "font.h"
#include "map.h"
#include "textbox.h"
#include "menu.h"
#include "terminal.h"
#include "morph.h"
#include "font.h"
#include "config.h"
#include "mnemonic.h"
#include "assembler.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * MACROS
 *
 * ************************************************************************/

#define WANDERLUST_SCALING_DEFAULT			1

#define WANDERLUST_SCALING_MIN				1

#define WANDERLUST_SCALING_MAX				4

#define WANDERLUST_ITERATION				1

#define WANDERLUST_TILE_DIMENSION			16

#define WANDERLUST_TILESET_ROWS_DYNAMIC_2	16

#define WANDERLUST_TILESET_ROWS_DYNAMIC_4	24

#define WANDERLUST_TILESET_DYNAMIC_RATE		40

#define WANDERLUST_TILESET_TYPES			2

#define WANDERLUST_TILESET_ROWS				32

#define WANDERLUST_TILESET_COLUMNS			16

#define WANDERLUST_MAP_LAYER				6

#define WANDERLUST_MAP_LAYER_TERRAIN		5

#define WANDERLUST_MAP_LAYER_EVENT			4

#define WANDERLUST_MAP_OFFSET_ROW			1

#define WANDERLUST_MAP_OFFSET_COLUMN		1

#define WANDERLUST_MAP_DIVISOR				256

#define WANDERLUST_MAP_STEPPING_WALK		1

#define WANDERLUST_MAP_STEPPING_RIDE		2

#define WANDERLUST_MAP_STEPPING_SWIM		1

#define WANDERLUST_TERRAIN_WATER			0x0001

#define WANDERLUST_TERRAIN_ENTITY			0xFFFE

#define WANDERLUST_TERRAIN_OPEN				0xFFFF

#define WANDERLUST_DRAW_COLOR_R				0x00

#define WANDERLUST_DRAW_COLOR_G				0x00

#define WANDERLUST_DRAW_COLOR_B				0x00

#define WANDERLUST_DRAW_COLOR_A				0xFF

#define WANDERLUST_FPS						16

#define WANDERLUST_STATE_LOCK				0xFF

#define WANDERLUST_STATE_OPEN				0x00

#define WANDERLUST_FONT_DIMENSION			0x8

#define WANDERLUST_FONT_ROWS				0x10

#define WANDERLUST_FONT_COLS				0x10

#define WANDERLUST_FONT_DEPTH				0x20

#define WANDERLUST_TRANSITION_SRC_W			21

#define WANDERLUST_TRANSITION_SRC_H			15

#define WANDERLUST_TEXTBOX_FRAME_X			40

#define WANDERLUST_TEXTBOX_FRAME_Y 			168

#define WANDERLUST_TEXTBOX_FRAME_R			8

#define WANDERLUST_TEXTBOX_FRAME_C 			32

#define WANDERLUST_TEXTBOX_FRAME_D 			8

#define WANDERLUST_TEXTBOX_TEXT_LR 			6

#define WANDERLUST_TEXTBOX_TEXT_LC 			30

/* ************************************************************************
 *
 * PROJ SCOPE VARIABLES
 *
 * ************************************************************************/

wanderlust_config e_configuration;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ************************************************************************/

static s08 	f_initialized = WANDERLUST_FAILURE;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ************************************************************************/

static s08 configuration_init (lua_State * L)
{
	e_configuration.title 							= 0;
	e_configuration.fps								= WANDERLUST_FPS;
	e_configuration.iteration						= WANDERLUST_ITERATION;
	e_configuration.success							= WANDERLUST_SUCCESS;
	e_configuration.failure							= WANDERLUST_FAILURE;
	e_configuration.flag							= 0;
	e_configuration.null							= 0;
	e_configuration.zero							= 0;

	e_configuration.resolution.hidden 				= SDL_WINDOW_HIDDEN;
	e_configuration.resolution.shown				= SDL_WINDOWEVENT_SHOWN;
	e_configuration.resolution.fullscreen			= 0;
	e_configuration.resolution.opengl				= SDL_WINDOW_OPENGL;
	e_configuration.resolution.scaling				= WANDERLUST_SCALING_DEFAULT;
	e_configuration.resolution.scalingDefault		= WANDERLUST_SCALING_DEFAULT;
	e_configuration.resolution.scalingMin			= WANDERLUST_SCALING_MIN;
	e_configuration.resolution.scalingMax 			= WANDERLUST_SCALING_MAX;

	e_configuration.rendering.accelerated			= SDL_RENDERER_ACCELERATED;
	e_configuration.rendering.presentVSync			= SDL_RENDERER_PRESENTVSYNC;

	e_configuration.tileset.width					= WANDERLUST_TILE_DIMENSION;
	e_configuration.tileset.height					= WANDERLUST_TILE_DIMENSION;
	e_configuration.tileset.rows					= WANDERLUST_TILESET_ROWS;
	e_configuration.tileset.rowsDynamic2			= WANDERLUST_TILESET_ROWS_DYNAMIC_2;
	e_configuration.tileset.rowsDynamic2Iteration	= 2;
	e_configuration.tileset.rowsDynamic4			= WANDERLUST_TILESET_ROWS_DYNAMIC_4;
	e_configuration.tileset.rowsDynamic4Iteration	= 4;
	e_configuration.tileset.columns					= WANDERLUST_TILESET_COLUMNS;
	e_configuration.tileset.rate					= WANDERLUST_TILESET_DYNAMIC_RATE;
	e_configuration.tileset.types					= WANDERLUST_TILESET_TYPES;

	e_configuration.map.rows						= WANDERLUST_MAP_ROWS;
	e_configuration.map.columns						= WANDERLUST_MAP_COLUMNS;
	e_configuration.map.divisor						= WANDERLUST_MAP_DIVISOR;
	e_configuration.map.layer						= WANDERLUST_MAP_LAYER;
	e_configuration.map.layerTerrain				= WANDERLUST_MAP_LAYER_TERRAIN;
	e_configuration.map.layerEvent					= WANDERLUST_MAP_LAYER_EVENT;
	e_configuration.map.offsetRow					= WANDERLUST_MAP_OFFSET_ROW;
	e_configuration.map.offsetColumn				= WANDERLUST_MAP_OFFSET_COLUMN;

	e_configuration.map.stepping.walk				= WANDERLUST_MAP_STEPPING_WALK;
	e_configuration.map.stepping.ride				= WANDERLUST_MAP_STEPPING_RIDE;
	e_configuration.map.stepping.swim				= WANDERLUST_MAP_STEPPING_SWIM;

	e_configuration.map.terrain.water				= WANDERLUST_TERRAIN_WATER;
	e_configuration.map.terrain.entity				= WANDERLUST_TERRAIN_ENTITY;
	e_configuration.map.terrain.open				= WANDERLUST_TERRAIN_OPEN;

	e_configuration.drawing.r						= WANDERLUST_DRAW_COLOR_R;
	e_configuration.drawing.g						= WANDERLUST_DRAW_COLOR_G;
	e_configuration.drawing.b						= WANDERLUST_DRAW_COLOR_B;
	e_configuration.drawing.a						= WANDERLUST_DRAW_COLOR_A;

	e_configuration.state.locked					= WANDERLUST_STATE_LOCK;
	e_configuration.state.open						= WANDERLUST_STATE_OPEN;

	e_configuration.font.dimension					= WANDERLUST_FONT_DIMENSION;
	e_configuration.font.rows						= WANDERLUST_FONT_ROWS;
	e_configuration.font.columns					= WANDERLUST_FONT_COLS;
	e_configuration.font.depth						= WANDERLUST_FONT_DEPTH;

	e_configuration.direction.n						= WANDERLUST_DIRECTION_N;
	e_configuration.direction.e						= WANDERLUST_DIRECTION_E;
	e_configuration.direction.s						= WANDERLUST_DIRECTION_S;
	e_configuration.direction.w						= WANDERLUST_DIRECTION_W;

	e_configuration.button.up						= BUTTON_UP;
	e_configuration.button.down						= BUTTON_DOWN;
	e_configuration.button.left						= BUTTON_LEFT;
	e_configuration.button.right					= BUTTON_RIGHT;
	e_configuration.button.a						= BUTTON_A;
	e_configuration.button.b						= BUTTON_B;
	e_configuration.button.x						= BUTTON_X;
	e_configuration.button.y						= BUTTON_Y;
	e_configuration.button.start					= BUTTON_START;
	e_configuration.button.back						= BUTTON_BACK;

	e_configuration.morph.singlelinkedlist			= M_SINGLE_LINKED_LIST;
	e_configuration.morph.doublelinkedlist			= M_DOUBLE_LINKED_LIST;
	e_configuration.morph.array						= M_ARRAY;
	e_configuration.morph.stack						= M_STACK;
	e_configuration.morph.queue						= M_QUEUE;

	e_configuration.textbox.frame.x					= WANDERLUST_TEXTBOX_FRAME_X;
	e_configuration.textbox.frame.y					= WANDERLUST_TEXTBOX_FRAME_Y;
	e_configuration.textbox.frame.rows				= WANDERLUST_TEXTBOX_FRAME_R;
	e_configuration.textbox.frame.columns			= WANDERLUST_TEXTBOX_FRAME_C;
	e_configuration.textbox.frame.dimension			= WANDERLUST_TEXTBOX_FRAME_D;

	e_configuration.textbox.content.rows			= WANDERLUST_TEXTBOX_TEXT_LR;
	e_configuration.textbox.content.columns			= WANDERLUST_TEXTBOX_TEXT_LC;

	e_configuration.transition.width				= WANDERLUST_TRANSITION_SRC_W;
	e_configuration.transition.height				= WANDERLUST_TRANSITION_SRC_H;

	e_configuration.property.integer				= PROPERTY_INTEGER;
	e_configuration.property.string					= PROPERTY_STRING;
	e_configuration.property.point					= PROPERTY_POINT;
	e_configuration.property.rectangle				= PROPERTY_RECTANGLE;
	e_configuration.property.types					= PROPERTY_TYPES;

	return (f_initialized = WANDERLUST_SUCCESS);
}

static nil configuration_quit (lua_State * L)
{
	if (e_configuration.title)
	{
		SDL_free (e_configuration.title);
	}

	e_configuration.title 							= 0;
	e_configuration.fps								= 0;
	e_configuration.iteration						= 0;
	e_configuration.success							= 0;
	e_configuration.failure							= 0;
	e_configuration.flag							= 0;
	e_configuration.null							= 0;

	e_configuration.resolution.hidden 				= 0;
	e_configuration.resolution.fullscreen			= 0;
	e_configuration.resolution.opengl				= 0;
	e_configuration.resolution.scaling				= 0;
	e_configuration.resolution.scalingDefault		= 0;
	e_configuration.resolution.scalingMin			= 0;
	e_configuration.resolution.scalingMax 			= 0;

	e_configuration.rendering.accelerated			= 0;
	e_configuration.rendering.presentVSync			= 0;

	e_configuration.tileset.width					= 0;
	e_configuration.tileset.height					= 0;
	e_configuration.tileset.rows					= 0;
	e_configuration.tileset.rowsDynamic2			= 0;
	e_configuration.tileset.rowsDynamic2Iteration	= 0;
	e_configuration.tileset.rowsDynamic4			= 0;
	e_configuration.tileset.rowsDynamic4Iteration	= 0;
	e_configuration.tileset.columns					= 0;
	e_configuration.tileset.rate					= 0;
	e_configuration.tileset.types					= 0;

	e_configuration.map.rows						= 0;
	e_configuration.map.columns						= 0;
	e_configuration.map.divisor						= 0;
	e_configuration.map.layer						= 0;
	e_configuration.map.layerTerrain				= 0;
	e_configuration.map.layerEvent					= 0;
	e_configuration.map.offsetRow					= 0;
	e_configuration.map.offsetColumn				= 0;

	e_configuration.map.stepping.walk				= 0;
	e_configuration.map.stepping.ride				= 0;
	e_configuration.map.stepping.swim				= 0;

	e_configuration.map.terrain.water				= 0;
	e_configuration.map.terrain.entity				= 0;
	e_configuration.map.terrain.open				= 0;

	e_configuration.drawing.r						= 0;
	e_configuration.drawing.g						= 0;
	e_configuration.drawing.b						= 0;
	e_configuration.drawing.a						= 0;

	e_configuration.state.locked					= 0;
	e_configuration.state.open						= 0;

	e_configuration.font.dimension					= 0;
	e_configuration.font.rows						= 0;
	e_configuration.font.columns					= 0;
	e_configuration.font.depth						= 0;

	e_configuration.direction.n						= 0;
	e_configuration.direction.e						= 0;
	e_configuration.direction.s						= 0;
	e_configuration.direction.w						= 0;

	e_configuration.button.up						= 0;
	e_configuration.button.down						= 0;
	e_configuration.button.left						= 0;
	e_configuration.button.right					= 0;
	e_configuration.button.a						= 0;
	e_configuration.button.b						= 0;
	e_configuration.button.x						= 0;
	e_configuration.button.y						= 0;
	e_configuration.button.start					= 0;
	e_configuration.button.back						= 0;

	e_configuration.morph.singlelinkedlist			= 0;
	e_configuration.morph.doublelinkedlist			= 0;
	e_configuration.morph.array						= 0;
	e_configuration.morph.stack						= 0;
	e_configuration.morph.queue						= 0;

	e_configuration.transition.width				= 0;
	e_configuration.transition.height				= 0;

	e_configuration.textbox.frame.x					= 0;
	e_configuration.textbox.frame.y					= 0;
	e_configuration.textbox.frame.rows				= 0;
	e_configuration.textbox.frame.columns			= 0;
	e_configuration.textbox.frame.dimension			= 0;

	e_configuration.textbox.content.rows			= 0;
	e_configuration.textbox.content.columns			= 0;

	e_configuration.property.integer				= 0;
	e_configuration.property.string					= 0;
	e_configuration.property.point					= 0;
	e_configuration.property.rectangle				= 0;
	e_configuration.property.types					= 0;
}

/* ************************************************************************
 *
 * BANKS
 *
 * ************************************************************************/

static s08 (* f_init [])(lua_State *) =
{
	configuration_init,
	task_initialize,
	video_initialize,
	input_initialize,
	transition_initialize,
	state_initialize,
	entity_initialize,
	map_initialize,
	font_initialize,
	textbox_initialize,
	menu_initialize,
	terminal_initialize,
	mnemonic_initialize,
	assembler_initialize
};

static nil (* f_quit [])() =
{
	terminal_terminate,
	menu_terminate,
	textbox_terminate,
	font_terminate,
	map_terminate,
	entity_terminate,
	state_terminate,
	transition_terminate,
	input_terminate,
	video_terminate,
	task_terminate,
	configuration_quit,
	mnemonic_terminate,
	assembler_terminate
};

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

s08 configuration_bootstrap ()
{
	return configuration_init (0);
}

s08 configuration_initialize (lua_State * L)
{
	u32 i;

	if (! L)
	{
		return WANDERLUST_FAILURE;
	}

	e_configuration.flag = 0;

	for (i = 0; i < SS_SYSTEMS; i++)
	{
		if (f_init [i] (L) == WANDERLUST_SUCCESS)
		{
			e_configuration.flag += (1 << i);
		}
	}

	f_initialized = WANDERLUST_SUCCESS;

	return e_configuration.flag == SS_F_READY;
}

nil configuration_terminate ()
{
	u32 i;

	if (f_initialized == WANDERLUST_FAILURE)
	{
		return;
	}

	for (i = 0; i < SS_SYSTEMS; i++)
	{
		f_quit [i] ();
	}

	f_initialized = WANDERLUST_FAILURE;
}

wanderlust_config * configuration_get ()
{
	return & e_configuration;
}
