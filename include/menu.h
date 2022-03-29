#ifndef WANDERLUST_MENU
#define WANDERLUST_MENU

s08 menu_initialize (lua_State *);

nil menu_terminate ();

s32 menu_select (lua_State *);

s32 menu_repeal (lua_State *);

s32 menu_create (lua_State *);

s32 menu_append (lua_State *);

s08 menu_active ();

nil menu_handle ();

nil menu_update ();

nil menu_render ();

#endif
