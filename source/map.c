#include "type.h"
#include "config.h"
#include "tileset.h"
#include "font.h"
#include "morph.h"
#include "entity.h"
#include "task.h"
#include "event.h"
#include "transition.h"
#include "state.h"
#include "video.h"
#include "texture.h"
#include "textbox.h"
#include "property.h"
#include "map.h"

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

typedef enum
{
	STATE_SCROLL_N,
	STATE_SCROLL_E,
	STATE_SCROLL_S,
	STATE_SCROLL_W,
	STATE_IDLE,
	MAP_STATE_TYPES
} wanderlust_segment_state;

typedef struct
{
	u08 			show;

	u16				data [WANDERLUST_MAP_ROWS] [WANDERLUST_MAP_COLUMNS];

	SDL_Texture *	tileset;

} wanderlust_segment_layer;

typedef struct
{
	const	u32 							hash;

			u08								state;

			u08								steps;

			u08								iteration;

			wanderlust_segment_layer	*	layer;

			SDL_Rect						area;	/* area of row/columns to draw */

			SDL_Point						origin;	/* point to begin drawing with */

			lua_State					*	lua;

} wanderlust_segment;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

static s08						f_initialized	= 0;

static nil					*	f_world			= 0;

static wanderlust_segment	*	f_segment		= 0;

/* ************************************************************************
 *
 * TASKS
 *
 * ***********************************************************************/

static nil task_load_segment (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	madd (f_world, item);

	if (! f_segment)
	{
		f_segment = item;
	}
}

static nil task_select_segment (nil * item)
{
	u08 i = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_segment = item;

	for (i = 0; i < e_configuration.map.layer; i ++)
	{
		if (f_segment->layer [i].tileset)
		{
			f_segment->layer [i].show = 1;
		}
		else
		{
			f_segment -> layer [i].show = 0;
		}
	}
}

static nil task_map_warp (nil * item)
{
	if (! item)
	{
		return;
	}

	map_set_row		(((wanderlust_warp_point *) item) -> row);
	map_set_column	(((wanderlust_warp_point *) item) -> column);

	SDL_free (item);
}

/* ************************************************************************
 *
 * COMPARATOR
 *
 * ***********************************************************************/

