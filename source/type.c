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

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

char * type_secure_copy_string (const char * text)
{
	char	*	s = 0;
	size_t		l = 0;

	if (! text)
	{
		return 0;
	}

	l = SDL_strlen (text) + 1;

	s = SDL_malloc (l);

	if (! s)
	{
		return 0;
	}

	SDL_strlcpy (s, text, l);

	return s;
}
