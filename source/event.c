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

#include "type.h"
#include "config.h"
#include "morph.h"
#include "entity.h"
#include "map.h"
#include "event.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * MACRO
 *
 * ***********************************************************************/

#define FN(N) F ## N

/* ************************************************************************
 *
 * TYPEDEFINITIONS
 *
 * ***********************************************************************/

typedef struct
{
	const	u08			id;

			s08	*		callback_l;

			nil			(* callback_c)(nil *);
} wanderlust_event;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

static s08		f_initialized 	= WANDERLUST_FAILURE;

static nil	*	f_events		= 0;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - EVENTS - ENTITY DOOR N
 *
 * ***********************************************************************/

static nil FN (00) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (! event->callback_l)
	{
		return;
	}

	/*
		if (! event->callback_c)
		{
			return;
		}
	*/

	if (map_lua_get_global ((const char*)event->callback_l) == e_configuration.failure)
	{
		return;
	}

	/*event->callback_c (entity);*/

	if (map_lua_push_number (entity->hash) == e_configuration.failure)
	{
		printf ("unable to push number");
	}

	if (map_lua_pcall (1, 1) == e_configuration. failure)
	{
		printf ("unable to call script\n");
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - EVENTS - ENTITY DOOR E
 *
 * ***********************************************************************/

static nil FN (01) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (! event->callback_l)
	{
		return;
	}

	/*
		if (! event->callback_c)
		{
			return;
		}
	*/

	if (map_lua_get_global ((const char*)event->callback_l) == e_configuration.failure)
	{
		return;
	}

	/*event->callback_c (entity);*/

	if (map_lua_push_number (entity->hash) == e_configuration.failure)
	{
		printf ("unable to push number");
	}

	if (map_lua_pcall (1, 1) == e_configuration.failure)
	{
		printf ("unable to call script\n");
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - EVENTS - ENTITY DOOR S
 *
 * ***********************************************************************/

static nil FN (02) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (! event->callback_l)
	{
		return;
	}

	/*
		if (! event->callback_c)
		{
			return;
		}
	*/

	if (map_lua_get_global ((const char*)event->callback_l) == e_configuration.failure)
	{
		return;
	}

	/*event->callback_c (entity);*/

	if (map_lua_push_number (entity->hash) == e_configuration.failure)
	{
		printf ("unable to push number");
	}

	if (map_lua_pcall (1, 1) == e_configuration.failure)
	{
		printf ("unable to call script\n");
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - EVENTS - ENTITY DOOR W
 *
 * ***********************************************************************/

static nil FN (03) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (! event->callback_l)
	{
		return;
	}
/*
	if (! event->callback_c)
	{
		return;
	}
*/
	if (map_lua_get_global ((const char*)event->callback_l) == e_configuration.failure)
	{
		return;
	}

	/*event->callback_c (entity);*/

	if (map_lua_push_number (entity->hash) == e_configuration.failure)
	{
		printf ("unable to push number");
	}

	if (map_lua_pcall (1, 1) == e_configuration.failure)
	{
		printf ("unable to call script\n");
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - EVENTS - ENTITY DOOR
 *
 * ***********************************************************************/

static nil FN (04) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (event->callback_c)
	{
		event->callback_c (entity);
	}
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - EVENTS - ENTITY READ SIGN
 *
 * ***********************************************************************/

static nil FN (05) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (event->callback_l)
	{
		if (map_lua_get_global ((const char*)event->callback_l) == e_configuration.failure)
		{
			return;
		}

		map_lua_pcall (0, 1);
	}
/*
	if (event->callback_c)
	{
		event->callback_c (entity);
	}
*/
}

static nil FN (06) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (07) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (08) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (09) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (0A) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (0B) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (0C) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (0D) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (0E) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (0F) (wanderlust_event * event, wanderlust_entity * entity)
{

}

/* ************************************************************************
 *
 * ENTITY JUMP N
 *
 * ***********************************************************************/

static nil FN (10) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (event->callback_c)
	{
		event->callback_c (entity);
	}
}

/* ************************************************************************
 *
 * ENTITY JUMP E
 *
 * ***********************************************************************/

static nil FN (11) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (event->callback_c)
	{
		event->callback_c (entity);
	}
}

/* ************************************************************************
 *
 * ENTITY JUMP S
 *
 * ***********************************************************************/

static nil FN (12) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (event->callback_c)
	{
		event->callback_c (entity);
	}
}