u08 compare_wanderlust_segment_by_hash (nil * item, nil * value)
{
	if (! item)
	{
		goto FAILURE;
	}

	if (! value)
	{
		goto FAILURE;
	}

	return ((wanderlust_segment *) item) -> hash == *((u32 *)value);

	FAILURE:

		return e_configuration.zero;
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ***********************************************************************/

static nil delete_wanderlust_segment (nil * item)
{
	wanderlust_segment * s = item;
	if (! s)
	{
		return;
	}

	if (s->layer)
	{
		SDL_free (s->layer);
	}

	if (s -> lua)
	{
		lua_close (s -> lua);
	}

	SDL_free (s);
}

#define CREATE_WANDERLUST_SEGMENT_ARG_COUNT			3
#define CREATE_WANDERLUST_SEGMENT_ARG_POS_NAME		1
#define CREATE_WANDERLUST_SEGMENT_ARG_POS_TEXTURE	2
#define CREATE_WANDERLUST_SEGMENT_ARG_POS_SCRIPT	3


static wanderlust_segment * create_wanderlust_segment (lua_State * L)
{
	wanderlust_segment 	* 	s 		= 0;
	SDL_RWops			* 	r 		= 0;
	u08						layer 	= 0;
	u16						row		= 0;

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < CREATE_WANDERLUST_SEGMENT_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, CREATE_WANDERLUST_SEGMENT_ARG_POS_NAME))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, CREATE_WANDERLUST_SEGMENT_ARG_POS_TEXTURE))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, CREATE_WANDERLUST_SEGMENT_ARG_POS_NAME))
	{
		goto FAILURE;
	}

	if (! (lua_isstring (L, 1) && lua_isstring (L, 2) && lua_isstring (L, 3)))
	{
		goto FAILURE;
	}

	if (! (s = SDL_malloc (sizeof (wanderlust_segment))))
	{
		goto FAILURE;
	}

	SDL_memset (s, 0, sizeof (wanderlust_segment));

	s->layer = SDL_malloc
	(
		sizeof (wanderlust_segment_layer) * e_configuration.map.layer
	);

	if (! s)
	{
		goto FAILURE;
	}

	SET_U32_CONST
	(
		s -> hash,
		mhash (lua_tostring (L, CREATE_WANDERLUST_SEGMENT_ARG_POS_NAME))
	);

	if (! (r = SDL_RWFromFile (lua_tostring (L, 1), "rb")))
	{
		goto FAILURE;
	}

	for (layer = 0; layer < e_configuration.map.layer; layer ++)
	{
		for (row = 0; row < e_configuration.map.rows; row ++)
		{
			SDL_RWread
			(
				r,
				s -> layer [layer].data [row],
				sizeof (u16),
				e_configuration.map.columns
			);
		}
	}

	SDL_RWclose (r);

	for (layer = 0; layer < e_configuration.map.layer; layer ++)
	{
		s -> layer [layer].tileset = texture_get_by_name
		(
			lua_tostring (L, CREATE_WANDERLUST_SEGMENT_ARG_POS_TEXTURE)
		);

		if (s->layer [layer].tileset)
		{
			s -> layer [layer].show = 1;
		}
		else
		{
			s -> layer [layer].show = 0;
		}
	}

	s -> area.x 	= 0;
	s -> area.y 	= 0;
	s -> area.h 	= property_get_integer ((const s08*)PROPERTY_MAP_VISIBLE_ROWS);
	s -> area.w		= property_get_integer ((const s08*)PROPERTY_MAP_VISIBLE_COLUMNS);

	s -> origin.x	= -e_configuration.tileset.width;
	s -> origin.y	= -e_configuration.tileset.height;

	s -> state		= STATE_IDLE;
	s -> steps		= 0;
	s -> iteration	= e_configuration.map.stepping.walk;

	if (! (s -> lua = lua_open ()))
	{
		goto FAILURE;
	}

	luaL_openlibs (s -> lua);

	if (luaL_loadfile (s -> lua, lua_tostring (L, CREATE_WANDERLUST_SEGMENT_ARG_POS_SCRIPT)))
	{
		goto FAILURE;
	}

	lua_register (s -> lua, WL_RUNDOWN_TASK, 				task_rundown);
	lua_register (s -> lua, WL_LOAD_MAP, 					map_load_segment);
	lua_register (s -> lua, WL_LOAD_IMAGE,					texture_load_image);
	lua_register (s -> lua, WL_SELECT_MAP,					map_select_segment);
	lua_register (s -> lua, WL_WARP_MAP,					map_warp);
	lua_register (s -> lua, WL_WARP_MAP_BY_TRANSITION,		map_transition);
	lua_register (s -> lua, WL_ENTER_STATE, 				state_enter);
	lua_register (s -> lua, WL_FETCH_STATE, 				state_fetch);
	lua_register (s -> lua, WL_CLEAR_STATE, 				state_clear);
	lua_register (s -> lua, WL_ATTACH_EVENT, 				event_attach_from_lua);
	lua_register (s -> lua, WL_DEATTACH_EVENT,				event_deattach_from_lua);
	lua_register (s -> lua, WL_CREATE_TRANSITION,			video_enqueue_transition);
	lua_register (s -> lua, WL_CREATE_ENTITY,				entity_enqueue);
	lua_register (s -> lua, WL_SELECT_STATE_ENTITY,			state_entity_select);
	lua_register (s -> lua, WL_REMOVE_STATE_ENTITY,			state_entity_remove);
	lua_register (s -> lua, WL_EXPIRE_STATE_ENTITY,			state_entity_expire);
	lua_register (s -> lua, WL_MODIFY_STATE_ENTITY,			state_entity_modify);
	lua_register (s -> lua, WL_GET_STATE_ENTITY_DIRECTION,	state_entity_by_hash_get_direction);
	lua_register (s -> lua, WL_GET_STATE_ENTITY_ROW,		state_entity_by_hash_get_row);
	lua_register (s -> lua, WL_GET_STATE_ENTITY_COLUMN,		state_entity_by_hash_get_column);
	lua_register (s -> lua, WL_CREATE_FONT, 				font_create);
	lua_register (s -> lua, WL_CREATE_TEXTBOX,				textbox_enqueue);
	lua_register (s -> lua, WL_GET_PROPERTY, 				property_get_wanderlust_property);
	lua_register (s -> lua, WL_SET_PROPERTY,				property_set_wanderlust_property);

	if (lua_pcall (s -> lua, 0, 0, 0))
	{
		goto FAILURE;
	}

	lua_settop (L, 0);

	return s;

	FAILURE:

		if (s)
		{
			delete_wanderlust_segment (s);
		}

		lua_settop (L, 0);

		return e_configuration.null;
}

