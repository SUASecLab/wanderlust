-- Wanderlust is an engine for rpgs rendered in top-down perspective, which
-- where common in the consoles of the 8- and 16-Bit Era.
-- Copyright (C) 2022 Stefan Reich
--
-- This program is free software: you can redistribute it and/or modify it
-- under the terms of the GNU General Public License as published by the Free
-- Software Foundation, either version 3 of the License, or (at your option)
-- any later version.
--
-- This program is distributed in the hope that it will be useful, but WITHOUT
-- ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
-- FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
-- more details.
--
-- You should have received a copy of the GNU General Public License along with
-- this program.  If not, see <http://www.gnu.org/licenses/>.
--

function init ()
	wl_load_controller_mapping ("gpdb")
	wl_enter_state (1, "script/title.state.lua")
	wl_inc_window_size ()
	wl_show_window ()
end

PTYPE_INT = 0
PTYPE_STR = 1
PTYPE_PNT = 2
PTYPE_REC = 3

RES_W 				= 336
RES_H 				= 240
TILE_EDGE_LENGTH	= 16

function init_properties ()											 
	wl_set_property (PTYPE_STR, "WL_TITLE", "wanderlust")
	wl_set_property (PTYPE_INT, "WL_WINDOW_POS_X", 					0x2FFF0000)
	wl_set_property (PTYPE_INT, "WL_WINDOW_POS_Y", 					0x2FFF0000)
	wl_set_property (PTYPE_INT, "WL_WINDOW_RES_H", 					RES_H)
	wl_set_property (PTYPE_INT, "WL_WINDOW_RES_W", 					RES_W)
	wl_set_property (PTYPE_INT, "WL_MAP_TILE_EDGE_LENGTH",	TILE_EDGE_LENGTH)
	wl_set_property (PTYPE_INT, "WL_MAP_VISIBLE_ROWS", 			(RES_H / TILE_EDGE_LENGTH) + 2)
	wl_set_property (PTYPE_INT, "WL_MAP_VISIBLE_COLUMNS", 	(RES_W / TILE_EDGE_LENGTH) + 2)
	wl_set_property (PTYPE_INT, "WL_HERO_POSITION_ROW",			((RES_H / TILE_EDGE_LENGTH) / 2))
	wl_set_property (PTYPE_INT, "WL_HERO_POSITION_COLUMN",	((RES_W / TILE_EDGE_LENGTH) / 2))	
end

--init ()
