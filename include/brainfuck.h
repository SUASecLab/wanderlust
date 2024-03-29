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

#ifndef BRAINFUCK_CONTEXT
#define BRAINFUCK_CONTEXT

typedef enum {
	MACHINE_POINTER_INC 	= 0,
	MACHINE_POINTER_DEC 	= 1,
	MACHINE_MEMORY_INC		= 2,
	MACHINE_MEMORY_DEC		= 3,
	MACHINE_MEMORY_IN		= 4,
	MACHINE_MEMORY_OUT		= 5,
	MACHINE_LOOP_START		= 6,
	MACHINE_LOOP_END		= 7,
	MACHINE_STACK_PUSH		= 8,
	MACHINE_STACK_POP		= 9,
	MACHINE_STACK_PEEK		= 10,
	MACHINE_UNDEFINED		= 11,
	MACHINE_PROGRAM_HALT	= 12,
	MACHINE_PROGRAM_FAIL	= 13,
	MACHINE_PROGRAM_STATUS	= 14
} symbols;

typedef enum {
	F_HALT			= 0x01,
	F_ERROR			= 0x02,
	F_OVERFLOW_P	= 0x10,
	F_OVERFLOW_S	= 0x20,
	F_OVERFLOW_D	= 0x40,
	F_OUT_OF_RANGE	= 0x80
} flags;

typedef struct {
	struct {
		int *p;
		int *s;
		int *d;
	} mem;
	struct {
		unsigned short	p; /* program*/
		unsigned short	s; /* stack */
		unsigned short	d; /* data */
		unsigned char 	f; /* flag*/
	} reg;
} context;

context * 	context_create ();

context *	context_delete (context *);

void		context_analyze (context *, char *, int (*)(int));

void		context_examine (context *);

void 		context_execute (context *);

#endif
