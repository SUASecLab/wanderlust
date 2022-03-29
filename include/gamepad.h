#ifndef WANDERLUST_GAMEPAD
#define WANDERLUST_GAMEPAD

s32 gp_initialize (lua_State *);

nil gp_terminate ();

nil gp_device_insert ();

nil gp_device_remove (s32);

nil gp_set_axis (u08, s16);

nil gp_set_button (u16, u08);

u08 gp_get_button (u16);

u08 gp_get_buffered_button (u16);

s32 gp_load_mapping_from_file (lua_State *);

#endif
