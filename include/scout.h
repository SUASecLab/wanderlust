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

#ifndef WANDERLUST_SCOUT
#define WANDERLUST_SCOUT

/* ************************************************************************
 *
 * TYPE DEFINITIONS
 *
 * ***********************************************************************/

typedef struct
{
	u08 direction;

	u08 n;
	u08 e;
	u08 s;
	u08 w;

	struct
	{
		u08 r;
		u08 c;
	} source;

	struct
	{
		u08 r;
		u08 c;
	} target;

	u16 terrain;

	u16 failure;

} wanderlust_scout;

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

u16		scout_next_a (wanderlust_scout *);

nil	* 	scout_next_b (wanderlust_scout *);

nil	* 	scout_next_c (wanderlust_scout *);

#endif
