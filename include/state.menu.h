#ifndef WANDERLUST_STATE_MENU
#define WANDERLUST_STATE_MENU

wanderlust_state	* 	state_menu_create (lua_State *);

nil						state_menu_direct (wanderlust_state *);

nil						state_menu_delete (wanderlust_state *);

nil						state_menu_handle (wanderlust_state *);

nil						state_menu_update (wanderlust_state *);

nil						state_menu_render (wanderlust_state *);

nil						state_menu_adjust (wanderlust_state *);

#endif
