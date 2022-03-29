#ifndef WANDERLUST_ENGINE
#define WANDERLUST_ENGINE

s08 engine_bootstrap (const char *);

s08 engine_initialize ();

s08 engine_terminate ();

s08 engine_execute ();

u08 engine_get_scaling_factor ();

u32 engine_get_cycle ();

s32 engine_quit (lua_State *);

nil engine_print_version ();


#endif
