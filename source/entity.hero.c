#include "type.h"
#include "config.h"
#include "font.h"
#include "menu.h"
#include "input.h"
#include "texture.h"
#include "entity.h"
#include "event.h"
#include "state.h"
#include "task.h"
#include "transition.h"
#include "video.h"
#include "textbox.h"
#include "property.h"
#include "map.h"
#include "terminal.h"
#include "morph.h"

#include "entity.hero.h"

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

#define CALLBACK_INPUT_X		"on_key_x"

#define CALLBACK_INPUT_Y		"on_key_y"

#define CALLBACK_INPUT_START	"on_key_start"

#define CALLBACK_INPUT_BACK		"on_key_back"

/* ************************************************************************
 *
 * TYPEDEFINITIONS
 *
 * ***********************************************************************/

typedef enum
{
	WALK_IDLE_N = 0,
	WALK_NORTH_A,
	WALK_NORTH_B,
	WALK_IDLE_E,
	WALK_EAST_A,
	WALK_IDLE_S,
	WALK_SOUTH_A,
	WALK_SOUTH_B,
	WALK_IDLE_W,
	WALK_WEST_A,
	JUMP_SHADOW,
	RIDE_IDLE_N,
	RIDE_MOVE_N_A,
	RIDE_MOVE_N_B,
	RIDE_IDLE_E,
	RIDE_MOVE_E_A,
	RIDE_IDLE_S,
	RIDE_MOVE_S_A,
	RIDE_MOVE_S_B,
	RIDE_IDLE_W,
	RIDE_MOVE_W_A,
	HERO_SPRITE_LOCATIONS
} hero_sprite_location;

typedef enum
{
	ABILITY_CUT			= 0x01,
	ABILTIY_FLY			= 0x02,
	ABILITY_SURF		= 0x04,
	ABILITY_STRENGTH	= 0x08,
	ABILITY_FLASH		= 0x10,
	ABILITY_DIG			= 0x20,
	ABILITY_TELEPORT	= 0x40,
	ABILITY_SMASH		= 0x80
} hero_ability;

typedef struct
{
	u08 			state;
	u08 			iterator;
	u08				ability;
	u16 			steps;
	u32 			counter;
	lua_State	*	lua;
	struct
	{
		SDL_Rect	*	source;
		SDL_Rect		target;
	} shadow;
} hero_property;

typedef enum
{
	HERO_WALK_SEQA_N, HERO_WALK_SEQA_E, HERO_WALK_SEQA_S, HERO_WALK_SEQA_W,
	HERO_WALK_SEQB_N, HERO_WALK_SEQB_E, HERO_WALK_SEQB_S, HERO_WALK_SEQB_W,
	HERO_WALK_SEQC_N, HERO_WALK_SEQC_E, HERO_WALK_SEQC_S, HERO_WALK_SEQC_W,
	HERO_WALK_SEQD_N, HERO_WALK_SEQD_E, HERO_WALK_SEQD_S, HERO_WALK_SEQD_W,
	HERO_WALK_JUMP_N, HERO_WALK_JUMP_E, HERO_WALK_JUMP_S, HERO_WALK_JUMP_W,
	HERO_WALK_RISE_N, HERO_WALK_RISE_E, HERO_WALK_RISE_S, HERO_WALK_RISE_W,
	HERO_WALK_FALL_N, HERO_WALK_FALL_E, HERO_WALK_FALL_S, HERO_WALK_FALL_W,

	HERO_RIDE_SEQA_N, HERO_RIDE_SEQA_E, HERO_RIDE_SEQA_S, HERO_RIDE_SEQA_W,
	HERO_RIDE_SEQB_N, HERO_RIDE_SEQB_E, HERO_RIDE_SEQB_S, HERO_RIDE_SEQB_W,
	HERO_RIDE_SEQC_N, HERO_RIDE_SEQC_E, HERO_RIDE_SEQC_S, HERO_RIDE_SEQC_W,
	HERO_RIDE_SEQD_N, HERO_RIDE_SEQD_E, HERO_RIDE_SEQD_S, HERO_RIDE_SEQD_W,
	HERO_RIDE_SEQE_N, HERO_RIDE_SEQE_E, HERO_RIDE_SEQE_S, HERO_RIDE_SEQE_W,
	HERO_RIDE_SEQF_N, HERO_RIDE_SEQF_E, HERO_RIDE_SEQF_S, HERO_RIDE_SEQF_W,
	HERO_RIDE_SEQG_N, HERO_RIDE_SEQG_E, HERO_RIDE_SEQG_S, HERO_RIDE_SEQG_W,
	HERO_RIDE_SEQH_N, HERO_RIDE_SEQH_E, HERO_RIDE_SEQH_S, HERO_RIDE_SEQH_W,

	HERO_RIDE_JUMP_N, HERO_RIDE_JUMP_E, HERO_RIDE_JUMP_S, HERO_RIDE_JUMP_W,
	HERO_RIDE_RISE_N, HERO_RIDE_RISE_E, HERO_RIDE_RISE_S, HERO_RIDE_RISE_W,
	HERO_RIDE_FALL_N, HERO_RIDE_FALL_E, HERO_RIDE_FALL_S, HERO_RIDE_FALL_W,

	HERO_SURF_IDLE_N, HERO_SURF_IDLE_E, HERO_SURF_IDLE_S, HERO_SURF_IDLE_W,
	HERO_SURF_MOVE_N, HERO_SURF_MOVE_E, HERO_SURF_MOVE_S, HERO_SURF_MOVE_W,

	HERO_FISH_IDLE_N, HERO_FISH_IDLE_E, HERO_FISH_IDLE_S, HERO_FISH_IDLE_W,
	HERO_FISH_HITS_N, HERO_FISH_HITS_E, HERO_FISH_HITS_S, HERO_FISH_HITS_W,

	HERO_WALK_DOOR_N, HERO_WALK_DOOR_E, HERO_WALK_DOOR_S, HERO_WALK_DOOR_W,
	HERO_RIDE_DOOR_N, HERO_RIDE_DOOR_E, HERO_RIDE_DOOR_S, HERO_RIDE_DOOR_W,

	HERO_WALK_WAIT_N, HERO_WALK_WAIT_E, HERO_WALK_WAIT_S, HERO_WALK_WAIT_W,
	HERO_RIDE_WAIT_N, HERO_RIDE_WAIT_E, HERO_RIDE_WAIT_S, HERO_RIDE_WAIT_W,
	HERO_STATES
} hero_state;

typedef enum
{
	MODIFY_TITLE,
	MODIFY_ROW,
	MODIFY_COLUMN,
	MODIFY_DIRECTION,
	MODIFY_TEXTURE,
	MODIFIER_COUNT
}
hero_modifier;

/* ************************************************************************
 *
 * BANKS
 *
 * ***********************************************************************/

static SDL_Rect f_sprite_sources [HERO_SPRITE_LOCATIONS];

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - LUA CALLBACK
 *
 * ***********************************************************************/

static nil trigger_lua_callback (hero_property * p, const char * callback)
{
	lua_getglobal (p->lua, callback);

	if (lua_pcall (p->lua, 0, 0, 0))
	{
		return;
	}

	lua_settop (p->lua, 0);
}

static nil trigger_lua_callback_buttons (hero_property * p)
{
	if (input_get_buffered_button (e_configuration.button.x))
	{
		trigger_lua_callback (p, CALLBACK_INPUT_X);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.y))
	{
		trigger_lua_callback (p, CALLBACK_INPUT_Y);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.start))
	{
		trigger_lua_callback (p, CALLBACK_INPUT_START);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.back))
	{
		trigger_lua_callback (p, CALLBACK_INPUT_BACK);

		return;
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - SETUP
 *
 * ***********************************************************************/

static nil setup_walk_seqa_n (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_N];
	e->direction	= e_configuration.direction.n;
}

static nil setup_walk_seqa_e (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_E];
	e->direction	= e_configuration.direction.e;
}

static nil setup_walk_seqa_s (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_S];
	e->direction	= e_configuration.direction.s;
}

static nil setup_walk_seqa_w (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_W];
	e->direction	= e_configuration.direction.w;
}

static nil setup_walk_seqb_n (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_N];
	e->direction	= e_configuration.direction.n;
}

static nil setup_walk_seqb_e (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_E];
	e->direction	= e_configuration.direction.e;
}

static nil setup_walk_seqb_s (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_S];
	e->direction	= e_configuration.direction.s;
}

static nil setup_walk_seqb_w (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_W];
	e->direction	= e_configuration.direction.n;
}

static nil setup_walk_seqc_n (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [WALK_IDLE_N];
	e->direction			= e_configuration.direction.n;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_N);
}

static nil setup_walk_seqc_e (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [WALK_IDLE_E];
	e->direction			= e_configuration.direction.e;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_E);
}

static nil setup_walk_seqc_s (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [WALK_IDLE_S];
	e->direction			= e_configuration.direction.s;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);

	map_scroll (SCROLL_S);
}

static nil setup_walk_seqc_w (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [WALK_IDLE_W];
	e->direction			= e_configuration.direction.w;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);

	map_scroll (SCROLL_W);
}

static nil setup_walk_seqd_n (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [WALK_IDLE_N];
	e->direction			= e_configuration.direction.n;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_N);
}

static nil setup_walk_seqd_e (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [WALK_IDLE_E];
	e->direction			= e_configuration.direction.e;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_E);
}

static nil setup_walk_seqd_s (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [WALK_IDLE_S];
	e->direction			= e_configuration.direction.s;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_S);
}

static nil setup_walk_seqd_w (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [WALK_IDLE_W];
	e->direction			= e_configuration.direction.w;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_W);
}

static nil setup_walk_jump_n (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_N];
	e->direction	= e_configuration.direction.n;
}

static nil setup_walk_jump_e (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_E];
	e->direction	= e_configuration.direction.e;
}

static nil setup_walk_jump_s (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_S];
	e->direction	= e_configuration.direction.s;
}

static nil setup_walk_jump_w (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [WALK_IDLE_W];
	e->direction	= e_configuration.direction.w;
}

static nil setup_walk_rise_n (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [WALK_NORTH_A];
	e->direction			= e_configuration.direction.n;
	p->steps				= e_configuration.tileset.height;
	p->shadow.target.y	= e->target.y + (e_configuration.tileset.height / 2);
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_N);
}

static nil setup_walk_rise_e (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [WALK_EAST_A];
	e->direction			= e_configuration.direction.e;
	p->steps				= e_configuration.tileset.width;
	p->shadow.target.y	= e->target.y + (e_configuration.tileset.height/ 2);
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_E);
}

static nil setup_walk_rise_s (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [WALK_SOUTH_A];
	e->direction			= e_configuration.direction.s;
	p->steps				= e_configuration.tileset.height;
	p->shadow.target.y	= e->target.y + (e_configuration.tileset.height / 2);
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_S);
}

static nil setup_walk_rise_w (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [WALK_WEST_A];
	e->direction			= e_configuration.direction.w;
	p->steps				= e_configuration.tileset.width;
	p->shadow.target.y	= e->target.y + (e_configuration.tileset.width / 2);
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_W);
}

static nil setup_walk_fall_n (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [WALK_NORTH_B];
	e->direction			= e_configuration.direction.n;
	p->steps				= e_configuration.tileset.height;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_N);
}

