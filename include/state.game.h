#ifndef WANDERLUST_STATE_GAME
#define WANDERLUST_STATE_GAME

wanderlust_state	* 	state_game_create (lua_State *);

nil						state_game_direct (wanderlust_state *);

nil						state_game_delete (wanderlust_state *);

nil						state_game_handle (wanderlust_state *);

nil						state_game_update (wanderlust_state *);

nil						state_game_render (wanderlust_state *);

nil						state_game_adjust (wanderlust_state *);

#endif