static s08 can_scroll_n ()
{
	return (f_segment -> area.y > 0);
}

static s08 can_scroll_e ()
{
	return
		(f_segment -> area.x + property_get_integer ((const s08*)PROPERTY_MAP_VISIBLE_COLUMNS) <
				e_configuration.map.columns);
}


static s08 can_scroll_s ()
{
	return
		(f_segment -> area.y + property_get_integer ((const s08*)PROPERTY_MAP_VISIBLE_ROWS) <
				e_configuration.map.rows);
}

static s08 can_scroll_w ()
{
	return (f_segment -> area.x > 0);
}

static nil scroll_n ()
{
	if (f_segment -> state == STATE_IDLE)
	{
		f_segment -> state = STATE_SCROLL_N;
		f_segment -> steps = e_configuration.tileset.height;
	}
}

static nil scroll_e ()
{
	if (f_segment -> state == STATE_IDLE)
	{
		f_segment -> state = STATE_SCROLL_E;
		f_segment -> steps = e_configuration.tileset.width;
	}
}

static nil scroll_s ()
{
	if (f_segment -> state == STATE_IDLE)
	{
		f_segment -> state = STATE_SCROLL_S;
		f_segment -> steps = e_configuration.tileset.height;
	}
}

static nil scroll_w ()
{
	if (f_segment -> state == STATE_IDLE)
	{
		f_segment -> state = STATE_SCROLL_W;
		f_segment -> steps = e_configuration.tileset.width;
	}
}

static nil update_scroll_n ()
{
	if (f_segment -> steps > 0)
	{
		f_segment -> origin.y	= f_segment -> origin.y + f_segment->iteration;
		f_segment -> steps		= f_segment -> steps - f_segment->iteration;
	}
	else
	{
		f_segment -> state 		= STATE_IDLE;
		f_segment -> area.y		= f_segment -> area.y - e_configuration.map.offsetRow;
		f_segment -> area.h		= f_segment -> area.h - e_configuration.map.offsetRow;
		f_segment -> origin.y	= -e_configuration.tileset.height;
	}
}

static nil update_scroll_e ()
{
	if (f_segment -> steps > 0)
	{
		f_segment -> origin.x	= f_segment -> origin.x - f_segment->iteration;
		f_segment -> steps		= f_segment -> steps - f_segment->iteration;
	}
	else
	{
		f_segment -> state 		= STATE_IDLE;
		f_segment -> area.x		= f_segment -> area.x + e_configuration.map.offsetColumn;
		f_segment -> area.w		= f_segment -> area.w + e_configuration.map.offsetColumn;
		f_segment -> origin.x	= -e_configuration.tileset.width;
	}
}

static nil update_scroll_s ()
{
	if (f_segment -> steps > 0)
	{
		f_segment -> origin.y	= f_segment -> origin.y - f_segment -> iteration;
		f_segment -> steps		= f_segment -> steps - f_segment->iteration;
	}
	else
	{
		f_segment -> state 		= STATE_IDLE;
		f_segment -> area.y		= f_segment -> area.y + e_configuration.map.offsetRow;
		f_segment -> area.h		= f_segment -> area.h + e_configuration.map.offsetRow;
		f_segment -> origin.y	= -e_configuration.tileset.height;
	}
}

static nil update_scroll_w ()
{
	if (f_segment -> steps > 0)
	{
		f_segment -> origin.x	= f_segment -> origin.x + f_segment->iteration;
		f_segment -> steps		= f_segment -> steps - f_segment->iteration;
	}
	else
	{
		f_segment -> state 		= STATE_IDLE;
		f_segment -> area.x		= f_segment -> area.x - e_configuration.map.offsetColumn;
		f_segment -> area.w		= f_segment -> area.w - e_configuration.map.offsetColumn;
		f_segment -> origin.x	= -e_configuration.tileset.width;
	}
}

static nil update_idle ()
{

}

/* ************************************************************************
 *
 * BANKS
 *
 * ***********************************************************************/

static s08 (* can_scroll [SCROLL_TYPES]) () =
{
	can_scroll_n, can_scroll_e, can_scroll_s, can_scroll_w
};