static nil setup_walk_fall_e (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [WALK_IDLE_E];
	e->direction			= e_configuration.direction.e;
	p->steps				= e_configuration.tileset.width;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_E);
}

static nil setup_walk_fall_s (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [WALK_SOUTH_B];
	e->direction			= e_configuration.direction.s;
	p->steps				= e_configuration.tileset.height;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_S);
}

static nil setup_walk_fall_w (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [WALK_IDLE_W];
	e->direction			= e_configuration.direction.w;
	p->steps				= e_configuration.tileset.width;
	p->iterator			= e_configuration.map.stepping.walk;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_W);
}

static nil setup_ride_seqa_n (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_N];
	e->direction	= e_configuration.direction.n;
}

static nil setup_ride_seqa_e (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_E];
	e->direction	= e_configuration.direction.e;
}

static nil setup_ride_seqa_s (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_S];
	e->direction	= e_configuration.direction.s;
}

static nil setup_ride_seqa_w (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_W];
	e->direction	= e_configuration.direction.w;
}

static nil setup_ride_seqb_n (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_N];
	e->direction	= e_configuration.direction.n;
}

static nil setup_ride_seqb_e (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_E];
	e->direction	= e_configuration.direction.e;
}

static nil setup_ride_seqb_s (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_S];
	e->direction	= e_configuration.direction.s;
}

static nil setup_ride_seqb_w (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_W];
	e->direction	= e_configuration.direction.w;
}

static nil setup_ride_seqc_n (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_N];
	e->direction	= e_configuration.direction.n;
}

static nil setup_ride_seqc_e (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_E];
	e->direction	= e_configuration.direction.e;
}

static nil setup_ride_seqc_s (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_S];
	e->direction	= e_configuration.direction.s;
}

static nil setup_ride_seqc_w (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_W];
	e->direction	= e_configuration.direction.w;
}

static nil setup_ride_seqd_n (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_N];
	e->direction	= e_configuration.direction.n;
}

static nil setup_ride_seqd_e (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_E];
	e->direction	= e_configuration.direction.e;
}

static nil setup_ride_seqd_s (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_S];
	e->direction	= e_configuration.direction.s;
}

static nil setup_ride_seqd_w (wanderlust_entity * e, hero_property * p)
{
	e->source		= & f_sprite_sources [RIDE_IDLE_W];
	e->direction	= e_configuration.direction.w;
}

static nil setup_ride_seqe_n (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_MOVE_N_A];
	e->direction			= e_configuration.direction.n;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_N);
}

static nil setup_ride_seqe_e (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_MOVE_E_A];
	e->direction			= e_configuration.direction.e;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_E);
}

static nil setup_ride_seqe_s (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_MOVE_S_A];
	e->direction			= e_configuration.direction.s;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_S);
}

static nil setup_ride_seqe_w (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_MOVE_W_A];
	e->direction			= e_configuration.direction.w;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_W);
}

static nil setup_ride_seqf_n (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_IDLE_N];
	e->direction			= e_configuration.direction.n;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_N);
}

static nil setup_ride_seqf_e (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_IDLE_E];
	e->direction			= e_configuration.direction.e;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_E);
}

static nil setup_ride_seqf_s (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_IDLE_S];
	e->direction			= e_configuration.direction.s;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_S);
}

static nil setup_ride_seqf_w (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_IDLE_W];
	e->direction			= e_configuration.direction.w;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_W);
}

static nil setup_ride_seqg_n (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_MOVE_N_B];
	e->direction			= e_configuration.direction.n;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_N);
}

static nil setup_ride_seqg_e (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_MOVE_E_A];
	e->direction			= e_configuration.direction.e;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_E);
}

static nil setup_ride_seqg_s (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_MOVE_S_B];
	e->direction			= e_configuration.direction.s;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_S);
}

static nil setup_ride_seqg_w (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_MOVE_W_A];
	e->direction			= e_configuration.direction.w;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_W);
}

static nil setup_ride_seqh_n (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_IDLE_N];
	e->direction			= e_configuration.direction.n;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_N);
}

static nil setup_ride_seqh_e (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_IDLE_E];
	e->direction			= e_configuration.direction.e;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);

	map_scroll (SCROLL_E);
}

static nil setup_ride_seqh_s (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_IDLE_S];
	e->direction			= e_configuration.direction.s;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_S);
}

static nil setup_ride_seqh_w (wanderlust_entity * e, hero_property * p)
{
	e->source 			= & f_sprite_sources [RIDE_IDLE_W];
	e->direction			= e_configuration.direction.w;
	p->steps				= 0;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_W);
}

static nil setup_ride_jump_n (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_N];
	e->direction	= e_configuration.direction.n;
}

static nil setup_ride_jump_e (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_E];
	e->direction	= e_configuration.direction.e;
}

static nil setup_ride_jump_s (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_S];
	e->direction	= e_configuration.direction.s;
}

static nil setup_ride_jump_w (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_W];
	e->direction	= e_configuration.direction.w;
}

static nil setup_ride_rise_n (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [RIDE_MOVE_N_A];
	e->direction			= e_configuration.direction.n;
	p->steps				= e_configuration.tileset.height;
	p->shadow.target.y	= e->target.y + (e_configuration.tileset.height / 2);
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_N);
}

static nil setup_ride_rise_e (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [RIDE_MOVE_E_A];
	e->direction			= e_configuration.direction.e;
	p->steps				= e_configuration.tileset.width;
	p->shadow.target.y	= e->target.y + (e_configuration.tileset.width / 2);
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_E);
}

static nil setup_ride_rise_s (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [RIDE_MOVE_S_A];
	e->direction			= e_configuration.direction.s;
	p->steps				= e_configuration.tileset.height;
	p->shadow.target.y	= e->target.y + (e_configuration.tileset.height / 2);
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_S);
}

static nil setup_ride_rise_w (wanderlust_entity * e, hero_property * p)
{
	e->source				= & f_sprite_sources [RIDE_MOVE_W_A];
	e->direction			= e_configuration.direction.w;
	p->steps				= e_configuration.tileset.width;
	p->shadow.target.y	= e->target.y + (e_configuration.tileset.width / 2);
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_W);
}

static nil setup_ride_fall_n (wanderlust_entity * e, hero_property * p)
{
	e->direction			= e_configuration.direction.n;
	p->steps				= e_configuration.tileset.height;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_N);
}

static nil setup_ride_fall_e (wanderlust_entity * e, hero_property * p)
{
	e->direction			= e_configuration.direction.e;
	p->steps				= e_configuration.tileset.width;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_E);
}

static nil setup_ride_fall_s (wanderlust_entity * e, hero_property * p)
{
	e->direction			= e_configuration.direction.s;
	p->steps				= e_configuration.tileset.height;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_S);
}

static nil setup_ride_fall_w (wanderlust_entity * e, hero_property * p)
{
	e->direction			= e_configuration.direction.w;
	p->steps				= e_configuration.tileset.width;
	p->iterator			= e_configuration.map.stepping.ride;

	map_set_iterator (p->iterator);
	map_scroll (SCROLL_W);
}

static nil setup_surf_idle_n (wanderlust_entity * e, hero_property * p) { }

static nil setup_surf_idle_e (wanderlust_entity * e, hero_property * p) { }

static nil setup_surf_idle_s (wanderlust_entity * e, hero_property * p) { }

static nil setup_surf_idle_w (wanderlust_entity * e, hero_property * p) { }

static nil setup_surf_move_n (wanderlust_entity * e, hero_property * p) { }

static nil setup_surf_move_e (wanderlust_entity * e, hero_property * p) { }

static nil setup_surf_move_s (wanderlust_entity * e, hero_property * p) { }

static nil setup_surf_move_w (wanderlust_entity * e, hero_property * p) { }

static nil setup_fish_idle_n (wanderlust_entity * e, hero_property * p) { }

static nil setup_fish_idle_e (wanderlust_entity * e, hero_property * p) { }

static nil setup_fish_idle_s (wanderlust_entity * e, hero_property * p) { }

static nil setup_fish_idle_w (wanderlust_entity * e, hero_property * p) { }

static nil setup_fish_hits_n (wanderlust_entity * e, hero_property * p) { }

static nil setup_fish_hits_e (wanderlust_entity * e, hero_property * p) { }

static nil setup_fish_hits_s (wanderlust_entity * e, hero_property * p) { }

static nil setup_fish_hits_w (wanderlust_entity * e, hero_property * p) { }

static nil setup_walk_door_n (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_N];
	e->direction	= e_configuration.direction.n;
}

static nil setup_walk_door_e (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_E];
	e->direction	= e_configuration.direction.e;
}

static nil setup_walk_door_s (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_S];
	e->direction	= e_configuration.direction.s;
}

static nil setup_walk_door_w (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_W];
	e->direction	= e_configuration.direction.w;
}

static nil setup_ride_door_n (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_N];
	e->direction	= e_configuration.direction.n;
}

static nil setup_ride_door_e (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_E];
	e->direction	= e_configuration.direction.e;
}

static nil setup_ride_door_s (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_S];
	e->direction	= e_configuration.direction.s;
}

static nil setup_ride_door_w (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_W];
	e->direction	= e_configuration.direction.w;
}

static nil setup_walk_wait_n (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_N];
	e->direction	= e_configuration.direction.n;
	p->steps		= e_configuration.tileset.height / 2;
}

static nil setup_walk_wait_e (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_E];
	e->direction	= e_configuration.direction.e;
	p->steps		= e_configuration.tileset.width / 2;
}

static nil setup_walk_wait_s (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_S];
	e->direction	= e_configuration.direction.s;
	p->steps		= e_configuration.tileset.height / 2;
}

static nil setup_walk_wait_w (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [WALK_IDLE_W];
	e->direction	= e_configuration.direction.w;
	p->steps		= e_configuration.tileset.width / 2;
}

static nil setup_ride_wait_n (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_N];
	e->direction	= e_configuration.direction.n;
	p->steps		= e_configuration.tileset.height / 2;
}

static nil setup_ride_wait_e (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_E];
	e->direction	= e_configuration.direction.e;
	p->steps		= e_configuration.tileset.width / 2;
}

static nil setup_ride_wait_s (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_S];
	e->direction	= e_configuration.direction.s;
	p->steps		= e_configuration.tileset.height / 2;
}

static nil setup_ride_wait_w (wanderlust_entity * e, hero_property * p)
{
	e->source 	= & f_sprite_sources [RIDE_IDLE_W];
	e->direction	= e_configuration.direction.w;
	p->steps		= e_configuration.tileset.width / 2;
}


/* ************************************************************************
 *
 * BANK - SETUP
 *
 * ***********************************************************************/