/* ************************************************************************
 *
 * ENTITY JUMP W
 *
 * ***********************************************************************/

static nil FN (13) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (event->callback_c)
	{
		event->callback_c (entity);
	}
}

/* ************************************************************************
 *
 * ENTITY JUMP
 *
 * ***********************************************************************/

static nil FN (14) (wanderlust_event * event, wanderlust_entity * entity)
{
	if (! event)
	{
		return;
	}

	if (! entity)
	{
		return;
	}

	if (event->callback_c)
	{
		event->callback_c (entity);
	}
}

static nil FN (15) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (16) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (17) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (18) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (19) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (1A) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (1B) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (1C) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (1D) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (1E) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (1F) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (20) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (21) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (22) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (23) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (24) (wanderlust_event * event, wanderlust_entity * entity)
{

}

/* ************************************************************************
 *
 * ENTITY SURF
 *
 * ***********************************************************************/

static nil FN (25) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (26) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (27) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (28) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (29) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (2A) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (2B) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (2C) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (2D) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (2E) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (2F) (wanderlust_event * event, wanderlust_entity * entity)
{

}


static nil FN (30) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (31) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (32) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (33) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (34) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (35) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (36) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (37) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (38) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (39) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (3A) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (3B) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (3C) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (3D) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (3E) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (3F) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (40) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (41) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (42) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (43) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (44) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (45) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (46) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (47) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (48) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (49) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (4A) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (4B) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (4C) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (4D) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (4E) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (4F) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (50) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (51) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (52) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (53) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (54) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (55) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (56) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (57) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (58) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (59) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (5A) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (5B) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (5C) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (5D) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (5E) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (5F) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (60) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (61) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (62) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (63) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (64) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (65) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (66) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (67) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (68) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (69) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (6A) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (6B) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (6C) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (6D) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (6E) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (6F) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (70) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (71) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (72) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (73) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (74) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (75) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (76) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (77) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (78) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (79) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (7A) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (7B) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (7C) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (7D) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (7E) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (7F) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (80) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (81) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (82) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (83) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (84) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (85) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (86) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (87) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (88) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (89) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (8A) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (8B) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (8C) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (8D) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (8E) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (8F) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (90) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (91) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (92) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (93) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (94) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (95) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (96) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (97) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (98) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (99) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (9A) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (9B) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (9C) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (9D) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (9E) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (9F) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (A0) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (A1) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (A2) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (A3) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (A4) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (A5) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (A6) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (A7) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (A8) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (A9) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (AA) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (AB) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (AC) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (AD) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (AE) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (AF) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (B0) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (B1) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (B2) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (B3) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (B4) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (B5) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (B6) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (B7) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (B8) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (B9) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (BA) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (BB) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (BC) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (BD) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (BE) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (BF) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (C0) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (C1) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (C2) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (C3) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (C4) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (C5) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (C6) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (C7) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (C8) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (C9) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (CA) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (CB) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (CC) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (CD) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (CE) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (CF) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (D0) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (D1) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (D2) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (D3) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (D4) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (D5) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (D6) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (D7) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (D8) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (D9) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (DA) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (DB) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (DC) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (DD) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (DE) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (DF) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (E0) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (E1) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (E2) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (E3) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (E4) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (E5) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (E6) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (E7) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (E8) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (E9) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (EA) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (EB) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (EC) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (ED) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (EE) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (EF) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (F0) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (F1) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (F2) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (F3) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (F4) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (F5) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (F6) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (F7) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (F8) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (F9) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (FA) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (FB) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (FC) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (FD) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (FE) (wanderlust_event * event, wanderlust_entity * entity)
{

}

static nil FN (FF) (wanderlust_event * event, wanderlust_entity * entity)
{

}

/* ************************************************************************
 *
 * BANK
 *
 * ***********************************************************************/

