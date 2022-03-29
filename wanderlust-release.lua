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

RES_W 				= 320
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