static nil (* setup [HERO_STATES]) (wanderlust_entity *, hero_property *) =
{
	setup_walk_seqa_n, setup_walk_seqa_e, setup_walk_seqa_s, setup_walk_seqa_w,
	setup_walk_seqb_n, setup_walk_seqb_e, setup_walk_seqb_s, setup_walk_seqb_w,
	setup_walk_seqc_n, setup_walk_seqc_e, setup_walk_seqc_s, setup_walk_seqc_w,
	setup_walk_seqd_n, setup_walk_seqd_e, setup_walk_seqd_s, setup_walk_seqd_w,
	setup_walk_jump_n, setup_walk_jump_e, setup_walk_jump_s, setup_walk_jump_w,
	setup_walk_rise_n, setup_walk_rise_e, setup_walk_rise_s, setup_walk_rise_w,
	setup_walk_fall_n, setup_walk_fall_e, setup_walk_fall_s, setup_walk_fall_w,

	setup_ride_seqa_n, setup_ride_seqa_e, setup_ride_seqa_s, setup_ride_seqa_w,
	setup_ride_seqb_n, setup_ride_seqb_e, setup_ride_seqb_s, setup_ride_seqb_w,
	setup_ride_seqc_n, setup_ride_seqc_e, setup_ride_seqc_s, setup_ride_seqc_w,
	setup_ride_seqd_n, setup_ride_seqd_e, setup_ride_seqd_s, setup_ride_seqd_w,

	setup_ride_seqe_n, setup_ride_seqe_e, setup_ride_seqe_s, setup_ride_seqe_w,
	setup_ride_seqf_n, setup_ride_seqf_e, setup_ride_seqf_s, setup_ride_seqf_w,
	setup_ride_seqg_n, setup_ride_seqg_e, setup_ride_seqg_s, setup_ride_seqg_w,
	setup_ride_seqh_n, setup_ride_seqh_e, setup_ride_seqh_s, setup_ride_seqh_w,

	setup_ride_jump_n, setup_ride_jump_e, setup_ride_jump_s, setup_ride_jump_w,
	setup_ride_rise_n, setup_ride_rise_e, setup_ride_rise_s, setup_ride_rise_w,
	setup_ride_fall_n, setup_ride_fall_e, setup_ride_fall_s, setup_ride_fall_w,

	setup_surf_idle_n, setup_surf_idle_e, setup_surf_idle_s, setup_surf_idle_w,
	setup_surf_move_n, setup_surf_move_e, setup_surf_move_s, setup_surf_move_w,

	setup_fish_idle_n, setup_fish_idle_e, setup_fish_idle_s, setup_fish_idle_w,
	setup_fish_hits_n, setup_fish_hits_e, setup_fish_hits_s, setup_fish_hits_w,

	setup_walk_door_n, setup_walk_door_e, setup_walk_door_s, setup_walk_door_w,
	setup_ride_door_n, setup_ride_door_e, setup_ride_door_s, setup_ride_door_w,

	setup_walk_wait_n, setup_walk_wait_e, setup_walk_wait_s, setup_walk_wait_w,
	setup_ride_wait_n, setup_ride_wait_e, setup_ride_wait_s, setup_ride_wait_w
};

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - MODIFIY
 *
 * ***********************************************************************/

