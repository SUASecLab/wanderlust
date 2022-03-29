#ifndef WANDERLUST_TERMINAL
#define WANDERLUST_TERMINAL

/* ************************************************************************
 *
 * TYPE DEFINITIONS
 *
 * ************************************************************************/

typedef struct
{
	const	s08 					active;

			wanderlust_frame	*	frame;
			lua_State			*	script;

	struct {
					u08 *	data;
			const	u08		rows;
			const	u08		cols;
			const	u16		size;
	} buffer;

	struct {
		u08				r;
		u08				c;
		u16				index;
		SDL_Texture	*	active;
		SDL_Texture	*	inactive;
	} cursor;
} wanderlust_terminal;

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

s08 terminal_initialize (lua_State *);

nil terminal_terminate ();

s08 terminal_active ();

s32 terminal_create (lua_State *);

s32 terminal_delete (lua_State *);

s32 terminal_show (lua_State *);

s32 terminal_hide (lua_State *);

s32 terminal_push_byte (lua_State *);

s32 terminal_undo_byte (lua_State *);

s32 terminal_increment_cursor (lua_State *);

s32 terminal_decrement_cursor (lua_State *);

s32 terminal_text_mode (lua_State *);

s32 terminal_data_mode (lua_State *);

s32 terminal_run (lua_State *);

s32 terminal_clear (lua_State *);

nil terminal_handle ();

nil terminal_update ();

nil terminal_render ();

#endif
