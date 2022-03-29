#ifndef WANDERLUST_PROPERTY
#define WANDERLUST_PROPERTY

s08		 		property_initialize (lua_State *);

nil 			property_terminate ();

nil				property_create_integer (const s08 *, u32);

nil				property_create_string (const s08 *, const s08 *);

nil				property_create_point (const s08 *, u32, u32);

nil				property_create_rectangle (const s08 *, u32, u32, u32, u32);

s32				property_delete_wanderlust_property (lua_State *);

s32				property_get_wanderlust_property (lua_State *);

s32				property_set_wanderlust_property (lua_State *);

u32				property_get_integer (const s08 *);

nil				property_set_integer (const s08 *, int);

const s08	*	property_get_string (const s08 *);

nil				property_set_string (const s08 *, const s08 *);

SDL_Point		property_get_point (const s08 *);

nil				property_set_point (const s08 *, SDL_Point *);

SDL_Rect		property_get_rectangle (const s08 *);

nil				property_set_rectangle (const s08 *, SDL_Rect *);

#endif
