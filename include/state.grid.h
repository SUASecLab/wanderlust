#ifndef WANDERLUST_STATE_GRID
#define WANDERLUST_STATE_GRID

wanderlust_state	* 	state_grid_create (lua_State *);

nil						state_grid_direct (wanderlust_state *);

nil						state_grid_delete (wanderlust_state *);

nil						state_grid_handle (wanderlust_state *);

nil 					state_grid_update (wanderlust_state *);

nil						state_grid_render (wanderlust_state *);

nil						state_grid_adjust (wanderlust_state *);

#endif