static nil modify_title (wanderlust_entity * e, lua_State * L, u16 index)
{
	s08 * title = 0;

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

static nil modify_direction_n (wanderlust_entity * e, hero_property * p)
{
	switch (p->state)
	{
		case HERO_WALK_SEQA_E: case HERO_WALK_SEQA_S: case HERO_WALK_SEQA_W:
		case HERO_WALK_DOOR_E: case HERO_WALK_DOOR_S: case HERO_WALK_DOOR_W:

			setup [(p->state = HERO_WALK_SEQA_N)] (e, p);

			break;

		case HERO_WALK_SEQB_E: case HERO_WALK_SEQB_S: case HERO_WALK_SEQB_W:

			setup [(p->state = HERO_WALK_SEQB_N)] (e, p);

			break;

		case HERO_RIDE_SEQA_E: case HERO_RIDE_SEQA_S: case HERO_RIDE_SEQA_W:
		case HERO_RIDE_DOOR_E: case HERO_RIDE_DOOR_S: case HERO_RIDE_DOOR_W:

			setup [(p->state = HERO_RIDE_SEQA_N)] (e, p);

			break;

		case HERO_RIDE_SEQB_E: case HERO_RIDE_SEQB_S: case HERO_RIDE_SEQB_W:

			setup [(p->state = HERO_RIDE_SEQB_N)] (e, p);

			break;

		case HERO_RIDE_SEQC_E: case HERO_RIDE_SEQC_S: case HERO_RIDE_SEQC_W:

			setup [(p->state = HERO_RIDE_SEQC_N)] (e, p);

			break;

		case HERO_RIDE_SEQD_E: case HERO_RIDE_SEQD_S: case HERO_RIDE_SEQD_W:

			setup [(p->state = HERO_RIDE_SEQD_N)] (e, p);

			break;

		default:
			break;
	}
}

static nil modify_direction_e (wanderlust_entity * e, hero_property * p)
{
	switch (p->state)
	{
		case HERO_WALK_SEQA_N: case HERO_WALK_SEQA_S: case HERO_WALK_SEQA_W:
		case HERO_WALK_DOOR_N: case HERO_WALK_DOOR_S: case HERO_WALK_DOOR_W:

			setup [(p->state = HERO_WALK_SEQA_E)] (e, p);

			break;

		case HERO_WALK_SEQB_N: case HERO_WALK_SEQB_S: case HERO_WALK_SEQB_W:

			setup [(p->state = HERO_WALK_SEQB_E)] (e, p);

			break;

		case HERO_RIDE_SEQA_N: case HERO_RIDE_SEQA_S: case HERO_RIDE_SEQA_W:
		case HERO_RIDE_DOOR_N: case HERO_RIDE_DOOR_S: case HERO_RIDE_DOOR_W:

			setup [(p->state = HERO_RIDE_SEQA_E)] (e, p);

			break;

		case HERO_RIDE_SEQB_N: case HERO_RIDE_SEQB_S: case HERO_RIDE_SEQB_W:

			setup [(p->state = HERO_RIDE_SEQB_E)] (e, p);

			break;

		case HERO_RIDE_SEQC_N: case HERO_RIDE_SEQC_S: case HERO_RIDE_SEQC_W:

			setup [(p->state = HERO_RIDE_SEQC_E)] (e, p);

			break;

		case HERO_RIDE_SEQD_N: case HERO_RIDE_SEQD_S: case HERO_RIDE_SEQD_W:

			setup [(p->state = HERO_RIDE_SEQD_E)] (e, p);

			break;

		default:

			break;
	}
}

static nil modify_direction_s (wanderlust_entity * e, hero_property * p)
{
	switch (p->state)
	{
		case HERO_WALK_SEQA_N: case HERO_WALK_SEQA_E: case HERO_WALK_SEQA_W:
		case HERO_WALK_DOOR_N: case HERO_WALK_DOOR_E: case HERO_WALK_DOOR_W:

			setup [(p->state = HERO_WALK_SEQA_S)] (e, p);

			break;

		case HERO_WALK_SEQB_N: case HERO_WALK_SEQB_E: case HERO_WALK_SEQB_W:

			setup [(p->state = HERO_WALK_SEQB_S)] (e, p);

			break;

		case HERO_RIDE_SEQA_N: case HERO_RIDE_SEQA_E: case HERO_RIDE_SEQA_W:
		case HERO_RIDE_DOOR_N: case HERO_RIDE_DOOR_E: case HERO_RIDE_DOOR_W:

			setup [(p->state = HERO_RIDE_SEQA_S)] (e, p);

			break;

		case HERO_RIDE_SEQB_N: case HERO_RIDE_SEQB_E: case HERO_RIDE_SEQB_W:

			setup [(p->state = HERO_RIDE_SEQB_S)] (e, p);

			break;

		case HERO_RIDE_SEQC_N: case HERO_RIDE_SEQC_E: case HERO_RIDE_SEQC_W:

			setup [(p->state = HERO_RIDE_SEQC_S)] (e, p);

			break;

		case HERO_RIDE_SEQD_N: case HERO_RIDE_SEQD_E: case HERO_RIDE_SEQD_W:

			setup [(p->state = HERO_RIDE_SEQD_S)] (e, p);

			break;

		default:

			break;
	}
}

static nil modify_direction_w (wanderlust_entity * e, hero_property * p)
{
	switch (p->state)
	{
		case HERO_WALK_SEQA_N: case HERO_WALK_SEQA_E: case HERO_WALK_SEQA_S:
		case HERO_WALK_DOOR_N: case HERO_WALK_DOOR_E: case HERO_WALK_DOOR_S:

			setup [(p->state = HERO_WALK_SEQA_W)] (e, p);

			break;

		case HERO_WALK_SEQB_N: case HERO_WALK_SEQB_E: case HERO_WALK_SEQB_S:

			setup [(p->state = HERO_WALK_SEQB_W)] (e, p);

			break;

		case HERO_RIDE_SEQA_N: case HERO_RIDE_SEQA_E: case HERO_RIDE_SEQA_S:
		case HERO_RIDE_DOOR_N: case HERO_RIDE_DOOR_E: case HERO_RIDE_DOOR_S:

			setup [(p->state = HERO_RIDE_SEQA_W)] (e, p);

			break;

		case HERO_RIDE_SEQB_N: case HERO_RIDE_SEQB_E: case HERO_RIDE_SEQB_S:

			setup [(p->state = HERO_RIDE_SEQB_W)] (e, p);

			break;

		case HERO_RIDE_SEQC_N: case HERO_RIDE_SEQC_E: case HERO_RIDE_SEQC_S:

			setup [(p->state = HERO_RIDE_SEQC_W)] (e, p);

			break;

		case HERO_RIDE_SEQD_N: case HERO_RIDE_SEQD_E: case HERO_RIDE_SEQD_S:

			setup [(p->state = HERO_RIDE_SEQD_W)] (e, p);

			break;

		default:

			break;
	}
}

static nil modify_direction (wanderlust_entity * e, lua_State * L, u16 index)
{
	u08 				direction 	= 0;
	hero_property	* 	p			= e->properties;

	if (! lua_isnumber (L, index))
	{
		return;
	}

	if ((direction = (u08) lua_tointeger (L, index)) >= WANDERLUST_DIRECTIONS)
	{
		return;
	}

	switch (direction)
	{
		case WANDERLUST_DIRECTION_N:

			modify_direction_n (e, p);

			break;

		case WANDERLUST_DIRECTION_E:

			modify_direction_e (e, p);

			break;

		case WANDERLUST_DIRECTION_S:

			modify_direction_s (e, p);

			break;

		case WANDERLUST_DIRECTION_W:

			modify_direction_w (e, p);

			break;

		default:

			break;
	}
}

static nil modify_texture (wanderlust_entity * e, lua_State * L, u16 index)
{
	SDL_Texture	* t = 0;

	if (! lua_isstring (L, index))
	{
		return;
	}

	if (! (t = texture_get_by_name (lua_tostring (L, index))))
	{
		return;
	}

	e->texture = t;
}

static nil event_door (nil * item)
{
	wanderlust_entity 	* e = item;
	hero_property		* p;

	if (! e)
	{
		return;
	}

	if (! (p = e->properties))
	{
		return;
	}

	switch (p->state)
	{
		case HERO_WALK_SEQA_N:
		case HERO_WALK_SEQB_N:
			setup [(p->state = HERO_WALK_DOOR_N)] (e, p);
			break;

		case HERO_WALK_SEQA_E:
		case HERO_WALK_SEQB_E:
			setup [(p->state = HERO_WALK_DOOR_E)] (e, p);
			break;

		case HERO_WALK_SEQA_S:
		case HERO_WALK_SEQB_S:
			setup [(p->state = HERO_WALK_DOOR_S)] (e, p);
			break;

		case HERO_WALK_SEQA_W:
		case HERO_WALK_SEQB_W:
			setup [(p->state = HERO_WALK_DOOR_W)] (e, p);
			break;

		case HERO_RIDE_SEQA_N:
		case HERO_RIDE_SEQB_N:
		case HERO_RIDE_SEQC_N:
		case HERO_RIDE_SEQD_N:
			setup [(p->state = HERO_RIDE_DOOR_N)] (e, p);
			break;

		case HERO_RIDE_SEQA_E:
		case HERO_RIDE_SEQB_E:
		case HERO_RIDE_SEQC_E:
		case HERO_RIDE_SEQD_E:
			setup [(p->state = HERO_RIDE_DOOR_E)] (e, p);
			break;

		case HERO_RIDE_SEQA_S:
		case HERO_RIDE_SEQB_S:
		case HERO_RIDE_SEQC_S:
		case HERO_RIDE_SEQD_S:
			setup [(p->state = HERO_RIDE_DOOR_S)] (e, p);
			break;

		case HERO_RIDE_SEQA_W:
		case HERO_RIDE_SEQB_W:
		case HERO_RIDE_SEQC_W:
		case HERO_RIDE_SEQD_W:
			setup [(p->state = HERO_RIDE_DOOR_W)] (e, p);
			break;
	}
}

static nil event_sign (nil * item)
{
	wanderlust_entity	* e = item;
	hero_property		* p	= 0;

	if (! e)
	{
		return;
	}

	if (! (p = e->properties))
	{
		return;
	}

	switch (p->state)
	{
		case HERO_WALK_SEQA_N: case HERO_WALK_SEQB_N: case HERO_RIDE_SEQA_N:
		case HERO_RIDE_SEQB_N: case HERO_RIDE_SEQC_N: case HERO_RIDE_SEQD_N:

			if (map_lua_push_number (e->column) == e_configuration.failure)
			{
				return;
			}

			if (map_lua_push_number (e->row - 1) == e_configuration.failure)
			{
				return;
			}

			break;

		default:
			if (map_lua_push_number (0x100) == e_configuration.failure)
			{
				return;
			}

			if (map_lua_push_number (0x100) == e_configuration.failure)
			{
				return;
			}
	}
}

static nil event_jump (nil * item)
{
	wanderlust_entity 	* e = item;
	hero_property		* p;

	if (! e)
	{
		return;
	}

	if (! (p = e->properties))
	{
		return;
	}

	switch (p->state)
	{
	case HERO_WALK_SEQA_N:
	case HERO_WALK_SEQB_N:

		p->state = HERO_WALK_JUMP_N;

		break;

	case HERO_WALK_SEQA_E:
	case HERO_WALK_SEQB_E:

		p->state = HERO_WALK_JUMP_E;

		break;

	case HERO_WALK_SEQA_S:
	case HERO_WALK_SEQB_S:

		p->state = HERO_WALK_JUMP_S;

		break;

	case HERO_WALK_SEQA_W:
	case HERO_WALK_SEQB_W:

		p->state = HERO_WALK_JUMP_W;

		break;

	case HERO_RIDE_SEQA_N:
	case HERO_RIDE_SEQB_N:
	case HERO_RIDE_SEQC_N:
	case HERO_RIDE_SEQD_N:

		p->state = HERO_RIDE_JUMP_N;

		break;

	case HERO_RIDE_SEQA_E:
	case HERO_RIDE_SEQB_E:
	case HERO_RIDE_SEQC_E:
	case HERO_RIDE_SEQD_E:

		p->state = HERO_RIDE_JUMP_E;

		break;

	case HERO_RIDE_SEQA_S:
	case HERO_RIDE_SEQB_S:
	case HERO_RIDE_SEQC_S:
	case HERO_RIDE_SEQD_S:

		p->state = HERO_RIDE_JUMP_S;

		break;

	case HERO_RIDE_SEQA_W:
	case HERO_RIDE_SEQB_W:
	case HERO_RIDE_SEQC_W:
	case HERO_RIDE_SEQD_W:

		p->state = HERO_RIDE_JUMP_W;

		break;
	}
}

static nil event_surf (nil * item)
{
	wanderlust_entity	* e = item;
	hero_property		* p;

	if (! e)
	{
		return;
	}

	if (! (p = e->properties))
	{
		return;
	}
}

static nil event_jump_n (nil * item)
{
	wanderlust_entity	* e = item;
	hero_property		* p;

	if (! e)
	{
		return;
	}

	if (! (p = e->properties))
	{
		return;
	}

	switch (p->state)
	{
	case HERO_WALK_JUMP_N:

		setup [(p->state = HERO_WALK_RISE_N)] (e, p);

		break;

	case HERO_RIDE_JUMP_N:

		setup [(p->state = HERO_RIDE_RISE_N)] (e, p);

		break;

	default:

		return;
	}
}

static nil event_jump_e (nil * item)
{
	wanderlust_entity	* e = item;
	hero_property		* p;

	if (! e)
	{
		return;
	}

	if (! (p = e->properties))
	{
		return;
	}

	switch (p->state)
	{
	case HERO_WALK_JUMP_E:

		setup [(p->state = HERO_WALK_RISE_E)] (e, p);

		break;

	case HERO_RIDE_JUMP_E:

		setup [(p->state = HERO_RIDE_RISE_E)] (e, p);

		break;
	}
}

static nil event_jump_s (nil * item)
{
	wanderlust_entity	* e = item;
	hero_property		* p;

	if (! e)
	{
		return;
	}

	if (! (p = e->properties))
	{
		return;
	}

	switch (p->state)
	{
	case HERO_WALK_JUMP_S:

		setup [(p->state = HERO_WALK_RISE_S)] (e, p);

		break;

	case HERO_RIDE_JUMP_S:

		setup [(p->state = HERO_RIDE_RISE_S)] (e, p);

		break;
	}
}

static nil event_jump_w (nil * item)
{
	wanderlust_entity	* e = item;
	hero_property		* p;

	if (! e)
	{
		return;
	}

	if (! (p = e->properties))
	{
		return;
	}

	switch (p->state)
	{
		case HERO_WALK_JUMP_W:

			setup [(p->state = HERO_WALK_RISE_W)] (e, p);

			break;

		case HERO_RIDE_JUMP_W:

			setup [(p->state = HERO_RIDE_RISE_W)] (e, p);

			break;
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - HANDLE INPUT
 *
 * ***********************************************************************/

static nil handle_walk_seqa_n (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		point.x = e->column;
		point.y = e->row - 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, -1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_N)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_SEQA_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_SEQA_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_SEQA_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, -1, 0))
		{
			map_trigger_event (e->row, e->column, -1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row - 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_SEQA_N)] (e, p);

		return;
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_seqa_e (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point = {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_SEQA_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_SEQA_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_SEQA_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		point.x = e->column + 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, 1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_E)] (e, p);
		}

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, 1))
		{
			map_trigger_event (e->row, e->column, 0, 1, e);

			return;
		}

		point.x = e->column + 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_SEQA_E)] (e, p);

		return;
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_seqa_s (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point = {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_SEQA_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		point.x = e->column;
		point.y = e->row + 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_S)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_SEQA_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_SEQA_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 1, 0))
		{
			map_trigger_event (e->row, e->column, 1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row + 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_SEQA_S)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_seqa_w (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_SEQA_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_SEQA_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		point.x = e->column - 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, -1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_W)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_SEQA_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, -1))
		{
			map_trigger_event (e->row, e->column, 0, -1, e);

			return;
		}

		point.x = e->column - 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_SEQA_W)] (e, p);

		return;
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_seqb_n (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point = {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		point.x = e->column;
		point.y = e->row - 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, -1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQD_N)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_SEQB_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_SEQB_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_SEQB_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, -1, 0))
		{
			map_trigger_event (e->row, e->column, -1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row - 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_SEQB_N)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_seqb_e (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_SEQB_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_SEQB_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_SEQB_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		point.x = e->column + 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, 1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQD_E)] (e, p);
		}

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, 1))
		{
			map_trigger_event (e->row, e->column, 0, 1, e);

			return;
		}

		point.x = e->column + 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_SEQB_E)] (e, p);

		return;
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_seqb_s (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_SEQB_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		point.x = e->column;
		point.y = e->row + 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQD_S)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_SEQB_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_SEQB_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 1, 0))
		{
			map_trigger_event (e->row, e->column, 1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row + 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_SEQB_S)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_seqb_w (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_SEQB_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_SEQB_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		point.x = e->column - 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, -1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQD_W)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{

		setup [(p->state = HERO_WALK_SEQB_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, -1))
		{
			map_trigger_event (e->row, e->column, 0, -1, e);

			return;
		}

		point.x = e->column - 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_SEQB_W)] (e, p);

		return;
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_seqc_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_seqc_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_seqc_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_seqc_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_seqd_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_seqd_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_seqd_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_seqd_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_jump_n (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		if (map_has_event (e->row, e->column, -1, 0))
		{
			if (map_get_event_id (e->row, e->column, -1, 0) == EVENT_JUMP_N)
			{
				map_trigger_event (e->row, e->column, -1, 0, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, -1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_N)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_JUMP_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_JUMP_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_JUMP_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_JUMP_N)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_jump_e (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_JUMP_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_JUMP_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_JUMP_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		if (map_has_event (e->row, e->column, 0, 1))
		{
			if (map_get_event_id (e->row, e->column, 0, 1) == EVENT_JUMP_E)
			{
				map_trigger_event (e->row, e->column, 0, 1, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 0, 1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_E)] (e, p);
		}

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_JUMP_E)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_jump_s (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_JUMP_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		if (map_has_event (e->row, e->column, 1, 0))
		{
			if (map_get_event_id (e->row, e->column, 1, 0) == EVENT_JUMP_S)
			{
				map_trigger_event (e->row, e->column, 1, 0, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_S)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_JUMP_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_JUMP_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_JUMP_S)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_jump_w (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_JUMP_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_JUMP_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		if (map_has_event (e->row, e->column, 0, -1))
		{
			if (map_get_event_id (e->row, e->column, 0, -1) == EVENT_JUMP_W)
			{
				map_trigger_event (e->row, e->column, 0, -1, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 0, -1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_W)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_JUMP_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_RIDE_JUMP_W)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_rise_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_rise_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_rise_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_rise_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_fall_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_fall_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_fall_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_fall_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqa_n (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		point.x = e->column;
		point.y = e->row - 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, -1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQE_N)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQA_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQA_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQA_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, -1, 0))
		{
			map_trigger_event (e->row, e->column, -1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row - 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQA_N)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqa_e (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQA_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		point.x = e->column + 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, 1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQE_E)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQA_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQA_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, 1))
		{
			map_trigger_event (e->row, e->column, 0, 1, e);

			return;
		}

		point.x = e->column + 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQA_E)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqa_s (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQA_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQA_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		point.x = e->column;
		point.y = e->row + 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQE_S)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQA_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 1, 0))
		{
			map_trigger_event (e->row, e->column, 1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row + 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQA_S)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqa_w (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQA_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQA_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQA_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		point.x = e->column - 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, -1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQE_W)] (e, p);
		}

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, -1))
		{
			map_trigger_event (e->row, e->column, 0, -1, e);

			return;
		}

		point.x = e->column - 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQA_W)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqb_n (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		point.x = e->column;
		point.y = e->row - 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, -1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQF_N)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQB_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQB_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQB_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, -1, 0))
		{
			map_trigger_event (e->row, e->column, -1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row - 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_N)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqb_e (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQB_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		point.x = e->column + 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, 1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQF_E)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQB_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQB_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, 1))
		{
			map_trigger_event (e->row, e->column, 0, 1, e);

			return;
		}

		point.x = e->column + 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_E)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqb_s (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQB_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQB_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		point.x = e->column;
		point.y = e->row + 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQF_S)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQB_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 1, 0))
		{
			map_trigger_event (e->row, e->column, 1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row + 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_S)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqb_w (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQB_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQB_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQB_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		point.x = e->column - 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, -1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQF_W)] (e, p);
		}

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, -1))
		{
			map_trigger_event (e->row, e->column, 0, -1, e);

			return;
		}

		point.x = e->column - 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_W)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqc_n (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		point.x = e->column;
		point.y = e->row - 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, -1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQG_N)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQC_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQC_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQC_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, -1, 0))
		{
			map_trigger_event (e->row, e->column, -1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row - 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_N)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqc_e (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQC_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		point.x = e->column + 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, 1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQG_E)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQC_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQC_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, 1))
		{
			map_trigger_event (e->row, e->column, 0, 1, e);

			return;
		}

		point.x = e->column + 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_E)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqc_s (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQC_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQC_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		point.x = e->column;
		point.y = e->row + 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQG_S)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQC_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 1, 0))
		{
			map_trigger_event (e->row, e->column, 1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row + 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_S)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqc_w (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQC_N)] (e, p);
		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQC_E)] (e, p);
		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQC_S)] (e, p);
		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		point.x = e->column - 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, -1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQG_W)] (e, p);
		}

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, -1))
		{
			map_trigger_event (e->row, e->column, 0, -1, e);

			return;
		}

		point.x = e->column - 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_W)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqd_n (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		point.x = e->column;
		point.y = e->row - 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, -1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQH_N)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQD_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQD_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQD_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, -1, 0))
		{
			map_trigger_event (e->row, e->column, -1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row - 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_N)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqd_e (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQD_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		point.x = e->column + 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, 1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQH_E)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQD_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQD_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, 1))
		{
			map_trigger_event (e->row, e->column, 0, 1, e);

			return;
		}

		point.x = e->column + 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_E)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqd_s (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQD_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQD_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		point.x = e->column;
		point.y = e->row + 1;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQH_S)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQD_W)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 1, 0))
		{
			map_trigger_event (e->row, e->column, 1, 0, e);

			return;
		}

		point.x = e->column;
		point.y = e->row + 1;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_S)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqd_w (wanderlust_entity * e, hero_property * p)
{
	SDL_Point point	= {0, 0};

	if (map_has_event (e->row, e->column, 0, 0))
	{
		map_trigger_event (e->row, e->column, 0, 0, e);
	}

	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQD_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQD_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQD_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		point.x = e->column - 1;
		point.y = e->row;

		if (state_entity_has_collision (& point))
		{
			return;
		}

		if (map_get_terrain (e->row, e->column, 0, -1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQH_W)] (e, p);
		}

		return;
	}

	if (input_get_buffered_button (e_configuration.button.a))
	{
		if (map_has_event (e->row, e->column, 0, -1))
		{
			map_trigger_event (e->row, e->column, 0, -1, e);

			return;
		}

		point.x = e->column - 1;
		point.y = e->row;

		state_entity_excite (& point);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_SEQB_W)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_seqe_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqe_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqe_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqe_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqf_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqf_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqf_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqf_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqg_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqg_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqg_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqg_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqh_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqh_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqh_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_seqh_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_jump_n (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		if (map_has_event (e->row, e->column, -1, 0))
		{
			if (map_get_event_id (e->row, e->column, -1, 0) == EVENT_JUMP_N)
			{
				map_trigger_event (e->row, e->column, -1, 0, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, -1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQC_N)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_JUMP_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_JUMP_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_JUMP_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state 	= HERO_WALK_JUMP_N)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_jump_e (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_JUMP_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_JUMP_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_JUMP_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		if (map_has_event (e->row, e->column, 0, 1))
		{
			if (map_get_event_id (e->row, e->column, 0, 1) == EVENT_JUMP_E)
			{
				map_trigger_event (e->row, e->column, 0, 1, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 0, 1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQC_E)] (e, p);
		}

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_JUMP_E)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_jump_s (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_JUMP_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		if (map_has_event (e->row, e->column, 1, 0))
		{
			if (map_get_event_id (e->row, e->column, 1, 0) == EVENT_JUMP_S)
			{
				map_trigger_event (e->row, e->column, 1, 0, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQC_S)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_JUMP_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_JUMP_E)] (e, p);

		return;
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_jump_w (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_JUMP_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_JUMP_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		if (map_has_event (e->row, e->column, 0, -1))
		{
			if (map_get_event_id (e->row, e->column, 0, -1) == EVENT_JUMP_W)
			{
				map_trigger_event (e->row, e->column, 0, -1, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 0, -1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQC_W)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_JUMP_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state = HERO_WALK_JUMP_W)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_rise_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_rise_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_rise_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_rise_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_fall_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_fall_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_fall_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_ride_fall_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_surf_idle_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_surf_idle_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_surf_idle_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_surf_idle_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_surf_move_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_surf_move_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_surf_move_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_surf_move_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_fish_idle_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_fish_idle_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_fish_idle_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_fish_idle_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_fish_hits_n (wanderlust_entity * e, hero_property * p) { }

static nil handle_fish_hits_e (wanderlust_entity * e, hero_property * p) { }

static nil handle_fish_hits_s (wanderlust_entity * e, hero_property * p) { }

static nil handle_fish_hits_w (wanderlust_entity * e, hero_property * p) { }

static nil handle_walk_door_n (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		if (map_has_event (e->row, e->column, -1, 0))
		{
			if (map_get_event_id (e->row, e->column, -1, 0) == EVENT_DOOR_N)
			{
				map_trigger_event (e->row, e->column, -1, 0, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, -1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_N)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_DOOR_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_DOOR_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_DOOR_E)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.b))
	{
		setup [(p->state 	= HERO_RIDE_DOOR_N)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_door_e (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_DOOR_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_DOOR_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_DOOR_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		if (map_has_event (e->row, e->column, 0, 1))
		{
			if (map_get_event_id (e->row, e->column, 0, 1) == EVENT_DOOR_E)
			{
				map_trigger_event (e->row, e->column, 0, 1, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 0, 1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_E)] (e, p);
		}

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state 	= HERO_RIDE_DOOR_E)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_door_s (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_DOOR_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		if (map_has_event (e->row, e->column, 1, 0))
		{
			if (map_get_event_id (e->row, e->column, 1, 0) == EVENT_DOOR_S)
			{
				map_trigger_event (e->row, e->column, 1, 0, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_S)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_DOOR_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_DOOR_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state 	= HERO_RIDE_DOOR_S)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_door_w (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_DOOR_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_DOOR_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		if (map_has_event (e->row, e->column, 0, -1))
		{
			if (map_get_event_id (e->row, e->column, 0, -1) == EVENT_DOOR_W)
			{
				map_trigger_event (e->row, e->column, 0, -1, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 0, -1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_WALK_SEQC_W)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_DOOR_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state 	= HERO_RIDE_DOOR_W)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_door_n (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		if (map_has_event (e->row, e->column, -1, 0))
		{
			if (map_get_event_id (e->row, e->column, -1, 0) == EVENT_DOOR_N)
			{
				map_trigger_event (e->row, e->column, -1, 0, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, -1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQC_N)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_DOOR_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_DOOR_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_DOOR_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state 	= HERO_WALK_DOOR_N)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_door_e (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_DOOR_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_DOOR_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_DOOR_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		if (map_has_event (e->row, e->column, 0, 1))
		{
			if (map_get_event_id (e->row, e->column, 0, 1) == EVENT_DOOR_E)
			{
				map_trigger_event (e->row, e->column, 0, 1, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 0, 1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQC_E)] (e, p);
		}

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state 	= HERO_WALK_DOOR_E)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_door_s (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_DOOR_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		if (map_has_event (e->row, e->column, 1, 0))
		{
			if (map_get_event_id (e->row, e->column, 1, 0) == EVENT_DOOR_S)
			{
				map_trigger_event (e->row, e->column, 1, 0, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 1, 0) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQC_S)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_DOOR_W)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_DOOR_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state 	= HERO_WALK_DOOR_S)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_ride_door_w (wanderlust_entity * e, hero_property * p)
{
	if (input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_DOOR_N)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_DOOR_S)] (e, p);

		return;
	}

	if (input_get_button (e_configuration.button.left))
	{
		if (map_has_event (e->row, e->column, 0, -1))
		{
			if (map_get_event_id (e->row, e->column, 0, -1) == EVENT_DOOR_W)
			{
				map_trigger_event (e->row, e->column, 0, -1, e);

				return;
			}
		}

		if (map_get_terrain (e->row, e->column, 0, -1) == e_configuration.map.terrain.open)
		{
			setup [(p->state = HERO_RIDE_SEQC_W)] (e, p);
		}

		return;
	}

	if (input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_DOOR_E)] (e, p);

		return;
	}

	if (input_get_buffered_button (e_configuration.button.b))
	{
		setup [(p->state 	= HERO_WALK_DOOR_W)] (e, p);
	}

	trigger_lua_callback_buttons (p);
}

static nil handle_walk_wait_n (wanderlust_entity * e, hero_property * p)
{
	if (! input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_WALK_SEQA_N)] (e, p);
	}
}

static nil handle_walk_wait_e (wanderlust_entity * e, hero_property * p)
{
	if (! input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_WALK_SEQA_E)] (e, p);
	}
}

static nil handle_walk_wait_s (wanderlust_entity * e, hero_property * p)
{
	if (! input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_WALK_SEQA_S)] (e, p);
	}
}

static nil handle_walk_wait_w (wanderlust_entity * e, hero_property * p)
{
	if (! input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_WALK_SEQA_W)] (e, p);
	}
}