static nil (* scroll [SCROLL_TYPES]) () =
{
	scroll_n, scroll_e, scroll_s, scroll_w
};

static nil (* update_on_state [MAP_STATE_TYPES]) () =
{
	update_scroll_n,
	update_scroll_e,
	update_scroll_s,
	update_scroll_w,

	update_idle
};

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 map_initialize (lua_State * L)
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

	lua_register (L, WL_LOAD_MAP, 				map_load_segment);
	lua_register (L, WL_LOAD_IMAGE,				texture_load_image);
	lua_register (L, WL_SELECT_MAP,				map_select_segment);
	lua_register (L, WL_WARP_MAP,				map_warp);
	lua_register (L, WL_WARP_MAP_BY_TRANSITION,	map_transition);
	lua_register (L, WL_CREATE_TEXTBOX,			textbox_enqueue);

	if (! (f_world = mcreate (M_SINGLE_LINKED_LIST)))
	{
		goto FAILURE;
	}

	if (tileset_initialize (L) == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (event_initialize (L) == e_configuration.failure)
	{
		goto FAILURE;
	}

	task_register (TASK_MAP_LOAD, 	task_load_segment);
	task_register (TASK_MAP_SELECT,	task_select_segment);
	task_register (TASK_MAP_WARP,	task_map_warp);

	return e_configuration.success;

	FAILURE:

		map_terminate ();

		return e_configuration.failure;
}

nil map_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (f_world)
	{
		f_world = mdelete (f_world, delete_wanderlust_segment);
	}

	f_segment		= 0;
	f_initialized	= e_configuration.failure;

	tileset_terminate ();

	event_terminate ();
}

nil map_update ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! f_segment)
	{
		return;
	}

	tileset_update ();

	update_on_state [f_segment -> state] ();
}

nil map_render ()
{
	u08 l = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! f_segment)
	{
		return;
	}

	for (l = 0; l < e_configuration.map.layerEvent; l ++)
	{
		map_render_layer (l);
	}
}

nil map_render_layer (u08 layer)
{
	SDL_Rect target =
	{
		0, 0, 0, 0
	};

	u16 r, c, x, y;

	target.w = e_configuration.tileset.width;
	target.h = e_configuration.tileset.height;

	if (layer >= e_configuration.map.layerEvent)
	{
		return;
	}

	if (! f_segment)
	{
		return;
	}

	target.x = f_segment -> origin.x;

	target.y = f_segment -> origin.y;

	for (r = f_segment -> area.y; r < f_segment -> area.h; r ++)
	{
		for (c = f_segment -> area.x; c < f_segment -> area.w; c++)
		{
			if (f_segment -> layer [layer].data [r][c] != 0xFFFF)
			{
				x = f_segment -> layer [layer].data [r][c] % e_configuration.map.divisor;

				y = f_segment -> layer [layer].data [r][c] / e_configuration.map.divisor;

				tileset_render_distinct_tile
				(
					f_segment -> layer [layer].tileset,
					layer,
					y,
					x,
					& target
				);
			}

			target.x = target.x + e_configuration.tileset.width;
		}

		target.x = f_segment -> origin.x;

		target.y = target.y + e_configuration.tileset.height;
	}
}

nil map_render_range (u08 p, u08 q)
{
	u16 u;

	if (q >= e_configuration.map.layerEvent)
	{
		return;
	}

	if (p >= q)
	{
		return;
	}

	if (! f_segment)
	{
		return;
	}

	for (u = p; u <= q; u ++)
	{
		map_render_layer (u);
	}
}

nil map_scroll (wanderlust_scroll_direction direction)
{
	if (direction >= SCROLL_TYPES)
	{
		return;
	}

	if (! f_segment)
	{
		return;
	}

	if (! can_scroll [direction] ())
	{
		return;
	}

	scroll [direction] ();
}


u08 map_has_focus (u08 r, u08 c)
{
	if (! f_segment)
	{
		return 0;
	}

	return
		((r >= f_segment -> area.y) && (r <= f_segment -> area.h)) &&
		((c >= f_segment -> area.x) && (c <= f_segment -> area.w));
}

u16 map_get_terrain (u08 r, u08 c, s08 v, s08 u)
{
	u08 x, y;
	u16 p;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! f_segment)
	{
		goto FAILURE;
	}

	x = c + u;

	y = r + v;

	p = f_segment -> layer [e_configuration.map.layerTerrain].data [y][x];

	return p;

	FAILURE:

		return e_configuration.failure;
}

