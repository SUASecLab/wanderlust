#ifndef WANDERLUST_MAP
#define WANDERLUST_MAP

/* ************************************************************************
 *
 * TYPE DEFINITIONS
 *
 * ***********************************************************************/

typedef enum
{
	SCROLL_N,
	SCROLL_E,
	SCROLL_S,
	SCROLL_W,
	SCROLL_TYPES
} wanderlust_scroll_direction;

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 map_initialize (lua_State *);

nil map_terminate ();

nil map_update ();

nil map_render ();

nil map_render_layer (u08);

nil map_render_range (u08, u08);

nil map_scroll (wanderlust_scroll_direction);

u08 map_has_focus (u08, u08);

u16 map_get_terrain (u08, u08, s08, s08);

nil map_set_terrain (u08, u08, s08, s08, u16);

u16 map_has_event (u08, u08, s08, s08);

u08 map_get_event_id (u08, u08, s08, s08);

nil map_trigger_event (u08, u08, s08, s08, wanderlust_entity *);

s32 map_load_segment (lua_State *);

s32 map_select_segment (lua_State *);

s32 map_warp (lua_State *);

s32 map_transition (lua_State *);

u08 map_get_row ();

u08 map_get_column ();

nil map_set_row (u08);

nil map_set_column (u08);

nil map_set_iterator (u08);

s32 map_get_origin_x ();

s32 map_get_origin_y ();

u08 map_get_area_row ();

u08 map_get_area_column ();

u08 map_lua_push_number (u32);

u08 map_lua_pcall (u08, u08);

u08 map_lua_get_global (const char *);

#endif
