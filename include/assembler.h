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

#ifndef WANDERLUST_ASSEMBLER
#define WANDERLUST_ASSEMBLER

s08 assembler_initialize (lua_State *);

nil assembler_terminate ();

#define N(SUFFIX) TEXT_ ## SUFFIX

typedef enum {
  N(__), N(R_), N(RB), N(RR), N(P_), N(PW), N(C_), N(B_), N(W_), N(_R), N(MC)
} instruction_t;

u08 assembler_get_type (u08);

u08 assembler_get_byte (u16);

nil assembler_set_byte (u16, u08);

nil assembler_set_byte_increment (u16, u08);

nil assembler_continue (u16);

u08 assembler_writeline (u16, const u08*, u32);

u08 assembler_writeline_data (u16, const u08*, u32);

u08 assembler_writeline_offset (u16, const u08*, u32, u16);

u08 assembler_writeline_byte (u16, const u08*, u32, u16);

u16 assembler_get_cursor ();

nil assembler_set_cursor (u16);

nil assembler_inc_cursor (u16);

s32 assembler_inr_cursor (lua_State *);

nil assembler_dec_cursor (u16);

s32 assembler_dcr_cursor (lua_State *);

u16 assembler_get_next_index (unsigned index);

u16 assembler_get_prev_index (unsigned index);

s32 assembler_program_run (lua_State*);

s32 assembler_load_rom (lua_State*);

#endif