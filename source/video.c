#include "type.h"
#include "config.h"
#include "font.h"
#include "task.h"
#include "entity.h"
#include "texture.h"
#include "video.h"
#include "property.h"
#include "video.h"
#include "terminal.h"
#include "transition.h"

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

static s08 				f_initialized 	= WANDERLUST_FAILURE;

static SDL_Renderer	*	f_renderer		= 0;

static SDL_Window	*	f_window		= 0;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - TASKS
 *
 * ************************************************************************/

static nil task_window_show (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	SDL_ShowWindow (f_window);
}

static nil task_window_hide (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	SDL_HideWindow (f_window);
}

static nil task_window_scaling (nil * item)
{

}

static nil task_video_scaling_increment (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (e_configuration.resolution.scaling >= e_configuration.resolution.scalingMax)
	{
		return;
	}

	property_set_integer
	(
		(const s08*)PROPERTY_SCALING,
		e_configuration.resolution.scaling += 1
	);

	SDL_RenderSetScale
	(
		f_renderer,
		e_configuration.resolution.scaling,
		e_configuration.resolution.scaling
	);

	SDL_SetWindowSize
	(
		f_window,
		e_configuration.resolution.scaling * e_configuration.resolution.width,
		e_configuration.resolution.scaling * e_configuration.resolution.height
	);
}

static nil task_video_scaling_decrement (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (e_configuration.resolution.scaling <= e_configuration.resolution.scalingMin)
	{
		return;
	}

	property_set_integer ((const s08*)PROPERTY_SCALING, e_configuration.resolution.scaling -= 1);

	SDL_RenderSetScale
	(
		f_renderer,
		e_configuration.resolution.scaling,
		e_configuration.resolution.scaling
	);

	SDL_SetWindowSize
	(
		f_window,
		e_configuration.resolution.scaling * e_configuration.resolution.width,
		e_configuration.resolution.scaling * e_configuration.resolution.height
	);
}

static nil task_video_fullscreen (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (e_configuration.resolution.fullscreen == SDL_WINDOW_FULLSCREEN)
	{
		return;
	}

	e_configuration.resolution.fullscreen = SDL_WINDOW_FULLSCREEN;

	SDL_SetWindowFullscreen (f_window, e_configuration.resolution.fullscreen);

	property_set_integer ((const s08*)PROPERTY_FULLSCREEN, e_configuration.resolution.fullscreen);
}

static nil task_video_window (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (e_configuration.resolution.fullscreen != SDL_WINDOW_FULLSCREEN)
	{
		return;
	}

	e_configuration.resolution.fullscreen = 0;

	SDL_SetWindowFullscreen (f_window, e_configuration.resolution.fullscreen);

	property_set_integer ((const s08*)PROPERTY_FULLSCREEN, e_configuration.resolution.fullscreen);
}
/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - HELPING HANDS
 *
 * ************************************************************************/

static nil draw_entity_hero (wanderlust_entity * e)
{
	SDL_RenderCopy (f_renderer, e->texture, e->source, & e->target);
}

static nil draw_entity_npc (wanderlust_entity * e)
{
	SDL_RenderCopy (f_renderer, e->texture, e->source, & e->target);
}

static nil draw_entity_retrobutton (wanderlust_entity * e)
{
	SDL_Rect 
		target =
		{
			0, 0, 0, 0
		},
		source = 
		{
			0, 0, 0, 0
		};

	s08 * p = e->title;
/*
	SDL_Rect source =
	{
		0, 0, 0, 0
	};
*/
	target.x = e->target.x;
	target.y = e->target.y;
	target.w = e_configuration.font.dimension;
	target.h = e_configuration.font.dimension;

	source.x = 0;
	source.y = 0;
	source.w = e_configuration.font.dimension;
	source.h = e_configuration.font.dimension;

	while (p ++)
	{
		source.x = (* p % 0x10) * e_configuration.font.dimension;
		source.y = (* p / 0x10) * e_configuration.font.dimension;

		SDL_RenderCopy (f_renderer, e->texture, & source, & target);

		target.x = target.x + e_configuration.font.dimension;
	}
}

static nil draw_entity_primitive (wanderlust_entity * e)
{
	SDL_RenderCopy (f_renderer, e->texture, e->source, & e->target);
}

