#ifndef WANDERLUST_VIDEO
#define WANDERLUST_VIDEO

s08 			video_initialize (lua_State *);

nil 			video_terminate ();

nil				video_render_clear ();

nil				video_render_present ();

nil				video_draw_background (SDL_Texture *);

nil				video_draw_entity (nil *);

nil				video_draw_sprite (SDL_Texture *, SDL_Rect*, SDL_Rect *);

nil				video_draw_tile (SDL_Texture *, SDL_Rect *, SDL_Rect *);

nil				video_draw_text (SDL_Texture *, s08 *, u16, u16, u16, u16);

nil				video_draw_textbox (SDL_Texture *, const s08 *, u32, u32, wanderlust_frame *);

nil				video_draw_frame (wanderlust_frame *);

nil				video_draw_terminal (nil *);

s32 			video_show_window (lua_State *);

s32 			video_hide_window (lua_State *);

s32				video_fullscreen_mode (lua_State *);

s32				video_window_mode (lua_State *);

s32				video_set_scaling_factor (lua_State *);

s32				video_increment_window_size (lua_State *);

s32				video_decrement_window_size (lua_State *);

s32				video_enqueue_transition (lua_State *);

SDL_Texture	*	video_create_texture_from_surface (SDL_Surface *);

#endif
