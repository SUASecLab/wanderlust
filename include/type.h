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

#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <math.h>

#ifndef WANDERLUST_TYPES
#define WANDERLUST_TYPES

/* ************************************************************************
 *
 * TYPE DEFINITIONS
 *
 * ************************************************************************/

typedef unsigned 	char 	u08;

typedef unsigned 	short	u16;

typedef unsigned	int		u32;

typedef signed		char	s08;

typedef signed		short	s16;

typedef signed		int		s32;

typedef				void	nil;

typedef struct
{
	/*
	 * UPPER LEFT CORNER
	 */
	struct
	{
		SDL_Rect src;
		SDL_Rect dst;
	} ulc;

	/*
	 * LOWER LEFT CORNER
	 */
	struct
	{
		SDL_Rect src;
		SDL_Rect dst;
	} llc;

	/*
	 * UPPER RIGHT CORNER
	 */
	struct
	{
		SDL_Rect src;
		SDL_Rect dst;
	} urc;

	/*
	 * LOWER RIGHT CORNER
	 */
	struct
	{
		SDL_Rect src;
		SDL_Rect dst;
	} lrc;

	/*
	 * TOP FRAME BORDER
	 */
	struct
	{
		SDL_Rect dst;
		SDL_Rect src;
	} tfb;

	/*
	 * BOTTOM FRAME BORDER
	 */
	struct
	{
		SDL_Rect dst;
		SDL_Rect src;
	} bfb;

	/*
	 * LEFT FRAME BORDER
	 */
	struct
	{
		SDL_Rect dst;
		SDL_Rect src;
	} lfb;

	/*
	 * RIGHT FRAME BORDER
	 */
	struct
	{
		SDL_Rect dst;
		SDL_Rect src;
	} rfb;

	/*
	 * FRAME DRAWABLE AREA
	 */
	struct
	{
		SDL_Rect dst;
		SDL_Rect src;
	} fda;

	SDL_Texture 	* 	texture;

	u32					dimension;

	u16					rows;

	u16					columns;

} wanderlust_frame;

typedef struct
{
	const	u32 			hash;
	const	u08				dimension;

			SDL_Texture	*	texture;

	struct
	{
		u32 r;
		u32 g;
		u32 b;
		u32 a;
	} Mask;

	struct
	{
		u32 fg;
		u32 bg;
	} Color;

} wanderlust_font;

typedef enum
{
	WANDERLUST_DIRECTION_N,
	WANDERLUST_DIRECTION_E,
	WANDERLUST_DIRECTION_S,
	WANDERLUST_DIRECTION_W,
	WANDERLUST_DIRECTIONS
}
wanderlust_direction;

typedef enum
{
	PROPERTY_INTEGER,
	PROPERTY_STRING,
	PROPERTY_POINT,
	PROPERTY_RECTANGLE,
	PROPERTY_TYPES
}
wanderlust_property_type;

typedef struct
{
	const s32	type;

	const u32						hash;

	union
	{
		u32				integer;
		s08			*	string;
		SDL_Point		point;
		SDL_Rect		rectangle;
	} data;
}
wanderlust_property;

/* ************************************************************************
 *
 * CONSTANTS
 *
 * ************************************************************************/

#define WANDERLUST_TITLE					"wanderlust"

#define WANDERLUST_SUCCESS					1

#define WANDERLUST_FAILURE					0

#define WANDERLUST_MAP_ROWS					256

#define WANDERLUST_MAP_COLUMNS				256

/* ************************************************************************
 *
 * CONSTANTS - LUA FUNCTIONS
 *
 * ************************************************************************/

#define WL_INIT_PROPERTIES					"init_properties"

#define WL_INIT_DEFAULT						"init"

#define WL_HALT_ENGINE						"wl_halt_engine"

#define WL_LOAD_IMAGE						"wl_load_image"

#define	WL_RUNDOWN_TASK						"wl_rundown_task"

#define WL_LOAD_MAP							"wl_load_map"

#define	WL_SELECT_MAP						"wl_select_map"

#define WL_ENTER_STATE						"wl_enter_state"

