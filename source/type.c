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
