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

#ifndef WANDERLUST_TEXTBOX
#define WANDERLUST_TEXTBOX

/* ************************************************************************
 *
 * MACRO
 *
 * ***********************************************************************/

#define TEXTBOX_UL_CORNER_X 40

#define TEXTBOX_UL_CORNER_Y 184

#define TEXTBOX_LL_CORNER_X	40

#define TEXTBOX_LL_CORNER_Y 224

#define TEXTBOX_UR_CORNER_X 288

#define TEXTBOX_UR_CORNER_Y 184

#define TEXTBOX_LR_CORNER_X	288

#define TEXTBOX_LR_CORNER_Y 224

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 textbox_initialize (lua_State *);

nil textbox_terminate ();

s08 textbox_pending ();

s32 textbox_enqueue (lua_State *);

nil	textbox_handle ();

nil textbox_update ();

nil textbox_render ();

#endif