#define WL_ENTER_STATE_BY_TRANSITION		"wl_enter_state_by_transition"

#define WL_LEAVE_STATE						"wl_leave_state"

#define WL_LEAVE_STATE_BY_TRANSITION		"wl_leave_state_by_transition"

#define WL_FETCH_STATE						"wl_fetch_state"

#define	WL_CLEAR_STATE						"wl_clear_state"

#define WL_SELECT_STATE_ENTITY				"wl_select_state_entity"

#define WL_REMOVE_STATE_ENTITY				"wl_remove_state_entity"

#define WL_MODIFY_STATE_ENTITY				"wl_modify_state_entity"

#define WL_GET_STATE_ENTITY_DIRECTION		"wl_get_state_entity_by_hash_direction"

#define WL_GET_STATE_ENTITY_ROW				"wl_get_state_entity_by_hash_row"

#define WL_SET_BACKGROUND					"wl_set_background"

#define WL_GET_STATE_ENTITY_COLUMN			"wl_get_state_entity_by_hash_column"

#define WL_EXPIRE_STATE_ENTITY				"wl_expire_state_entity"

#define WL_CREATE_ENTITY					"wl_create_entity"

#define WL_CREATE_FONT						"wl_create_font"

#define WL_CREATE_TRANSITION				"wl_create_transition"

#define WL_CREATE_MENU						"wl_create_menu"

#define WL_REPEAL_MENU						"wl_repeal_menu"

#define WL_APPEND_MENU						"wl_append_menu"

#define WL_SELECT_MENU_ENTITY				"wl_select_menu_entity"

#define WL_CREATE_TEXTBOX					"wl_create_textbox"

#define WL_GET_PROPERTY						"wl_get_property"

#define WL_SET_PROPERTY						"wl_set_property"

#define WL_SET_SCALING_FACTOR				"wl_set_scaling_factor"

#define WL_SCALING_FACTOR_INCREMENT			"wl_inc_scaling_factor"

#define WL_SCALING_FACTOR_DECREMENT			"wl_dec_scaling_factor"

#define WL_SET_FULLSCREEN_MODE				"wl_set_fullscreen_mode"

#define WL_SET_WINDOW_MODE					"wl_set_window_mode"

#define WL_SHOW_WINDOW						"wl_show_window"

#define WL_HIDE_WINDOW						"wl_hide_window"

#define WL_INCREMENT_WINDOW_SIZE			"wl_inc_window_size"

#define WL_DECREMENT_WINDOW_SIZE			"wl_dec_window_size"

#define WL_LOAD_CONTROLLER_MAPPING			"wl_load_controller_mapping"

#define WL_WARP_MAP							"wl_warp_map"

#define WL_WARP_MAP_BY_TRANSITION			"wl_warp_map_by_transition"

#define WL_ATTACH_EVENT						"wl_attach_event"

#define WL_DEATTACH_EVENT					"wl_deattach_event"

#define WL_CREATE_TERMINAL					"wl_create_terminal"

#define WL_DELETE_TERMINAL					"wl_delete_terminal"

#define WL_SHOW_TERMINAL					"wl_show_terminal"

#define WL_HIDE_TERMINAL					"wl_hide_terminal"

#define WL_PUSH_BYTE_TERMINAL				"wl_push_byte_terminal"

#define WL_UNDO_BYTE_TERMINAL				"wl_undo_byte_terminal"

#define WL_INCREMENT_CURSOR_TERMINAL		"wl_increment_cursor_terminal"

#define WL_DECREMENT_CURSOR_TERMINAL		"wl_decrement_cursor_terminal"

#define WL_RUN_TERMINAL						"wl_run_terminal"

#define WL_CLEAR_TERMINAL					"wl_clear_terminal"

#define WL_SUCCESS							1

#define WL_UNDEFINED						0

#define WL_FAILURE							-1

#define WL_MNEMONIC_BASE					"wl_mnemonic_set_base"

#define WL_MNEMONIC_TARGET_REGISTER			"wl_mnemonic_set_target_register"

#define WL_MNEMONIC_TARGET_REGISTER_PAIR	"wl_mnemonic_set_target_register_pair"