static nil handle_ride_wait_n (wanderlust_entity * e, hero_property * p)
{
	if (! input_get_button (e_configuration.button.up))
	{
		setup [(p->state = HERO_RIDE_SEQA_N)] (e, p);
	}
}

static nil handle_ride_wait_e (wanderlust_entity * e, hero_property * p)
{
	if (! input_get_button (e_configuration.button.right))
	{
		setup [(p->state = HERO_RIDE_SEQA_E)] (e, p);
	}
}

static nil handle_ride_wait_s (wanderlust_entity * e, hero_property * p)
{
	if (! input_get_button (e_configuration.button.down))
	{
		setup [(p->state = HERO_RIDE_SEQA_S)] (e, p);
	}
}

static nil handle_ride_wait_w (wanderlust_entity * e, hero_property * p)
{
	if (! input_get_button (e_configuration.button.left))
	{
		setup [(p->state = HERO_RIDE_SEQA_W)] (e, p);
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - UPDATE BEHAVIOUR
 *
 * ***********************************************************************/

static nil update_walk_seqa_n (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_seqa_e (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_seqa_s (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_seqa_w (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_seqb_n (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_seqb_e (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_seqb_s (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_seqb_w (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_seqc_n (wanderlust_entity * e, hero_property * p)
{
		if (p->steps < e_configuration.tileset.height)
	{
		if (p->steps / (e_configuration.tileset.height / 2))
		{
			e->source = & f_sprite_sources [WALK_NORTH_A];
		}

		p->steps += p->iterator;
	}
	else
	{
		setup [p->state = HERO_WALK_SEQB_N] (e, p);
	}
}

static nil update_walk_seqc_e (wanderlust_entity * e, hero_property * p)
{
		if (p->steps < e_configuration.tileset.width)
	{
		if (p->steps / (e_configuration.tileset.width / 2))
		{
			e->source = & f_sprite_sources [WALK_EAST_A];
		}

		p->steps += p->iterator;
	}
	else
	{
		setup [p->state = HERO_WALK_SEQB_E] (e, p);
	}
}

static nil update_walk_seqc_s (wanderlust_entity * e, hero_property * p)
{
		if (p->steps < e_configuration.tileset.height)
	{
		if (p->steps / (e_configuration.tileset.height / 2))
		{
			e->source = & f_sprite_sources [WALK_SOUTH_A];
		}

		p->steps += p->iterator;
	}
	else
	{
		setup [p->state = HERO_WALK_SEQB_S] (e, p);
	}
}

static nil update_walk_seqc_w (wanderlust_entity * e, hero_property * p)
{
		if (p->steps < e_configuration.tileset.width)
	{
		if (p->steps / (e_configuration.tileset.width / 2))
		{
			e->source = & f_sprite_sources [WALK_WEST_A];
		}

		p->steps += p->iterator;
	}
	else
	{
		setup [p->state = HERO_WALK_SEQB_W] (e, p);
	}
}

static nil update_walk_seqd_n (wanderlust_entity * e, hero_property * p)
{
		if (p->steps < e_configuration.tileset.height)
	{
		if (p->steps / (e_configuration.tileset.height / 2))
		{
			e->source = & f_sprite_sources [WALK_NORTH_B];
		}

		p->steps += p->iterator;
	}
	else
	{
		setup [p->state = HERO_WALK_SEQA_N] (e, p);
	}
}

static nil update_walk_seqd_e (wanderlust_entity * e, hero_property * p)
{
		if (p->steps < e_configuration.tileset.width)
	{
		if (p->steps / (e_configuration.tileset.width / 2))
		{
			e->source = & f_sprite_sources [WALK_EAST_A];
		}

		p->steps += p->iterator;
	}
	else
	{
		setup [p->state = HERO_WALK_SEQA_E] (e, p);
	}
}

static nil update_walk_seqd_s (wanderlust_entity * e, hero_property * p)
{
		if (p->steps < e_configuration.tileset.height)
	{
		if (p->steps / (e_configuration.tileset.height / 2))
		{
			e->source = & f_sprite_sources [WALK_SOUTH_B];
		}

		p->steps += p->iterator;
	}
	else
	{
		setup [p->state = HERO_WALK_SEQA_S] (e, p);
	}
}

static nil update_walk_seqd_w (wanderlust_entity * e, hero_property * p)
{
		if (p->steps < e_configuration.tileset.width)
	{
		if (p->steps / (e_configuration.tileset.width / 2))
		{
			e->source = & f_sprite_sources [WALK_WEST_A];
		}

		p->steps += p->iterator;
	}
	else
	{
		setup [p->state = HERO_WALK_SEQA_W] (e, p);
	}
}

static nil update_walk_jump_n (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_jump_e (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_jump_s (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_jump_w (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_rise_n (wanderlust_entity * e, hero_property * p)
{
		if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	-= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_WALK_FALL_N)] (e, p);
	}
}

static nil update_walk_rise_e (wanderlust_entity * e, hero_property * p)
{
	if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	-= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_WALK_FALL_E)] (e, p);
	}
}

static nil update_walk_rise_s (wanderlust_entity * e, hero_property * p)
{
		if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	-= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_WALK_FALL_S)] (e, p);
	}
}

static nil update_walk_rise_w (wanderlust_entity * e, hero_property * p)
{
		if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	-= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_WALK_FALL_W)] (e, p);
	}
}

static nil update_walk_fall_n (wanderlust_entity * e, hero_property * p)
{
		if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	+= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_WALK_SEQA_N)] (e, p);
	}
}