nil map_set_terrain (u08 r, u08 c, s08 v, s08 u, u16 p)
{
	u08 x, y;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! f_segment)
	{
		return;
	}

	x = c + u;

	y = r + v;

	f_segment -> layer [e_configuration.map.layerTerrain].data [y][x] = p;
}

u16 map_has_event (u08 r, u08 c, s08 v, s08 u)
{
	u08 x, y;
	u16 e;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! f_segment)
	{
		goto FAILURE;
	}

	x = c + u;

	y = r + v;

	e = f_segment -> layer [e_configuration.map.layerEvent].data [y][x];

	return e < 0x100;

	FAILURE:

		return e_configuration.failure;
}

u08 map_get_event_id (u08 r, u08 c, s08 v, s08 u)
{
	u08 x, y;
	u16 e;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! f_segment)
	{
		goto FAILURE;
	}

	x = c + u;

	y = r + v;

	e = f_segment -> layer [e_configuration.map.layerEvent].data [y][x];

	return (u08) e;

	FAILURE:

		return e_configuration.failure;
}

nil map_trigger_event (u08 r, u08 c, s08 v, s08 u, wanderlust_entity * entity)
{
	u08 x, y;
	u16 e;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! f_segment)
	{
		return;
	}

	x = c + u;

	y = r + v;

	e = f_segment -> layer [e_configuration.map.layerEvent].data [y][x];

	if (e > 0xFF)
	{
		return;
	}

	event_trigger (e, entity);
}

s32 map_load_segment (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_MAP_LOAD, create_wanderlust_segment (L));

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define MAP_SELECT_SEGEMENT_ARG_COUNT		1
#define MAP_SELECT_SEGEMENT_ARG_POS_NAME	1

s32 map_select_segment (lua_State * L)
{
	s32 					hash 	= 0;
	wanderlust_segment	*	segment = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < MAP_SELECT_SEGEMENT_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, MAP_SELECT_SEGEMENT_ARG_POS_NAME))
	{
		goto FAILURE;
	}

	hash 	= mhash (lua_tostring (L, MAP_SELECT_SEGEMENT_ARG_POS_NAME));
	segment	= mfind (f_world, &hash, compare_wanderlust_segment_by_hash, 0);

	if (segment)
	{
		task_enqueue (TASK_MAP_SELECT, segment);
	}

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define MAP_WARP_ARG_COUNT 		2
#define MAP_WARP_ARG_POS_ROW	1
#define MAP_WARP_ARG_POS_COLUMN	2

s32 map_warp (lua_State * L)
{
	wanderlust_warp_point * warp_point = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < MAP_WARP_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, MAP_WARP_ARG_POS_ROW))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, MAP_WARP_ARG_POS_COLUMN))
	{
		goto FAILURE;
	}

	if (! (warp_point = SDL_malloc (sizeof (wanderlust_warp_point))))
	{
		goto FAILURE;
	}

	warp_point->row		= (u08) lua_tointeger (L, MAP_WARP_ARG_POS_ROW);
	warp_point->column	= (u08) lua_tointeger (L, MAP_WARP_ARG_POS_COLUMN);

	task_enqueue (TASK_MAP_WARP, warp_point);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define MAP_TRANSITION_ARG_COUNT			6
#define MAP_TRANSITION_ARG_POS_ROW			1
#define MAP_TRANSITION_ARG_POS_COLUMN		2
#define MAP_TRANSITION_ARG_POS_DIRECTION	3
#define MAP_TRANSITION_ARG_POS_HASH			4
#define MAP_TRANSITION_ARG_POS_TEXTURE		5
#define MAP_TRANSITION_ARG_POS_DURATION		6

