#ifndef WANDERLUST_MNEMONIC
#define WANDERLUST_MNEMONIC

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 mnemonic_initialize (lua_State *);

nil mnemonic_terminate ();

s32 mnemonic_reset (lua_State*);

s32 mnemonic_set_base (lua_State*);

s32 mnemonic_set_target_register (lua_State*);

s32 mnemonic_set_target_register_pair (lua_State*);

s32 mnemonic_set_source_register (lua_State*);

s32 mnemonic_set_source_register_pair (lua_State*);

s32 mnemonic_set_target_constant (lua_State*);

s32 mnemonic_set_source_byte_na (lua_State*);

s32 mnemonic_set_source_byte_nb (lua_State*);

s32 mnemonic_set_source_word_na (lua_State*);

s32 mnemonic_set_source_word_nb (lua_State*);

s32 mnemonic_set_source_word_nc (lua_State*);

s32 mnemonic_set_source_word_nd (lua_State*);

s32 mnemonic_set_source_byte (lua_State*);

s32 mnemonic_set_apply (lua_State*);

#endif