static nil update_walk_fall_e (wanderlust_entity * e, hero_property * p)
{
		if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	+= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_WALK_SEQA_E)] (e, p);
	}
}

static nil update_walk_fall_s (wanderlust_entity * e, hero_property * p)
{
		if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	+= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_WALK_SEQA_S)] (e, p);
	}
}

static nil update_walk_fall_w (wanderlust_entity * e, hero_property * p)
{
		if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	+= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_WALK_SEQA_W)] (e, p);
	}
}

static nil update_ride_seqa_n (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqa_e (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqa_s (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqa_w (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqb_n (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqb_e (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqb_s (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqb_w (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqc_n (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqc_e (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqc_s (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqc_w (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqd_n (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqd_e (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqd_s (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqd_w (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_seqe_n (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.height)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQB_N)] (e, p);
	}
}

static nil update_ride_seqe_e (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.width)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQB_E)] (e, p);
	}
}

static nil update_ride_seqe_s (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.height)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQB_S)] (e, p);
	}
}

static nil update_ride_seqe_w (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.width)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQB_W)] (e, p);
	}
}

static nil update_ride_seqf_n (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.height)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQC_N)] (e, p);
	}
}

static nil update_ride_seqf_e (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.width)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQC_E)] (e, p);
	}
}

static nil update_ride_seqf_s (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.height)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQC_S)] (e, p);
	}
}

static nil update_ride_seqf_w (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.width)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQC_W)] (e, p);
	}
}

static nil update_ride_seqg_n (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.height)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQD_N)] (e, p);
	}
}

static nil update_ride_seqg_e (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.width)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQD_E)] (e, p);
	}
}

static nil update_ride_seqg_s (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.height)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQD_S)] (e, p);
	}
}

static nil update_ride_seqg_w (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.width)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQD_W)] (e, p);
	}
}

static nil update_ride_seqh_n (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.height)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQA_N)] (e, p);
	}
}

static nil update_ride_seqh_e (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.width)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQA_E)] (e, p);
	}
}

static nil update_ride_seqh_s (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.height)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQA_S)] (e, p);
	}
}

static nil update_ride_seqh_w (wanderlust_entity * e, hero_property * p)
{
	if (p->steps < e_configuration.tileset.width)
	{
		p->steps += p->iterator;
	}
	else
	{
		setup [(p->state	= HERO_RIDE_SEQA_W)] (e, p);
	}
}

static nil update_ride_jump_n (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_jump_e (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_jump_s (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_jump_w (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_rise_n (wanderlust_entity * e, hero_property * p)
{
	if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	-= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_RIDE_FALL_N)] (e, p);
	}
}

static nil update_ride_rise_e (wanderlust_entity * e, hero_property * p)
{
	if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	-= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_RIDE_FALL_E)] (e, p);
	}
}

static nil update_ride_rise_s (wanderlust_entity * e, hero_property * p)
{
	if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	-= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_RIDE_FALL_S)] (e, p);
	}
}

static nil update_ride_rise_w (wanderlust_entity * e, hero_property * p)
{
	if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	-= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_RIDE_FALL_W)] (e, p);
	}
}

static nil update_ride_fall_n (wanderlust_entity * e, hero_property * p)
{
	if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	+= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_RIDE_SEQA_N)] (e, p);
	}
}

static nil update_ride_fall_e (wanderlust_entity * e, hero_property * p)
{
	if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	+= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_RIDE_SEQA_E)] (e, p);
	}
}

static nil update_ride_fall_s (wanderlust_entity * e, hero_property * p)
{
	if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	+= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_RIDE_SEQA_S)] (e, p);
	}
}

static nil update_ride_fall_w (wanderlust_entity * e, hero_property * p)
{
	if (p->steps)
	{
		p->steps 		-= p->iterator;
		e->target.y	+= e_configuration.map.stepping.walk;
	}
	else
	{
		setup [(p->state = HERO_RIDE_SEQA_W)] (e, p);
	}
}

static nil update_surf_idle_n (wanderlust_entity * e, hero_property * p) { }

static nil update_surf_idle_e (wanderlust_entity * e, hero_property * p) { }

static nil update_surf_idle_s (wanderlust_entity * e, hero_property * p) { }

static nil update_surf_idle_w (wanderlust_entity * e, hero_property * p) { }

static nil update_surf_move_n (wanderlust_entity * e, hero_property * p) { }

static nil update_surf_move_e (wanderlust_entity * e, hero_property * p) { }

static nil update_surf_move_s (wanderlust_entity * e, hero_property * p) { }

static nil update_surf_move_w (wanderlust_entity * e, hero_property * p) { }

static nil update_fish_idle_n (wanderlust_entity * e, hero_property * p) { }

static nil update_fish_idle_e (wanderlust_entity * e, hero_property * p) { }

static nil update_fish_idle_s (wanderlust_entity * e, hero_property * p) { }

static nil update_fish_idle_w (wanderlust_entity * e, hero_property * p) { }

static nil update_fish_hits_n (wanderlust_entity * e, hero_property * p) { }

static nil update_fish_hits_e (wanderlust_entity * e, hero_property * p) { }

static nil update_fish_hits_s (wanderlust_entity * e, hero_property * p) { }