/* ************************************************************************
 *
 * BANKS
 *
 * ************************************************************************/

static nil (* draw_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	draw_entity_hero,
	draw_entity_npc,
	draw_entity_retrobutton,
	draw_entity_primitive
};

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

s08 video_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized 	= e_configuration.success;
	f_renderer		= 0;
	f_window		= 0;

	if (! L)
	{
		goto FAILURE;
	}

	lua_register (L, WL_SHOW_WINDOW, 			video_show_window);

	lua_register (L, WL_HIDE_WINDOW, 			video_hide_window);

	lua_register (L, WL_SET_FULLSCREEN_MODE, 	video_fullscreen_mode);

	lua_register (L, WL_SET_WINDOW_MODE, 		video_window_mode);

	lua_register (L, WL_INCREMENT_WINDOW_SIZE,	video_increment_window_size);

	lua_register (L, WL_DECREMENT_WINDOW_SIZE,	video_decrement_window_size);

	lua_register (L, WL_CREATE_TRANSITION,		video_enqueue_transition);

	lua_register (L, WL_SET_SCALING_FACTOR,		video_set_scaling_factor);

	if (texture_initialize (L) == e_configuration.failure)
	{
		goto FAILURE;
	}

	e_configuration.resolution.x 		= property_get_integer ((const s08*)PROPERTY_WINDOW_POS_X);
	e_configuration.resolution.y		= property_get_integer ((const s08*)PROPERTY_WINDOW_POS_Y);
	e_configuration.resolution.width	= property_get_integer ((const s08*)PROPERTY_WINDOW_RES_W),
	e_configuration.resolution.height	= property_get_integer ((const s08*)PROPERTY_WINDOW_RES_H),

	property_create_integer
	(
		(const s08*)PROPERTY_SCALING,
		e_configuration.resolution.scaling
	);

	property_create_integer
	(
		(const s08*)PROPERTY_FULLSCREEN,
		e_configuration.resolution.fullscreen
	);

	f_window = SDL_CreateWindow
	(
		WANDERLUST_TITLE,
		e_configuration.resolution.x,
		e_configuration.resolution.y,
		e_configuration.resolution.width,
		e_configuration.resolution.height,
		e_configuration.resolution.hidden | e_configuration.resolution.opengl
	);

	if (! f_window)
	{
		goto FAILURE;
	}

	f_renderer = SDL_CreateRenderer
	(
		f_window,
		-1,
		e_configuration.rendering.accelerated | e_configuration.rendering.presentVSync
	);

	if (! f_renderer)
	{
		goto FAILURE;
	}

	SDL_SetRenderDrawColor
	(
		f_renderer,
		e_configuration.drawing.r,
		e_configuration.drawing.g,
		e_configuration.drawing.b,
		e_configuration.drawing.a
	);

	task_register (TASK_VIDEO_SHOW, 		task_window_show);
	task_register (TASK_VIDEO_HIDE, 		task_window_hide);
	task_register (TASK_VIDEO_SCALING, 		task_window_scaling);
	task_register (TASK_VIDEO_FULLSCREEN, 	task_video_fullscreen);
	task_register (TASK_VIDEO_WINDOW, 		task_video_window);
	task_register (TASK_VIDEO_SCALE_INC, 	task_video_scaling_increment);
	task_register (TASK_VIDEO_SCALE_DEC, 	task_video_scaling_decrement);

	task_enqueue (TASK_VIDEO_SCALING, 0);

	return e_configuration.success;

	FAILURE:

		video_terminate ();

		return e_configuration.failure;
}

nil video_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	texture_terminate ();

	if (f_renderer)
	{
		SDL_DestroyRenderer (f_renderer);
	}

	if (f_window)
	{
		SDL_DestroyWindow (f_window);
	}

	e_configuration.resolution.scaling 	= e_configuration.resolution.scalingDefault;
	f_initialized						= e_configuration.failure;
}

nil	video_render_clear ()
{
	SDL_RenderClear (f_renderer);
}

nil	video_render_present ()
{
	SDL_RenderPresent (f_renderer);
}

