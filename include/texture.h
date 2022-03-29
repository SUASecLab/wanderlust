#ifndef WANDERLUST_TEXTURE
#define WANDERLUST_TEXTURE

s08 			texture_initialize (lua_State *);

nil 			texture_terminate ();

SDL_Texture * 	texture_get_by_hash (u32);

SDL_Texture * 	texture_get_by_name (const char *);

s32 			texture_load_image (lua_State *);

#endif