#define WL_MNEMONIC_SOURCE_REGISTER			"wl_mnemonic_set_source_register"

#define WL_MNEMONIC_SOURCE_REGISTER_PAIR	"wl_mnemonic_set_source_register_pair"

#define WL_MNEMONIC_TARGET_CONSTANT			"wl_mnemonic_set_target_constant"

#define WL_MNEMONIC_SOURCE_BYTE_A			"wl_mnemonic_set_source_byte_na"

#define WL_MNEMONIC_SOURCE_BYTE_B			"wl_mnemonic_set_source_byte_nb"

#define WL_MNEMONIC_SOURCE_WORD_A			"wl_mnemonic_set_source_word_na"

#define WL_MNEMONIC_SOURCE_WORD_B			"wl_mnemonic_set_source_word_nb"

#define WL_MNEMONIC_SOURCE_WORD_C			"wl_mnemonic_set_source_word_nc"

#define WL_MNEMONIC_SOURCE_WORD_D			"wl_mnemonic_set_source_word_nd"

#define WL_MNEMONIC_SOURCE_BYTE				"wl_mnemonic_write_byte"

#define WL_MNEMONIC_APPLY					"wl_mnemonic_apply"

#define WL_ASSEMBLER_INR_CURSOR				"wl_assembler_inr_cursor"

#define WL_ASSEMBLER_DCR_CURSOR				"wl_assembler_dcr_cursor"

#define WL_ASSEMBLER_PROGRAM_RUN			"wl_assembler_program_run"

#define WL_ASSEMBLER_LOAD_ROM				"wl_assembler_rom_load"

#define WL_CORE_GET_OUTPUT					"wl_core_get_output"

#define WL_TERMINAL_TEXT_MODE				"wl_terminal_text_mode"

#define WL_TERMINAL_DATA_MODE				"wl_terminal_data_mode"

/* ************************************************************************
 *
 * CONSTANTS - PROPERTIES
 *
 * ************************************************************************/

#define PROPERTY_SCALING					"WL_SCALING"

#define PROPERTY_FULLSCREEN					"WL_FULLSCREEN"

#define	PROPERTY_SILENT						"WL_SILENT"

#define PROPERTY_MUSIC_ON					"WL_MUSIC"

#define PROPERTY_EFFECTS_ON					"WL_EFFECTS"

#define PROPERTY_TITLE						"WL_TITLE"

#define PROPERTY_WINDOW_POS_X				"WL_WINDOW_POS_X"

#define PROPERTY_WINDOW_POS_Y				"WL_WINDOW_POS_Y"

#define PROPERTY_WINDOW_RES_W				"WL_WINDOW_RES_W"

#define PROPERTY_WINDOW_RES_H				"WL_WINDOW_RES_H"

#define PROPERTY_MAP_TILE_EDGE_LENGTH		"WL_MAP_TILE_EDGE_LENGTH"

#define PROPERTY_MAP_VISIBLE_ROWS			"WL_MAP_VISIBLE_ROWS"

#define PROPERTY_MAP_VISIBLE_COLUMNS		"WL_MAP_VISIBLE_COLUMNS"

#define PROPERTY_HERO_POSITION_ROW			"WL_HERO_POSITION_ROW"

#define PROPERTY_HERO_POSITION_COLUMN		"WL_HERO_POSITION_COLUMN"

/* ************************************************************************
 *
 * MACRO
 *
 * ************************************************************************/

#define SET_U08_CONST(C,V)	\
	* ((u08 *) &C) = (u08)V

#define SET_U16_CONST(C,V)	\
	* ((u16 *) &C) = (u16)V

#define SET_U32_CONST(C,V)	\
	* ((u32 *) &C) = (u32)V

#define SET_S08_CONST(C,V)	\
	* ((s08 *) &C) = (s08)V

#define SET_S16_CONST(C,V)	\
	* ((s16 *) &C) = ((s16))V

#define SET_S32_CONST(C,V)	\
	* ((s32 *) &C) = (s32)V

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

char * type_secure_copy_string (const char *);

#endif