static nil (* event [0x100]) (wanderlust_event *, wanderlust_entity *) =
{
	FN (00), FN (01), FN (02), FN (03), FN (04), FN (05), FN (06), FN (07),
	FN (08), FN (09), FN (0A), FN (0B), FN (0C), FN (0D), FN (0E), FN (0F),
	FN (10), FN (11), FN (12), FN (13), FN (14), FN (15), FN (16), FN (17),
	FN (18), FN (19), FN (1A), FN (1B), FN (1C), FN (1D), FN (1E), FN (1F),
	FN (20), FN (21), FN (22), FN (23), FN (24), FN (25), FN (26), FN (27),
	FN (28), FN (29), FN (2A), FN (2B), FN (2C), FN (2D), FN (2E), FN (2F),
	FN (30), FN (31), FN (32), FN (33), FN (34), FN (35), FN (36), FN (37),
	FN (38), FN (39), FN (3A), FN (3B), FN (3C), FN (3D), FN (3E), FN (3F),
	FN (40), FN (41), FN (42), FN (43), FN (44), FN (45), FN (46), FN (47),
	FN (48), FN (49), FN (4A), FN (4B), FN (4C), FN (4D), FN (4E), FN (4F),
	FN (50), FN (51), FN (52), FN (53), FN (54), FN (55), FN (56), FN (57),
	FN (58), FN (59), FN (5A), FN (5B), FN (5C), FN (5D), FN (5E), FN (5F),
	FN (60), FN (61), FN (62), FN (63), FN (64), FN (65), FN (66), FN (67),
	FN (68), FN (69), FN (6A), FN (6B), FN (6C), FN (6D), FN (6E), FN (6F),
	FN (70), FN (71), FN (72), FN (73), FN (74), FN (75), FN (76), FN (77),
	FN (78), FN (79), FN (7A), FN (7B), FN (7C), FN (7D), FN (7E), FN (7F),
	FN (80), FN (81), FN (82), FN (83), FN (84), FN (85), FN (86), FN (87),
	FN (88), FN (89), FN (8A), FN (8B), FN (8C), FN (8D), FN (8E), FN (8F),
	FN (90), FN (91), FN (92), FN (93), FN (94), FN (95), FN (96), FN (97),
	FN (98), FN (99), FN (9A), FN (9B), FN (9C), FN (9D), FN (9E), FN (9F),
	FN (A0), FN (A1), FN (A2), FN (A3), FN (A4), FN (A5), FN (A6), FN (A7),
	FN (A8), FN (A9), FN (AA), FN (AB), FN (AC), FN (AD), FN (AE), FN (AF),
	FN (B0), FN (B1), FN (B2), FN (B3), FN (B4), FN (B5), FN (B6), FN (B7),
	FN (B8), FN (B9), FN (BA), FN (BB), FN (BC), FN (BD), FN (BE), FN (BF),
	FN (C0), FN (C1), FN (C2), FN (C3), FN (C4), FN (C5), FN (C6), FN (C7),
	FN (C8), FN (C9), FN (CA), FN (CB), FN (CC), FN (CD), FN (CE), FN (CF),
	FN (D0), FN (D1), FN (D2), FN (D3), FN (D4), FN (D5), FN (D6), FN (D7),
	FN (D8), FN (D9), FN (DA), FN (DB), FN (DC), FN (DD), FN (DE), FN (DF),
	FN (E0), FN (E1), FN (E2), FN (E3), FN (E4), FN (E5), FN (E6), FN (E7),
	FN (E8), FN (E9), FN (EA), FN (EB), FN (EC), FN (ED), FN (EE), FN (EF),
	FN (F0), FN (F1), FN (F2), FN (F3), FN (F4), FN (F5), FN (F6), FN (F7),
	FN (F8), FN (F9), FN (FA), FN (FB), FN (FC), FN (FD), FN (FE), FN (FF)
};

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - HELPING HANDS
 *
 * ***********************************************************************/

static wanderlust_event * create_wanderlust_event (u08 id)
{
	wanderlust_event * e = SDL_malloc (sizeof (wanderlust_event));

	if (! e)
	{
		goto FAILURE;
	}

	e->callback_l = 0;

	e->callback_c	= 0;

	SET_U08_CONST (e->id, id);

	return e;

	FAILURE:

		return e_configuration.null;
}

static nil delete_wanderlust_event (nil * item)
{
	wanderlust_event * e = item;

	if (! e)
	{
		return;
	}

	if (e->callback_l)
	{
		SDL_free (e->callback_l);
	}

	SDL_free (e);
}

