#ifndef WANDERLUST_KEYBOARD
#define WANDERLUST_KEYBOARD

s08 kb_initialize (lua_State *);

nil kb_terminate ();

nil kb_set_button (u16, u08);

u08 kb_any_key_down ();

s08 kb_get_button (u16);

s08 kb_get_buffered_button (u16);

#endif
