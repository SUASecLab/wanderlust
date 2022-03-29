#ifndef REFRAIN_SYMBOL
#define REFRAIN_SYMBOL

#define SYM_COLS 16
#define SYM_ROWS 16

/*******************************************************************************
 *
 * INTERFACE
 *
 ******************************************************************************/
s08 font_initialize (lua_State *);

nil font_terminate ();

s32 font_create (lua_State *);

wanderlust_font * font_get (u32);

#endif
