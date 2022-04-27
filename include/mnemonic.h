/*
 * Wanderlust is an engine for rpgs rendered in top-down perspective, which
 * where common in the consoles of the 8- and 16-Bit Era.
 * Copyright (C) 2022 Stefan Reich
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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