static nil update_fish_hits_w (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_door_n (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_door_e (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_door_s (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_door_w (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_door_n (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_door_e (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_door_s (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_door_w (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_wait_n (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_wait_e (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_wait_s (wanderlust_entity * e, hero_property * p) { }

static nil update_walk_wait_w (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_wait_n (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_wait_e (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_wait_s (wanderlust_entity * e, hero_property * p) { }

static nil update_ride_wait_w (wanderlust_entity * e, hero_property * p) { }

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - RENDER ENTITY
 *
 * ***********************************************************************/

static nil render_walk_seqa_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqa_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqa_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqa_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqb_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqb_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqb_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqb_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqc_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqc_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqc_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqc_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqd_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqd_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqd_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_seqd_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_jump_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_jump_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_jump_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_jump_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_rise_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_walk_rise_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_walk_rise_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_walk_rise_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_walk_fall_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_walk_fall_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_walk_fall_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_walk_fall_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_ride_seqa_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqa_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqa_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqa_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqb_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqb_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqb_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqb_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqc_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqc_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqc_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqc_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqd_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqd_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqd_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqd_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqe_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqe_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqe_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqe_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqf_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqf_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqf_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqf_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqg_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqg_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqg_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqg_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqh_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqh_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqh_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_seqh_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_jump_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_jump_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_jump_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_jump_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_rise_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_ride_rise_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_ride_rise_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_ride_rise_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_ride_fall_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_ride_fall_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_ride_fall_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_ride_fall_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_sprite (e->texture, p->shadow.source, & p->shadow.target);

	video_draw_entity (e);
}

static nil render_surf_idle_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_surf_idle_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_surf_idle_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_surf_idle_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_surf_move_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_surf_move_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_surf_move_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_surf_move_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_fish_idle_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_fish_idle_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_fish_idle_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_fish_idle_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_fish_hits_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_fish_hits_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_fish_hits_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_fish_hits_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_door_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_door_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_door_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_door_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_door_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_door_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_door_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_door_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_wait_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_wait_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_wait_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_walk_wait_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_wait_n (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_wait_e (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_wait_s (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

static nil render_ride_wait_w (wanderlust_entity * e, hero_property * p)
{
	video_draw_entity (e);
}

/* ************************************************************************
 *
 * BANKS
 *
 * ***********************************************************************/

static nil (* handle [HERO_STATES]) (wanderlust_entity *, hero_property *) =
{
	handle_walk_seqa_n, handle_walk_seqa_e, handle_walk_seqa_s, handle_walk_seqa_w,
	handle_walk_seqb_n, handle_walk_seqb_e, handle_walk_seqb_s, handle_walk_seqb_w,
	handle_walk_seqc_n, handle_walk_seqc_e, handle_walk_seqc_s, handle_walk_seqc_w,
	handle_walk_seqd_n, handle_walk_seqd_e, handle_walk_seqd_s, handle_walk_seqd_w,
	handle_walk_jump_n, handle_walk_jump_e, handle_walk_jump_s, handle_walk_jump_w,
	handle_walk_rise_n, handle_walk_rise_e, handle_walk_rise_s, handle_walk_rise_w,
	handle_walk_fall_n, handle_walk_fall_e, handle_walk_fall_s, handle_walk_fall_w,
	handle_ride_seqa_n, handle_ride_seqa_e, handle_ride_seqa_s, handle_ride_seqa_w,
	handle_ride_seqb_n, handle_ride_seqb_e, handle_ride_seqb_s, handle_ride_seqb_w,
	handle_ride_seqc_n, handle_ride_seqc_e, handle_ride_seqc_s, handle_ride_seqc_w,
	handle_ride_seqd_n, handle_ride_seqd_e, handle_ride_seqd_s, handle_ride_seqd_w,
	handle_ride_seqe_n, handle_ride_seqe_e, handle_ride_seqe_s, handle_ride_seqe_w,
	handle_ride_seqf_n, handle_ride_seqf_e, handle_ride_seqf_s, handle_ride_seqf_w,
	handle_ride_seqg_n, handle_ride_seqg_e, handle_ride_seqg_s, handle_ride_seqg_w,
	handle_ride_seqh_n, handle_ride_seqh_e, handle_ride_seqh_s, handle_ride_seqh_w,
	handle_ride_jump_n, handle_ride_jump_e, handle_ride_jump_s, handle_ride_jump_w,
	handle_ride_rise_n, handle_ride_rise_e, handle_ride_rise_s, handle_ride_rise_w,
	handle_ride_fall_n, handle_ride_fall_e, handle_ride_fall_s, handle_ride_fall_w,
	handle_surf_idle_n, handle_surf_idle_e, handle_surf_idle_s, handle_surf_idle_w,
	handle_surf_move_n, handle_surf_move_e, handle_surf_move_s, handle_surf_move_w,
	handle_fish_idle_n, handle_fish_idle_e, handle_fish_idle_s, handle_fish_idle_w,
	handle_fish_hits_n, handle_fish_hits_e, handle_fish_hits_s, handle_fish_hits_w,
	handle_walk_door_n, handle_walk_door_e, handle_walk_door_s, handle_walk_door_w,
	handle_ride_door_n, handle_ride_door_e, handle_ride_door_s, handle_ride_door_w,
	handle_walk_wait_n, handle_walk_wait_e, handle_walk_wait_s, handle_walk_wait_w,
	handle_ride_wait_n, handle_ride_wait_e, handle_ride_wait_s, handle_ride_wait_w
};

static nil (* update [HERO_STATES]) (wanderlust_entity *, hero_property *) =
{
	update_walk_seqa_n, update_walk_seqa_e, update_walk_seqa_s, update_walk_seqa_w,
	update_walk_seqb_n, update_walk_seqb_e, update_walk_seqb_s, update_walk_seqb_w,
	update_walk_seqc_n, update_walk_seqc_e, update_walk_seqc_s, update_walk_seqc_w,
	update_walk_seqd_n, update_walk_seqd_e, update_walk_seqd_s, update_walk_seqd_w,
	update_walk_jump_n, update_walk_jump_e, update_walk_jump_s, update_walk_jump_w,
	update_walk_rise_n, update_walk_rise_e, update_walk_rise_s, update_walk_rise_w,
	update_walk_fall_n, update_walk_fall_e, update_walk_fall_s, update_walk_fall_w,
	update_ride_seqa_n, update_ride_seqa_e, update_ride_seqa_s, update_ride_seqa_w,
	update_ride_seqb_n, update_ride_seqb_e, update_ride_seqb_s, update_ride_seqb_w,
	update_ride_seqc_n, update_ride_seqc_e, update_ride_seqc_s, update_ride_seqc_w,
	update_ride_seqd_n, update_ride_seqd_e, update_ride_seqd_s, update_ride_seqd_w,
	update_ride_seqe_n, update_ride_seqe_e, update_ride_seqe_s, update_ride_seqe_w,
	update_ride_seqf_n, update_ride_seqf_e, update_ride_seqf_s, update_ride_seqf_w,
	update_ride_seqg_n, update_ride_seqg_e, update_ride_seqg_s, update_ride_seqg_w,
	update_ride_seqh_n, update_ride_seqh_e, update_ride_seqh_s, update_ride_seqh_w,

	update_ride_jump_n, update_ride_jump_e, update_ride_jump_s, update_ride_jump_w,
	update_ride_rise_n, update_ride_rise_e, update_ride_rise_s, update_ride_rise_w,
	update_ride_fall_n, update_ride_fall_e, update_ride_fall_s, update_ride_fall_w,
	update_surf_idle_n, update_surf_idle_e, update_surf_idle_s, update_surf_idle_w,
	update_surf_move_n, update_surf_move_e, update_surf_move_s, update_surf_move_w,
	update_fish_idle_n, update_fish_idle_e, update_fish_idle_s, update_fish_idle_w,
	update_fish_hits_n, update_fish_hits_e, update_fish_hits_s, update_fish_hits_w,
	update_walk_door_n, update_walk_door_e, update_walk_door_s, update_walk_door_w,
	update_ride_door_n, update_ride_door_e, update_ride_door_s, update_ride_door_w,
	update_walk_wait_n, update_walk_wait_e, update_walk_wait_s, update_walk_wait_w,
	update_ride_wait_n, update_ride_wait_e, update_ride_wait_s, update_ride_wait_w
};

static nil (* render [HERO_STATES]) (wanderlust_entity *, hero_property *) =
{
	render_walk_seqa_n, render_walk_seqa_e, render_walk_seqa_s, render_walk_seqa_w,
	render_walk_seqb_n, render_walk_seqb_e, render_walk_seqb_s, render_walk_seqb_w,
	render_walk_seqc_n, render_walk_seqc_e, render_walk_seqc_s, render_walk_seqc_w,
	render_walk_seqd_n, render_walk_seqd_e, render_walk_seqd_s, render_walk_seqd_w,
	render_walk_jump_n, render_walk_jump_e, render_walk_jump_s, render_walk_jump_w,
	render_walk_rise_n, render_walk_rise_e, render_walk_rise_s, render_walk_rise_w,
	render_walk_fall_n, render_walk_fall_e, render_walk_fall_s, render_walk_fall_w,
	render_ride_seqa_n, render_ride_seqa_e, render_ride_seqa_s, render_ride_seqa_w,
	render_ride_seqb_n, render_ride_seqb_e, render_ride_seqb_s, render_ride_seqb_w,
	render_ride_seqc_n, render_ride_seqc_e, render_ride_seqc_s, render_ride_seqc_w,
	render_ride_seqd_n, render_ride_seqd_e, render_ride_seqd_s, render_ride_seqd_w,
	render_ride_seqe_n, render_ride_seqe_e, render_ride_seqe_s, render_ride_seqe_w,
	render_ride_seqf_n, render_ride_seqf_e, render_ride_seqf_s, render_ride_seqf_w,
	render_ride_seqg_n, render_ride_seqg_e, render_ride_seqg_s, render_ride_seqg_w,
	render_ride_seqh_n, render_ride_seqh_e, render_ride_seqh_s, render_ride_seqh_w,
	render_ride_jump_n, render_ride_jump_e, render_ride_jump_s, render_ride_jump_w,
	render_ride_rise_n, render_ride_rise_e, render_ride_rise_s, render_ride_rise_w,
	render_ride_fall_n, render_ride_fall_e, render_ride_fall_s, render_ride_fall_w,
	render_surf_idle_n, render_surf_idle_e, render_surf_idle_s, render_surf_idle_w,
	render_surf_move_n, render_surf_move_e, render_surf_move_s, render_surf_move_w,
	render_fish_idle_n, render_fish_idle_e, render_fish_idle_s, render_fish_idle_w,
	render_fish_hits_n, render_fish_hits_e, render_fish_hits_s, render_fish_hits_w,
	render_walk_door_n, render_walk_door_e, render_walk_door_s, render_walk_door_w,
	render_ride_door_n, render_ride_door_e, render_ride_door_s, render_ride_door_w,
	render_walk_wait_n, render_walk_wait_e, render_walk_wait_s, render_walk_wait_w,
	render_ride_wait_n, render_ride_wait_e, render_ride_wait_s, render_ride_wait_w
};

static nil (* modify [MODIFIER_COUNT]) (wanderlust_entity *, lua_State *, u16) =
{
	modify_title,
	modify_row,
	modify_column,
	modify_direction,
	modify_texture
};

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ***********************************************************************/

static nil hero_delete_property (hero_property * p)
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

#define HERO_CREATE_PROPERTIES_ARG_POS_SCRIPT 4

static hero_property * hero_create_properties (lua_State * L)
{
	hero_property * p = 0;

	if (! L)
	{
		goto FAILURE;
	}

	if (! (p = SDL_malloc (sizeof (hero_property))))
	{
		goto FAILURE;
	}

	p->lua = e_configuration.null;

	if (! (p->lua = lua_open ()))
	{
		goto FAILURE;
	}

	if (luaL_loadfile (p->lua, lua_tostring (L, HERO_CREATE_PROPERTIES_ARG_POS_SCRIPT)))
	{
		goto FAILURE;
	}

	luaL_openlibs (p->lua);

	if (lua_pcall (p->lua, 0, 0, 0))
	{
		goto FAILURE;
	}

	lua_register (p->lua, WL_LOAD_IMAGE,					texture_load_image);
	lua_register (p->lua, WL_RUNDOWN_TASK,				task_rundown);
	lua_register (p->lua, WL_LOAD_MAP,					map_load_segment);
	lua_register (p->lua, WL_SELECT_MAP,					map_select_segment);
	lua_register (p->lua, WL_ENTER_STATE,					state_enter);
	lua_register (p->lua, WL_LEAVE_STATE,					state_leave);
	lua_register (p->lua, WL_ENTER_STATE_BY_TRANSITION,	state_enter_by_transition);
	lua_register (p->lua, WL_LEAVE_STATE_BY_TRANSITION,	state_leave_by_transition);
	lua_register (p->lua, WL_FETCH_STATE,					state_fetch);
	lua_register (p->lua, WL_CLEAR_STATE,					state_clear);
	lua_register (p->lua, WL_CREATE_ENTITY,				entity_enqueue);
	lua_register (p->lua, WL_SELECT_STATE_ENTITY,			state_entity_select);
	lua_register (p->lua, WL_CREATE_FONT, 				font_create);
	lua_register (p->lua, WL_CREATE_TRANSITION,			video_enqueue_transition);
	lua_register (p->lua, WL_CREATE_MENU,					menu_create);
	lua_register (p->lua, WL_SELECT_MENU_ENTITY,			menu_select);
	lua_register (p->lua, WL_REPEAL_MENU,					menu_repeal);
	lua_register (p->lua, WL_APPEND_MENU,					menu_append);
	lua_register (p->lua, WL_CREATE_TEXTBOX,				textbox_enqueue);
	lua_register (p->lua, WL_GET_PROPERTY, 				property_get_wanderlust_property);
	lua_register (p->lua, WL_SET_PROPERTY,				property_set_wanderlust_property);
	lua_register (p->lua, WL_CREATE_TERMINAL, 			terminal_create);
	lua_register (p->lua, WL_SHOW_TERMINAL, 				terminal_show);
	lua_register (p->lua, WL_HIDE_TERMINAL, 				terminal_hide);

	p->steps				= 0;
	p->iterator			= 1;
	p->counter			= 0;
	p->ability			= 0;
	p->state				= HERO_WALK_SEQA_S;
	p->shadow.source		= &f_sprite_sources [JUMP_SHADOW];
	p->shadow.target.w	= e_configuration.tileset.width;
	p->shadow.target.h	= e_configuration.tileset.height;

	return p;

	FAILURE:

		if (p)
		{
			hero_delete_property (p);
		}

		return e_configuration.null;
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

#define PROPERTY_PLAYER_ROW "PLAYER_ROW"

#define PROPERTY_PLAYER_COL "PLAYER_COLUMN"

#define PROPERTY_PLAYER_DIR "PLAYER_DIRECTION"

#define ENTITY_HERO_CREATE_ARG_COUNT		4
#define ENTITY_HERO_CREATE_ARG_POS_TITLE	2
#define ENTITY_HERO_CREATE_ARG_POS_TEXTURE	3


wanderlust_entity * entity_hero_create (lua_State * L)
{
	wanderlust_entity 	* e = 0;
	hero_property		* p = 0;

	if (lua_gettop (L) < ENTITY_HERO_CREATE_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, ENTITY_HERO_CREATE_ARG_POS_TITLE))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, ENTITY_HERO_CREATE_ARG_POS_TEXTURE))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, HERO_CREATE_PROPERTIES_ARG_POS_SCRIPT))
	{
		goto FAILURE;
	}

	if (! (e = SDL_malloc (sizeof (wanderlust_entity))))
	{
		goto FAILURE;
	}

	e->texture 	= 0;
	e->properties = 0;
	e->title		= 0;

	if (! (p = e->properties = hero_create_properties (L)))
	{
		goto FAILURE;
	}

	e->title = (s08*)type_secure_copy_string
	(
		lua_tostring (L, ENTITY_HERO_CREATE_ARG_POS_TITLE)
	);

	if (! e->title)
	{
		goto FAILURE;
	}

	SET_U32_CONST
	(
		e->hash,
		(u32) mhash (lua_tostring (L, ENTITY_HERO_CREATE_ARG_POS_TITLE))
	);

	e->texture = texture_get_by_name
	(
		lua_tostring (L, ENTITY_HERO_CREATE_ARG_POS_TEXTURE)
	);

	if (! e->texture)
	{
		goto FAILURE;
	}

	e->source 			= &f_sprite_sources [WALK_IDLE_S];

	e->target.x			= e_configuration.tileset.width * property_get_integer ((const s08*)PROPERTY_HERO_POSITION_COLUMN);
	e->target.y			= e_configuration.tileset.height * property_get_integer ((const s08*)PROPERTY_HERO_POSITION_ROW);
	e->target.w			= e_configuration.tileset.width;
	e->target.h			= e_configuration.tileset.height;

	e->row				= map_get_row ();
	e->column				= map_get_column ();

	p->shadow.target.x	= e->target.x;
	p->shadow.target.y	= e->target.y;

	e->direction			= e_configuration.direction.s;

	SET_U08_CONST (e->type, 	HERO_ENTITY);
	SET_U08_CONST (e->locked,	e_configuration.zero);

	event_attach (EVENT_JUMP, 	event_jump);
	event_attach (EVENT_SURF, 	event_surf);
	event_attach (EVENT_SIGN,	event_sign);
	event_attach (EVENT_DOOR, 	event_door);

	event_attach (EVENT_JUMP_N, event_jump_n);
	event_attach (EVENT_JUMP_E, event_jump_e);
	event_attach (EVENT_JUMP_S, event_jump_s);
	event_attach (EVENT_JUMP_W, event_jump_w);

	property_create_integer ((const s08*)PROPERTY_PLAYER_ROW, e->row);
	property_create_integer ((const s08*)PROPERTY_PLAYER_COL, e->column);
	property_create_integer ((const s08*)PROPERTY_PLAYER_DIR, e->direction);

	lua_settop (L, 0);

	return e;

	FAILURE:

		if (e)
		{
			entity_hero_delete (e);
		}

		if (L)
		{
			lua_settop (L, 0);
		}

		return e_configuration.null;
}

nil	entity_hero_direct (wanderlust_entity * e)
{
	/*
	 * entity_hero_adjust (e);
	 */
}

nil	entity_hero_delete (wanderlust_entity * e)
{
	if (! e)
	{
		return;
	}

	if (e->properties)
	{
		hero_delete_property (e->properties);
	}

	if (e->title)
	{
		SDL_free (e->title);
	}

	SDL_free (e);
}

nil	entity_hero_handle (wanderlust_entity * e)
{
	hero_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	handle [p->state] (e, p);
}

nil	entity_hero_update (wanderlust_entity * e)
{
	hero_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	update [p->state] (e, p);
}

nil	entity_hero_render (wanderlust_entity * e)
{
	hero_property * p = e->properties;

	if (e->locked)
	{
		return;
	}

	render [p->state] (e, p);
}

nil	entity_hero_adjust (wanderlust_entity * e)
{
	property_set_integer (
		(const s08*)PROPERTY_PLAYER_ROW, 
		(e->row	= map_get_row ())
	);
	property_set_integer (
		(const s08*)PROPERTY_PLAYER_COL, 
		(e->column = map_get_column())
	);
	property_set_integer (
		(const s08*)PROPERTY_PLAYER_DIR,
		e->direction
	);
}

nil entity_hero_select (wanderlust_entity * e)
{
	if (! e->locked)
	{
		return;
	}

	SET_U08_CONST (e->locked, 0x00);
}

nil	entity_hero_recall (wanderlust_entity * e)
{
	if (e->locked)
	{
		return;
	}

	SET_U08_CONST (e->locked, 0xFF);
}

nil entity_hero_excite (wanderlust_entity * e)
{

}

nil entity_hero_expire (wanderlust_entity * e)
{

}

nil entity_hero_modify (wanderlust_entity * e, lua_State * L, u16 index)
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

nil	entity_hero_initialize ()
{
	f_sprite_sources [WALK_IDLE_N].x 	= 0;
	f_sprite_sources [WALK_IDLE_N].y 	= 0;
	f_sprite_sources [WALK_IDLE_N].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_IDLE_N].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_NORTH_A].x 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_NORTH_A].y 	= 0;
	f_sprite_sources [WALK_NORTH_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_NORTH_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_NORTH_B].x 	= e_configuration.tileset.width * 2;
	f_sprite_sources [WALK_NORTH_B].y 	= 0;
	f_sprite_sources [WALK_NORTH_B].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_NORTH_B].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_IDLE_E].x 	= 0;
	f_sprite_sources [WALK_IDLE_E].y 	= e_configuration.tileset.height;
	f_sprite_sources [WALK_IDLE_E].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_IDLE_E].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_EAST_A].x 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_EAST_A].y	= e_configuration.tileset.height;
	f_sprite_sources [WALK_EAST_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_EAST_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_IDLE_S].x 	= 0;
	f_sprite_sources [WALK_IDLE_S].y 	= e_configuration.tileset.height * 2;
	f_sprite_sources [WALK_IDLE_S].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_IDLE_S].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_SOUTH_A].x 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_SOUTH_A].y 	= e_configuration.tileset.height * 2;
	f_sprite_sources [WALK_SOUTH_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_SOUTH_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_SOUTH_B].x 	= e_configuration.tileset.width * 2;
	f_sprite_sources [WALK_SOUTH_B].y 	= e_configuration.tileset.height * 2;
	f_sprite_sources [WALK_SOUTH_B].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_SOUTH_B].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_IDLE_W].x 	= 0;
	f_sprite_sources [WALK_IDLE_W].y 	= e_configuration.tileset.height * 3;
	f_sprite_sources [WALK_IDLE_W].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_IDLE_W].h 	= e_configuration.tileset.height;

	f_sprite_sources [WALK_WEST_A].x 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_WEST_A].y	= e_configuration.tileset.height * 3;
	f_sprite_sources [WALK_WEST_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [WALK_WEST_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [JUMP_SHADOW].x 	= e_configuration.tileset.width * 5;
	f_sprite_sources [JUMP_SHADOW].y	= e_configuration.tileset.height * 4;
	f_sprite_sources [JUMP_SHADOW].w 	= e_configuration.tileset.width;
	f_sprite_sources [JUMP_SHADOW].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_IDLE_N].x 	= e_configuration.tileset.width * 3;
	f_sprite_sources [RIDE_IDLE_N].y	= 0;
	f_sprite_sources [RIDE_IDLE_N].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_IDLE_N].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_N_A].x 	= e_configuration.tileset.width * 4;
	f_sprite_sources [RIDE_MOVE_N_A].y	= 0;
	f_sprite_sources [RIDE_MOVE_N_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_N_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_N_B].x 	= e_configuration.tileset.width * 5;
	f_sprite_sources [RIDE_MOVE_N_B].y	= 0;
	f_sprite_sources [RIDE_MOVE_N_B].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_N_B].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_IDLE_E].x 	= e_configuration.tileset.width * 3;
	f_sprite_sources [RIDE_IDLE_E].y	= e_configuration.tileset.height;
	f_sprite_sources [RIDE_IDLE_E].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_IDLE_E].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_E_A].x 	= e_configuration.tileset.width * 4;
	f_sprite_sources [RIDE_MOVE_E_A].y	= e_configuration.tileset.height;
	f_sprite_sources [RIDE_MOVE_E_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_E_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_IDLE_S].x 	= e_configuration.tileset.width * 3;
	f_sprite_sources [RIDE_IDLE_S].y	= e_configuration.tileset.height * 2;
	f_sprite_sources [RIDE_IDLE_S].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_IDLE_S].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_S_A].x 	= e_configuration.tileset.width * 4;
	f_sprite_sources [RIDE_MOVE_S_A].y	= e_configuration.tileset.height * 2;
	f_sprite_sources [RIDE_MOVE_S_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_S_A].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_S_B].x 	= e_configuration.tileset.width * 5;
	f_sprite_sources [RIDE_MOVE_S_B].y	= e_configuration.tileset.height * 2;
	f_sprite_sources [RIDE_MOVE_S_B].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_S_B].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_IDLE_W].x 	= e_configuration.tileset.width * 3;
	f_sprite_sources [RIDE_IDLE_W].y	= e_configuration.tileset.height * 3;
	f_sprite_sources [RIDE_IDLE_W].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_IDLE_W].h 	= e_configuration.tileset.height;

	f_sprite_sources [RIDE_MOVE_W_A].x 	= e_configuration.tileset.width * 4;
	f_sprite_sources [RIDE_MOVE_W_A].y	= e_configuration.tileset.height * 3;
	f_sprite_sources [RIDE_MOVE_W_A].w 	= e_configuration.tileset.width;
	f_sprite_sources [RIDE_MOVE_W_A].h 	= e_configuration.tileset.height;
}

nil	entity_hero_terminate ()
{

}