s32 map_transition (lua_State * L)
{
	wanderlust_warp_point	*	warp_point	= 0;
	SDL_Texture				*	texture		= 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < MAP_TRANSITION_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, MAP_TRANSITION_ARG_POS_ROW))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, MAP_TRANSITION_ARG_POS_COLUMN))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, MAP_TRANSITION_ARG_POS_DIRECTION))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, MAP_TRANSITION_ARG_POS_HASH))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, MAP_TRANSITION_ARG_POS_TEXTURE))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, MAP_TRANSITION_ARG_POS_DURATION))
	{
		goto FAILURE;
	}

	if (! (warp_point = SDL_malloc (sizeof (wanderlust_warp_point))))
	{
		goto FAILURE;
	}

	warp_point->row			= (u08) lua_tointeger (L, MAP_TRANSITION_ARG_POS_ROW);
	warp_point->column		= (u08) lua_tointeger (L, MAP_TRANSITION_ARG_POS_COLUMN);
	warp_point->direction	= (u08) lua_tointeger (L, MAP_TRANSITION_ARG_POS_DIRECTION);
	warp_point->hash		= (u32) lua_tointeger (L, MAP_TRANSITION_ARG_POS_HASH);

	texture	= texture_get_by_name
	(
		lua_tostring (L, MAP_TRANSITION_ARG_POS_TEXTURE)
	);

	if (! texture)
	{
		goto FAILURE;
	}

	transition_enqueue
	(
		(u08) lua_tointeger (L, MAP_TRANSITION_ARG_POS_DURATION),
		texture,
		task_map_warp,
		warp_point
	);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		if (warp_point)
		{
			SDL_free (warp_point);
		}

		if (L)
		{
			lua_settop (L, 0);
		}

		return e_configuration.failure;
}

u08 map_get_row ()
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! f_segment)
	{
		goto FAILURE;
	}

	return f_segment -> area.y + property_get_integer ((const s08*)PROPERTY_HERO_POSITION_ROW) + e_configuration.map.offsetRow;

	FAILURE:

		return e_configuration.zero;

}

u08 map_get_column ()
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! f_segment)
	{
		goto FAILURE;
	}

	return f_segment -> area.x + property_get_integer ((const s08*)PROPERTY_HERO_POSITION_COLUMN) + e_configuration.map.offsetColumn;

	FAILURE:

		return e_configuration.failure;
}

nil map_set_row (u08 row)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! f_segment)
	{
		return;
	}

	f_segment->area.y = row - e_configuration.map.offsetRow - property_get_integer ((const s08*)PROPERTY_HERO_POSITION_ROW);
	f_segment->area.h = row + property_get_integer ((const s08*)PROPERTY_MAP_VISIBLE_ROWS);
}

nil map_set_column (u08 column)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! f_segment)
	{
		return;
	}

	f_segment -> area.x = column - e_configuration.map.offsetColumn - property_get_integer ((const s08*)PROPERTY_HERO_POSITION_COLUMN);
	f_segment -> area.w = column + property_get_integer ((const s08*)PROPERTY_MAP_VISIBLE_COLUMNS);
}

nil map_set_iterator (u08 value)
{
	if (! f_segment)
	{
		return;
	}

	f_segment->iteration = value;
}

s32 map_get_origin_x ()
{
	if (! f_segment)
	{
		return 0;
	}

	return f_segment -> origin.x;
}

s32 map_get_origin_y ()
{
	if (! f_segment)
	{
		return 0;
	}

	return f_segment -> origin.y;
}

u08 map_get_area_row ()
{
	if (! f_segment)
	{
		return 0;
	}

	return f_segment -> area.y;
}

u08 map_get_area_column ()
{
	if (! f_segment)
	{
		return 0;
	}

	return f_segment -> area.x;
}

u08 map_lua_get_global (const char * callback)
{
	if (! f_segment)
	{
		goto FAILURE;
	}

	if (! f_segment -> lua)
	{
		goto FAILURE;
	}

	if (! callback)
	{
		goto FAILURE;
	}

	lua_getglobal (f_segment -> lua, callback);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

u08 map_lua_pcall (u08 input_count, u08 output_count)
{
	u08 e = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! f_segment)
	{
		goto FAILURE;
	}

	if (! f_segment -> lua)
	{
		goto FAILURE;
	}

	if (lua_pcall (f_segment -> lua, input_count, output_count, 0))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (f_segment -> lua, -1))
	{
		goto FAILURE;
	}

	e = lua_tointeger (f_segment -> lua, -1);

	lua_settop (f_segment->lua, 0);

	return e ? e_configuration.success : e_configuration.failure;

	FAILURE:

		lua_settop (f_segment->lua, 0);

		return e_configuration.failure;
}

u08 map_lua_push_number (u32 number)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! f_segment)
	{
		goto FAILURE;
	}

	if (! f_segment -> lua)
	{
		goto FAILURE;
	}

	lua_pushnumber (f_segment -> lua, number);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}
