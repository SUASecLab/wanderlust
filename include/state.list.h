#ifndef WANDERLUST_STATE_LIST
#define WANDERLUST_STATE_LIST

wanderlust_state	* 	state_list_create (lua_State *);

nil						state_list_direct (wanderlust_state *);

nil						state_list_delete (wanderlust_state *);

nil						state_list_handle (wanderlust_state *);

nil 					state_list_update (wanderlust_state *);

nil 					state_list_render (wanderlust_state *);

nil						state_list_adjust (wanderlust_state *);

#endif
