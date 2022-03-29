#include "type.h"
#include "config.h"
#include "font.h"
#include "frame.h"
#include "config.h"

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ***********************************************************************/

static nil frame_unfold (wanderlust_frame * f, u16 y, u16 x, u16 r, u16 c, u16 	d)
{
	if (! f)
	{
		return;
	}

	f -> dimension 	= d;
	f -> columns 	= c;
	f -> rows		= r;

	f -> ulc.dst.x	= x;
	f -> ulc.dst.y	= y;
	f -> ulc.dst.w	= d;
	f -> ulc.dst.h	= d;

	f -> ulc.src.x	= 0;
	f -> ulc.src.y	= 0;
	f -> ulc.src.w	= d;
	f -> ulc.src.h	= d;

	f -> llc.dst.x	= x;
	f -> llc.dst.y	= y + (d * (r - 1));
	f -> llc.dst.w	= d;
	f -> llc.dst.h	= d;

	f -> llc.src.x	= 0;
	f -> llc.src.y	= d * 2;
	f -> llc.src.w	= d;
	f -> llc.src.h	= d;

	f -> urc.dst.x	= x + (d * (c - 1));
	f -> urc.dst.y	= y;
	f -> urc.dst.w	= d;
	f -> urc.dst.h	= d;

	f -> urc.src.x	= d * 2;
	f -> urc.src.y	= 0;
	f -> urc.src.w	= d;
	f -> urc.src.h	= d;

	f -> lrc.dst.x	= x + (d * (c - 1));
	f -> lrc.dst.y	= y + (d * (r - 1));
	f -> lrc.dst.w	= d;
	f -> lrc.dst.h	= d;

	f -> lrc.src.x	= d * 2;
	f -> lrc.src.y	= d * 2;
	f -> lrc.src.w	= d;
	f -> lrc.src.h	= d;

	f -> tfb.dst.x	= x + d;
	f -> tfb.dst.y	= y;
	f -> tfb.dst.w	= x + (d * (c - 1));
	f -> tfb.dst.h	= d;

	f -> tfb.src.x	= d;
	f -> tfb.src.y	= 0;
	f -> tfb.src.w	= d;
	f -> tfb.src.h	= d;

	f -> rfb.dst.x	= x + (d * (c - 1));
	f -> rfb.dst.y	= y + d;
	f -> rfb.dst.w	= d;
	f -> rfb.dst.h	= y + (d * (r - 1));

	f -> rfb.src.x	= d * 2;
	f -> rfb.src.y	= d;
	f -> rfb.src.w	= d;
	f -> rfb.src.h	= d;

	f -> bfb.dst.x	= x + d;
	f -> bfb.dst.y	= y + (d * (r - 1));
	f -> bfb.dst.w	= x + (d * (c - 1));
	f -> bfb.dst.h	= d;

	f -> bfb.src.x	= d;
	f -> bfb.src.y	= d * 2;
	f -> bfb.src.w	= d;
	f -> bfb.src.h	= d;

	f -> lfb.dst.x	= x;
	f -> lfb.dst.y	= y + d;
	f -> lfb.dst.w	= d;
	f -> lfb.dst.h	= y + (d * (r - 1));

	f -> lfb.src.x	= 0;
	f -> lfb.src.y	= d;
	f -> lfb.src.w	= d;
	f -> lfb.src.h	= d;

	f -> fda.dst.x	= x + d;
	f -> fda.dst.y	= y + d;
	f -> fda.dst.w	= x + (d * (c - 1));
	f -> fda.dst.h	= y + (d * (r - 1));

	f -> fda.src.x	= d;
	f -> fda.src.y	= d;
	f -> fda.src.w	= d;
	f -> fda.src.h	= d;
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

wanderlust_frame * frame_create_wanderlust_frame
(
	u16				x,
	u16				y,
	u16				r,
	u16				c,
	u16 			d,
	SDL_Texture *	texture
) {
	wanderlust_frame * f = 0;

	if (! texture)
	{
		goto FAILURE;
	}

	if (! (f = SDL_malloc (sizeof (wanderlust_frame))))
	{
		goto FAILURE;
	}

	f -> texture = texture;

	frame_unfold (f, y, x, r, c, d);

	return f;

	FAILURE:

		return 0;
}

nil frame_delete_wanderlust_frame (wanderlust_frame * f)
{
	if (! f)
	{
		return;
	}

	SDL_free (f);
}

nil frame_resize_wanderlust_frame (wanderlust_frame * f, u16 y, u16 x, u16 r, u16 c)
{
	if (! f)
	{
		return;
	}

	frame_unfold (f, y, x, r, c, f -> dimension);
}