nil	video_draw_background (SDL_Texture * texture)
{
	static SDL_Rect source = {0, 0, 0, 0}, target = {0, 0, 0, 0};

/*
	source.x = target.x = 0;
	source.y = target.x = 0;
*/
	source.w = target.w = e_configuration.resolution.width;
	source.h = target.h = e_configuration.resolution.height;

	SDL_RenderCopy (f_renderer, texture, &source, &target);
}

nil video_draw_entity (nil * item)
{
	draw_entity [((wanderlust_entity *) item)->type] (item);
}

nil video_draw_sprite (SDL_Texture * texture, SDL_Rect * source, SDL_Rect * target)
{
	SDL_RenderCopy (f_renderer, texture, source, target);
}

nil	video_draw_tile (SDL_Texture * texture, SDL_Rect * target, SDL_Rect * source)
{
	SDL_RenderCopy (f_renderer, texture, source, target);
}

nil	video_draw_text (SDL_Texture * texture, s08 * text, u16 length, u16 x, u16 y, u16 d)
{
	SDL_Rect 	source	= {0, 0, 0, 0};
	SDL_Rect 	target	= {0, 0, 0, 0};
	u16			i		= 0;

	source.w = target.w = d;
	source.h = target.h = d;
	target.x = x;
	target.y = y;

	for (i = 0; i < length; i++)
	{
		source.x = (text [i] % 0x10) * d;
		source.y = (text [i] / 0x10) * d;

		SDL_RenderCopy (f_renderer, texture, & source, & target);

		target.x += d;
	}
}

nil	video_draw_textbox
(
	SDL_Texture * texture,
	const s08 * text,
	u32 cursor,
	u32 columns,
	wanderlust_frame * f
) {
	SDL_Rect src = {0, 0, 0, 0}, dst = {0, 0, 0, 0};

	u32 c = 0;

	src.w = dst.w = e_configuration.font.dimension;
	src.h = dst.h = e_configuration.font.dimension;
	dst.x = f->fda.dst.x;
	dst.y = f->fda.dst.y;

	for (c = 0; c < cursor; c++)
	{
		if (! (c % columns) && c > 0)
		{
			dst.x = f->fda.dst.x;
			dst.y = dst.y + e_configuration.font.dimension;
		}

		src.x = e_configuration.font.dimension * (text [c] % e_configuration.font.columns);
		src.y = e_configuration.font.dimension * (text [c] / e_configuration.font.rows);

		SDL_RenderCopy (f_renderer, texture, & src, & dst);

		dst.x = dst.x + e_configuration.font.dimension;
	}
}

nil	video_draw_frame (wanderlust_frame * f)
{
	SDL_Rect r = {0, 0, 0, 0};

	SDL_RenderCopy (f_renderer, f->texture, & f->ulc.src, & f->ulc.dst);

	SDL_RenderCopy (f_renderer, f->texture, & f->llc.src, & f->llc.dst);

	SDL_RenderCopy (f_renderer, f->texture, & f->urc.src, & f->urc.dst);

	SDL_RenderCopy (f_renderer, f->texture, & f->lrc.src, & f->lrc.dst);

	r.w = f->dimension;
	r.h = f->dimension;

	for
	(
		r.x = f->tfb.dst.x, r.y = f->tfb.dst.y; r.x < f->tfb.dst.w; r.x += f->dimension
	) {
		SDL_RenderCopy (f_renderer, f->texture, & f->tfb.src, & r);
	}

	for
	(
		r.x = f->bfb.dst.x, r.y = f->bfb.dst.y; r.x < f->bfb.dst.w; r.x += f->dimension
	) {
		SDL_RenderCopy (f_renderer, f->texture, & f->bfb.src, & r);
	}

	for
	(
		r.x = f->lfb.dst.x, r.y = f->lfb.dst.y; r.y < f->lfb.dst.h; r.y += f->dimension
	) {
		SDL_RenderCopy (f_renderer, f->texture, & f->lfb.src, & r);
	}

	for
	(
		r.x = f->rfb.dst.x, r.y = f->rfb.dst.y; r.y < f->rfb.dst.h; r.y += f->dimension
	) {
		SDL_RenderCopy (f_renderer, f->texture, & f->rfb.src, & r);
	}

	for (r.y = f->fda.dst.y; r.y < f->fda.dst.h; r.y += f->dimension)
	{
		for (r.x = f->fda.dst.x; r.x < f->fda.dst.w; r.x += f->dimension)
		{
			SDL_RenderCopy (f_renderer, f->texture, & f->fda.src, & r);
		}
	}
}

