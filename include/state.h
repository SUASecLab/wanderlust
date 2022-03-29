#ifndef WANDERLUST_STATE
#define WANDERLUST_STATE

/* ************************************************************************
 *
 * TYPEDEFINITIONS
 *
 * ***********************************************************************/

typedef struct
{
	const	u08				type;
	const	u08				locked;

			nil			* 	entities;

			SDL_Texture	*	background;

			lua_State	*	lua;
} wanderlust_state;

typedef enum
{
	GAME_STATE,
	MENU_STATE,
	LIST_STATE,
	GRID_STATE,
	STATE_TYPE_COUNT
} wanderlust_state_types;

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 	state_initialize (lua_State *);

nil 	state_terminate ();

s32 	state_enter (lua_State *);

s32		state_leave (lua_State *);

s32		state_enter_by_transition (lua_State *);

s32		state_leave_by_transition (lua_State *);

s32 	state_fetch (lua_State *);

s32 	state_clear (lua_State *);

s32		state_entity_select (lua_State *);

s32		state_entity_remove (lua_State *);

s32		state_entity_expire (lua_State *);

s32		state_entity_modify (lua_State *);

s32		state_entity_by_hash_get_direction (lua_State *);

s32		state_entity_by_hash_get_row (lua_State *);

s32		state_entity_by_hash_get_column (lua_State *);

s32		state_set_background (lua_State *);

u08		state_entity_has_collision (SDL_Point *);

nil		state_entity_excite (SDL_Point *);

nil 	state_direct ();

nil 	state_handle ();

nil 	state_update ();

nil		state_render ();

nil		state_adjust ();

nil * 	state_get_successor (nil *);

nil * 	state_get_predecessor (nil *);

#endif