static u08 compare_wanderlust_event_by_id (nil * item, nil * value)
{
	if (! item)
	{
		goto FAILURE;
	}

	if (! value)
	{
		goto FAILURE;
	}

	return ((wanderlust_event *) item)->id == * ((u08 *) value);

	FAILURE:

		return e_configuration.zero;
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 event_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized = e_configuration.success;

	if (! L)
	{
		goto FAILURE;
	}

	lua_register (L, WL_ATTACH_EVENT, 	event_attach_from_lua);
	lua_register (L, WL_DEATTACH_EVENT,	event_deattach_from_lua);

	if (! (f_events = mcreate (e_configuration.morph.singlelinkedlist)))
	{
		goto FAILURE;
	}

	return e_configuration.success;

	FAILURE:

		event_terminate ();

		return e_configuration.failure;
}

nil event_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	f_events		= mdelete (f_events, delete_wanderlust_event);
	f_initialized 	= e_configuration.failure;
}

nil event_trigger (u08 id, wanderlust_entity * entity)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	event [id]
	(
		mfind (f_events, &id, compare_wanderlust_event_by_id, 0), entity
	);
}

nil event_attach (u08 id, nil (* callback_c)(nil *))
{
	wanderlust_event * e = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	e = mfind (f_events, &id, compare_wanderlust_event_by_id, 0);

	if (! e)
	{
		if (! (e = create_wanderlust_event (id)))
		{
			return;
		}

		madd (f_events, e);
	}

	e->callback_c = callback_c;
}

nil event_deattach (u08 id)
{
	wanderlust_event * e = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! (e = mfind (f_events, &id, compare_wanderlust_event_by_id, 0)))
	{
		return;
	}

	e->callback_c = 0;
}

#define EVENT_ATTACH_FROM_LUA_ARG_COUNT		2

#define EVENT_ATTACH_FROM_LUA_ARG_POS_ID	1

#define EVENT_ATTACH_FROM_LUA_ARG_POS_FUN	2

s32 event_attach_from_lua (lua_State * L)
{
	u08 					id			= 0;
	s08					*	callback	= 0;
	wanderlust_event	* 	event		= 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < EVENT_ATTACH_FROM_LUA_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, EVENT_ATTACH_FROM_LUA_ARG_POS_ID))
	{
		goto FAILURE;
	}

	if (! lua_isstring (L, EVENT_ATTACH_FROM_LUA_ARG_POS_FUN))
	{
		goto FAILURE;
	}

	callback = (s08*)type_secure_copy_string
	(
		lua_tostring (L, EVENT_ATTACH_FROM_LUA_ARG_POS_FUN)
	);

	if (! callback)
	{
		goto FAILURE;
	}

	id = (u08) lua_tointeger
	(
		L, EVENT_ATTACH_FROM_LUA_ARG_POS_ID
	);

	event = mfind
	(
		f_events, &id, compare_wanderlust_event_by_id, 0
	);

	if (! event)
	{
		if (! (event = create_wanderlust_event (id)))
		{
			goto FAILURE;
		}

		madd (f_events, event);
	}
	else
	{
		if (event->callback_l)
		{
			SDL_free (event->callback_l);
		}
	}

	event->callback_l 	= callback;
	callback			= e_configuration.null;

	lua_settop (L, 0);

	return e_configuration.failure;

	FAILURE:

		if (callback)
		{
			SDL_free (callback);
		}

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define EVENT_DEATTACH_FROM_LUA_ARG_COUNT	1

#define EVENT_DEATTACH_FROM_LUA_ARG_POS_ID	1

s32 event_deattach_from_lua (lua_State * L)
{
	u08 					id		= 0;
	wanderlust_event	* 	event	= 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < EVENT_DEATTACH_FROM_LUA_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! (lua_isnumber (L, EVENT_DEATTACH_FROM_LUA_ARG_POS_ID)))
	{
		goto FAILURE;
	}

	id		= (u08) lua_tointeger (L, EVENT_DEATTACH_FROM_LUA_ARG_POS_ID);
	event	= mfind (f_events, &id, compare_wanderlust_event_by_id, 0);

	if (! event)
	{
		goto FAILURE;
	}

	if (! event->callback_l)
	{
		goto SUCCESS;
	}

	SDL_free (event->callback_l);

	event->callback_l = 0;

	SUCCESS:

		lua_settop (L, 0);

		return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}
