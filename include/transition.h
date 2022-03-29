#ifndef WANDERLUST_TRANSITION
#define WANDERLUST_TRANSITION

s08 transition_initialize (lua_State *);

nil transition_terminate ();

nil transition_enqueue (u08, SDL_Texture *, nil (*)(nil *), nil *);

u08 transition_pending ();

nil transition_update ();

nil transition_render ();

#endif
