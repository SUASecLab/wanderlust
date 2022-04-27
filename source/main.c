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
#include "engine.h"
#include "task.h"

#include "assembler.h"
#include "core.h"

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ************************************************************************/

static nil shutdown_procedure ()
{
	engine_terminate ();

	if (task_pending ())
	{
		task_rundown (0);
	}

	task_terminate ();

	SDL_Quit ();
}

/* ************************************************************************
 *
 * MAIN
 *
 * ************************************************************************/

s32 main (int argc, char ** argv)
{
	if (argc < 2)
	{
		return WANDERLUST_FAILURE;
	}

	if (SDL_Init (SDL_INIT_EVERYTHING) < 0)
	{
		shutdown_procedure ();

		return WANDERLUST_FAILURE;
	}

	if (engine_bootstrap (argv [1]) == WANDERLUST_FAILURE)
	{
		shutdown_procedure ();

		return WANDERLUST_FAILURE;
	}

	if (engine_initialize (argv [1]) == WANDERLUST_FAILURE)
	{
		shutdown_procedure ();

		return WANDERLUST_FAILURE;
	}

/*
	assembler_set_byte (0x0000, 0x21);
	assembler_set_byte (0x0001, 0x34);
	assembler_set_byte (0x0002, 0x56);
	assembler_set_byte (0x0003, 0x78);
	assembler_set_byte (0x0004, 0x04);
	assembler_set_byte (0x0005, 0x06);
	assembler_set_byte (0x0006, 0x78);
*/
	core_init ();
	
	if (engine_execute () == WANDERLUST_FAILURE)
	{
		shutdown_procedure ();

		return WANDERLUST_FAILURE;
	}

	core_quit ();
	
	shutdown_procedure ();

	return WANDERLUST_SUCCESS;
}
