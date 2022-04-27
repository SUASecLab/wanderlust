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

#ifndef WANDERLUST_CONFIG
#define WANDERLUST_CONFIG

typedef enum
{
	SS_CONFIGURATION = 0,
	SS_TASK,
	SS_VIDEO,
	SS_INPUT,
	SS_TRANSITION,
	SS_STATE,
	SS_ENTITY,
	SS_MAP,
	SS_FONT,
	SS_TEXTBOX,
	SS_MENU,
	SS_TERMINAL,
	SS_MNEMONIC,
	SS_ASSEMBLER,
	SS_SYSTEMS
} wanderlust_subsystems;

typedef enum
{
	SS_F_CONFIGURATION 	= 0x0001,
	SS_F_TASK			= 0x0002,
	SS_F_VIDEO			= 0x0004,
	SS_F_INPUT			= 0x0008,
	SS_F_TRANSITION		= 0x0010,
	SS_F_STATE			= 0x0020,
	SS_F_ENTITY			= 0x0040,
	SS_F_MAP			= 0x0080,
	SS_F_FONT			= 0x0100,
	SS_F_TEXTBOX		= 0x0200,
	SS_F_MENU			= 0x0400,
	SS_F_TERMINAL		= 0x0800,
	SS_F_MNEMONIC		= 0x1000,
	SS_F_ASSEMBLER		= 0x2000,
	SS_F_READY			= 0x3FFF
} wanderlust_subsystems_flag;

typedef struct
{
	u32		flag;
	s32 	iteration;
	s08 *	title;
	s08		success;
	s08		failure;
	nil *	null;		/* for pointer			*/
	s32		zero;		/* for everything else	*/
	s32		fps;

	struct
	{
		s32 x;
		s32 y;
		u32 width;
		u32 height;
		u08 scaling;
		u08 scalingDefault;
		u08 scalingMin;
		u08 scalingMax;
		s32 fullscreen;
		s32 hidden;
		s32 shown;
		s32 opengl;
	} resolution;

	struct
	{
		s32 accelerated;
		s32 presentVSync;
	} rendering;

	struct
	{
		u08 width;
		u08 height;
		u08 rows;
		u08 rowsDynamic2;
		u08 rowsDynamic2Iteration;
		u08 rowsDynamic4;
		u08 rowsDynamic4Iteration;
		u08 columns;
		u08 rate;
		u08 types;
	} tileset;

	struct
	{
		u16 rows;
		u16 columns;
		u16 divisor;
		u08	layer;
		u08 layerTerrain;
		u08 layerEvent;
		u08 offsetRow;
		u08 offsetColumn;

		struct
		{
			u16 water;
			u16 entity;
			u16 open;
		} terrain;

		struct
		{
			u08 walk;
			u08 ride;
			u08 swim;
		} stepping;
	} map;

	struct
	{
		u08 r;
		u08 g;
		u08 b;
		u08 a;
	} drawing;

	struct
	{
		u08 locked;
		u08 open;
	} state;

	struct
	{
		u08 dimension;
		u08 rows;
		u08 columns;
		u08 depth;
	} font;

	struct
	{
		u08 n;
		u08 e;
		u08 s;
		u08 w;
	} direction;

	struct
	{
		u08 up;
		u08 down;
		u08 left;
		u08 right;
		u08 a;
		u08 b;
		u08 x;
		u08 y;
		u08 start;
		u08 back;
	} button;

	struct
	{
		u08 singlelinkedlist;
		u08 doublelinkedlist;
		u08 array;
		u08 stack;
		u08 queue;
	} morph;

	struct {
		u32 width;
		u32 height;
	}transition;

	struct
	{
		struct
		{
			u32 x;
			u32 y;
			u32 rows;
			u32 columns;
			u32 dimension;
		} frame;

		struct{
			u32 rows;
			u32 columns;
		} content;
	} textbox;

	struct
	{
		u32 integer;
		u32 string;
		u32 point;
		u32 rectangle;
		u32 types;
	} property;
} wanderlust_config;

s08 configuration_bootstrap ();

s08 configuration_initialize (lua_State *);

nil configuration_terminate ();

wanderlust_config * configuration_get ();

#endif
