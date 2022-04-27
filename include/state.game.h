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

#ifndef WANDERLUST_STATE_GAME
#define WANDERLUST_STATE_GAME

wanderlust_state	* 	state_game_create (lua_State *);

nil						state_game_direct (wanderlust_state *);

nil						state_game_delete (wanderlust_state *);

nil						state_game_handle (wanderlust_state *);

nil						state_game_update (wanderlust_state *);

nil						state_game_render (wanderlust_state *);

nil						state_game_adjust (wanderlust_state *);

#endif