nil video_draw_terminal (nil * t)
{
	wanderlust_terminal * 	terminal	= t;
	SDL_Rect				src			= {0, 0, 0, 0};
	SDL_Rect				dst			= {0, 0, 0, 0};
	u32						r			= 0;
	u32						c			= 0;
	u08						s			= 0;

	src.w = dst.w = e_configuration.font.dimension;
	src.h = dst.h = e_configuration.font.dimension;

	if (! terminal)
	{
		return;
	}

	video_draw_frame (terminal->frame);

	dst.y = terminal->frame->fda.dst.y;

	for (r = 0; r < terminal->buffer.rows; r ++)
	{
		dst.x = terminal->frame->fda.dst.x - e_configuration.font.dimension;

		for (c = 0; c < terminal->buffer.cols; c ++)
		{
			dst.x += e_configuration.font.dimension;

			if (! (s = terminal->buffer.data [(r * terminal->buffer.cols) + c]))
			{
				continue;
			}

			src.x	= (s & 0xF) * e_configuration.font.dimension;
			src.y	= ((s >> 4) & 0xF) * e_configuration.font.dimension;

			if (!r)
			{
				SDL_RenderCopy (f_renderer, terminal->cursor.active, &src, &dst);
			}
			else
			{
				SDL_RenderCopy (f_renderer, terminal->cursor.inactive, &src, &dst);
			}
		}

		dst.y += e_configuration.font.dimension;
	}
}

s32 video_show_window (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_VIDEO_SHOW, 0);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 video_hide_window (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_VIDEO_HIDE, 0);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32	video_fullscreen_mode (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_VIDEO_FULLSCREEN, 0);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32 video_window_mode (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_VIDEO_WINDOW, 0);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32	video_set_scaling_factor (lua_State * L)
{
	u08	scaling = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	scaling = property_get_integer ((const s08*)PROPERTY_SCALING);

	if (scaling == e_configuration.resolution.scaling)
	{
		goto SUCCESS;
	}

	if (scaling < e_configuration.resolution.scaling)
	{
		task_enqueue (TASK_VIDEO_SCALE_DEC, 0);
	}
	else
	{
		task_enqueue (TASK_VIDEO_SCALE_INC, 0);
	}

	SUCCESS:

		lua_settop (L, 0);

		return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32	video_increment_window_size (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_VIDEO_SCALE_INC, 0);

	return e_configuration.success;

	lua_settop (L, 0);

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32	video_decrement_window_size (lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	task_enqueue (TASK_VIDEO_SCALE_DEC, 0);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define VIDEO_ENQUEUE_TRANSITION_ARG_COUNT			2

#define VIDEO_ENQUEUE_TRANSITION_ARG_POS_IMAGE		1

#define VIDEO_ENQUEUE_TRANSITION_ARG_POS_DURATION	2

s32 video_enqueue_transition (lua_State * L)
{
	SDL_Texture	* texture = e_configuration.null;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < VIDEO_ENQUEUE_TRANSITION_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, VIDEO_ENQUEUE_TRANSITION_ARG_POS_IMAGE))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, VIDEO_ENQUEUE_TRANSITION_ARG_POS_DURATION))
	{
		goto FAILURE;
	}

	texture = texture_get_by_name
	(
		lua_tostring (L, VIDEO_ENQUEUE_TRANSITION_ARG_POS_IMAGE)
	);

	if (! texture)
	{
		goto FAILURE;
	}

	transition_enqueue
	(
		(u08) lua_tointeger (L, VIDEO_ENQUEUE_TRANSITION_ARG_POS_DURATION),
		texture,
		0,
		0
	);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

SDL_Texture * video_create_texture_from_surface (SDL_Surface * surface)
{
	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! surface)
	{
		goto FAILURE;
	}

	return SDL_CreateTextureFromSurface (f_renderer, surface);

	FAILURE:

		return e_configuration.null;
}
