#ifndef WANDERLUST_TILESET
#define WANDERLUST_TILESET

s08 tileset_initialize (lua_State * L);

nil tileset_terminate ();

nil tileset_update ();

nil tileset_render_distinct_tile (SDL_Texture *, u08, u08, u08, SDL_Rect *);

#endif
