#ifndef ATX2_CORE
#define ATX2_CORE
	
s32 core_init ();

s32 core_quit ();

s32 core_inte (void *, unsigned char);

nil core_load (unsigned char*, unsigned);

nil core_save (unsigned char*, unsigned);

s32 core_execute ();

u08 core_get_port_o (u08);

nil core_set_port_i (u08, u08);

s32 core_get_output (lua_State*);

#endif
