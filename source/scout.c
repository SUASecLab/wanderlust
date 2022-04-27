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
#include "scout.h"
#include "entity.h"
#include "morph.h"
#include "map.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - VARIANT A
 *
 * ***********************************************************************/

static u16 scout_next_n (wanderlust_scout * s)
{
	u16 terrain = 0;

	if (s -> target.r < s -> source.r)
	{
		if (s -> target.c < s -> source.c)
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}
		}
		else
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}
		}
	}
	else
	{
		if (s -> target.c < s -> source.c)
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}


			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}
		}
		else
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}
		}
	}

	return s -> failure;
}

static u16 scout_next_e (wanderlust_scout * s)
{
	u16 terrain = 0;

	if (s -> target.r < s -> source.r)
	{
		if (s -> target.c < s -> source.c)
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}
		}
		else
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}
		}
	}
	else
	{
		if (s -> target.c < s -> source.c)
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}
		}
		else
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}


			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}
		}
	}

	return s -> failure;
}

static u16 scout_next_s (wanderlust_scout * s)
{
	u16 terrain = 0;

	if (s -> target.r < s -> source.r)
	{
		if (s -> target.c < s -> source.c)
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}
		}
		else
		{

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}


			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}
		}
	}
	else
	{
		if (s -> source.c < s -> target.c)
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}
		}
		else
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}


			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}
		}
	}

	return s -> failure;
}

static u16 scout_next_w (wanderlust_scout * s)
{
	u16 terrain = 0;

	if (s -> target.r < s -> source.r)
	{
		if (s -> target.c < s -> source.c)
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}
		}
		else
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}
		}
	}
	else
	{
		if (s -> target.c < s -> source.c)
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}
		}
		else
		{
			terrain = map_get_terrain (s -> source.r, s -> source.c, 1, 0);

			if (terrain == s -> terrain)
			{
				return s -> s;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, -1, 0);

			if (terrain == s -> terrain)
			{
				return s -> n;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, -1);

			if (terrain == s -> terrain)
			{
				return s -> w;
			}

			terrain = map_get_terrain (s -> source.r, s -> source.c, 0, 1);

			if (terrain == s -> terrain)
			{
				return s -> e;
			}
		}
	}

	return s -> failure;
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - VARIANT B
 *
 * ************************************************************************/

typedef struct
{
	u08 r;
	u08 c;
	u32 counter;
} vb_cell;

static vb_cell * create_vb_cell (u08 r, u08 c, u32 counter)
{
	vb_cell * cell = SDL_malloc (sizeof (vb_cell));

	if (! cell)
	{
		return 0;
	}

	cell -> r 		= r;
	cell -> c		= c;
	cell -> counter	= counter;

	return cell;
}

static u08 compare_vb_cell (nil * item, nil * value)
{
	vb_cell * u = item;
	vb_cell * v = value;

	if ((! u) || (! v))
	{
		return 0;
	}

	return (u -> r == v -> r) && (u -> c == v -> c);
}

static u08 compare_vb_cell_complete (nil * item, nil * value)
{
	vb_cell * u = item;
	vb_cell * v = value;

	if ((! u) || (! v))
	{
		return 0;
	}

	return (u -> r == v -> r) && (u -> c == v -> c) && (v -> counter == u -> counter);
}


static void scout_variant_b_analyze
(
	nil * list, u08 r, u08 c, u32 counter, wanderlust_scout * s
) {
	vb_cell	* 	p_cell_a = 0;
	vb_cell *	p_cell_b = 0;
	vb_cell		v_cell_a = {0, 0, 0};
	
	if (s -> terrain != map_get_terrain (r, c, 0, 0))
	{
		return;
	}

	v_cell_a.r 			= r;
	v_cell_a.c 			= c;
	v_cell_a.counter	= counter;
	p_cell_a 			= create_vb_cell (r, c, counter);

	if (! p_cell_a)
	{
		return;
	}

	if ((p_cell_a -> r == s -> source.r) && (p_cell_a -> c == s -> source.c))
	{
		madd (list, p_cell_a);

		return;
	}

	p_cell_b = mfind (list, & v_cell_a, compare_vb_cell, 0);

	if (p_cell_b)
	{
		if (p_cell_b -> counter > counter)
		{
			p_cell_b -> counter = counter;
		}

		SDL_free (p_cell_a);

		return;
	}
	else
	{
		madd (list, p_cell_a);
	}

	if (p_cell_a -> r < s -> source.r)
	{
		scout_variant_b_analyze (list, r + 1, c, counter + 1, s);

		if (p_cell_a -> c < s -> source.c)
		{
			scout_variant_b_analyze (list, r, c + 1, counter + 1, s);
			scout_variant_b_analyze (list, r, c - 1, counter + 1, s);
		}
		else
		{
			scout_variant_b_analyze (list, r, c - 1, counter + 1, s);
			scout_variant_b_analyze (list, r, c + 1, counter + 1, s);
		}

		scout_variant_b_analyze (list, r - 1, c, counter + 1, s);
	}
	else if (p_cell_a -> r == s -> source.r)
	{
		if (p_cell_a -> c <= s -> source.c)
		{
			scout_variant_b_analyze (list, r, c + 1, counter + 1, s);
			scout_variant_b_analyze (list, r, c - 1, counter + 1, s);
		}
		else
		{
			scout_variant_b_analyze (list, r, c - 1, counter + 1, s);
			scout_variant_b_analyze (list, r, c + 1, counter + 1, s);
		}
	}
	else
	{
		scout_variant_b_analyze (list, r - 1, c, counter + 1, s);

		if (p_cell_a -> c < s -> source.c)
		{
			scout_variant_b_analyze (list, r, c + 1, counter + 1, s);
			scout_variant_b_analyze (list, r, c - 1, counter + 1, s);
		}
		else
		{
			scout_variant_b_analyze (list, r, c - 1, counter + 1, s);
			scout_variant_b_analyze (list, r, c + 1, counter + 1, s);
		}

		scout_variant_b_analyze (list, r + 1, c, counter + 1, s);
	}
}

static nil batch_scout_find_source_counter (nil * item, nil * value)
{
	vb_cell * a = item;
	vb_cell * b = value;

	if ((! a) || (! b))
	{
		return;
	}

	if ((a -> r == b -> r) && (b -> c == a -> c))
	{
		if (b -> counter == 0)
		{
			b -> counter = a -> counter;
		}

		if (a -> counter < b -> counter)
		{
			b -> counter = a -> counter;
		}
	}
}

/*
static nil batch_print_cells (nil * item, nil * value)
{
	vb_cell * a = item;

	if (! a)
	{
		return;
	}

	printf ("cell: (%i,%i,%i)\n", a -> r, a -> c, a -> counter);
}

static nil batch_print_directions (nil * item, nil * value)
{
	u08 * a = item;

	if (! a)
	{
		printf ("nope\n");

		return;
	}

	printf ("direction: %i\n", * a);
}
*/

static nil * scout_variant_b_create_direction_list (nil * vb_cells, vb_cell * start, wanderlust_scout * s)
{
	vb_cell 	next 			= {0, 0, 0};
	vb_cell	*	temp 			= 0;
	u08		*	direction		= 0;
	nil		*	instructions	= 0;
	s32			counter			= 0;

	u08			r				= 0;
	u08			c				= 0;
	u08			prev			= 0xFF;

	if (! vb_cells)
	{
		return 0;
	}

	if (! start)
	{
		return 0;
	}

	if (! s)
	{
		return 0;
	}

	if (! (instructions = mcreate (M_QUEUE)))
	{
		return 0;
	}

	counter = start -> counter;
	r		= start -> r;
	c		= start -> c;

	prev 	= s -> direction;

	for (; counter >= 0; counter --)
	{
		next.counter	= counter;
		next.r 			= r - 1;
		next.c 			= c;

		if ((temp = mfind (vb_cells, &next, compare_vb_cell_complete, 0)))
		{
			if ((direction = SDL_malloc (sizeof (u08))))
			{
				if (prev != s -> n)
				{
					counter += 1;
				}
				else
				{
					r -= 1;
				}

				* direction = prev = s -> n;

				menqueue (instructions, direction);

				continue;
			}
			else
			{
				break;
			}
		}

		next.r = r;
		next.c = c + 1;

		if ((temp = mfind (vb_cells, & next, compare_vb_cell_complete, 0)))
		{
			if ((direction = SDL_malloc (sizeof (u08))))
			{
				if (prev != s -> e)
				{
					counter += 1;
				}
				else
				{
					c += 1;
				}

				* direction = prev = s -> e;

				menqueue (instructions, direction);

				continue;
			}
			else
			{
				break;
			}
		}

		next.r = r + 1;
		next.c = c;

		if ((temp = mfind (vb_cells, &next, compare_vb_cell_complete, 0)))
		{
			if ((direction = SDL_malloc (sizeof (u08))))
			{
				if (prev != s -> s)
				{
					counter += 1;
				}
				else
				{
					r += 1;
				}

				* direction = prev = s -> s;

				menqueue (instructions, direction);

				continue;
			}
			else
			{
				break;
			}
		}

		next.r = r;
		next.c = c - 1;

		if ((temp = mfind (vb_cells, &next, compare_vb_cell_complete, 0)))
		{
			if ((direction = SDL_malloc (sizeof (u08))))
			{
				if (prev != s -> w)
				{
					counter += 1;
				}
				else
				{
					c -= 1;
				}

				* direction = prev = s -> w;

				menqueue (instructions, direction);

				continue;
			}
			else
			{
				break;
			}
		}
	}

	return instructions;
}

static nil * scout_variant_b (wanderlust_scout * s)
{
	nil 	* 	vb_cells 	= 0;
	nil 	*	directions	= 0;
	vb_cell		start		= {0,0,0};

	if (! s)
	{
		return 0;
	}

	vb_cells = mcreate (M_SINGLE_LINKED_LIST);

	if (! vb_cells)
	{
		return 0;
	}

	scout_variant_b_analyze (vb_cells, s -> target.r, s -> target.c, 0, s);

	if (! msize (vb_cells))
	{
		vb_cells = mdelete (vb_cells, SDL_free);

		return 0;
	}

	start.c = s -> source.c;
	start.r = s -> source.r;

	mbatch (vb_cells, batch_scout_find_source_counter, & start);

	/*mbatch (vb_cells, batch_print_cells, 0);*/

	directions 	= scout_variant_b_create_direction_list (vb_cells, &start, s);
	vb_cells	= mdelete (vb_cells, SDL_free);

	if (! directions)
	{
		return 0;
	}

	/*
		printf ("directions: 	%i\n", msize (directions));

		mbatch (directions, batch_print_directions, 0);
	*/

	return directions;
}

/* ************************************************************************
 *
 * FILE SCOPE_FUNCTIONS - VARIANT C  - A STAR
 *
 * ***********************************************************************/

typedef struct
{
			u08 		row;
			u08 		column;

	struct	cnode	*	predecessor;

			u16 		h;	/* DISTANCE TO TARGET 	*/
			u16 		g;	/* COST OF MOVEMENT		*/
			u16 		f;	/* g + h */
} cnode;


static cnode * scout_variant_c_build_cnode
(
	u08 r, u08 c, nil * predecessor, u16 h, u16 g
)
{
	cnode * node = SDL_malloc (sizeof (cnode));

	if (! node)
	{
		return 0;
	}

	node -> row 		= r;
	node -> column		= c;
	node -> predecessor	= predecessor;
	node -> g			= g;
	node -> h			= h;
	node -> f			= g + h;

	return node;
}

static u08 compare_nodes_by_row_and_column (nil * item, nil * value)
{
	cnode * u = item, * v = value;

	if ((! u) || (! v))
	{
		return 0;
	}

	return (u -> row == v -> row) && (v -> column == u -> column);
}

static u08 compare_node_by_f (nil * item, nil * value)
{
	cnode * u = item, * v = value;

	if ((! u) || (! v))
	{
		return 0;
	}

	return (u -> f) > (v -> f);
}

#define SCOUT_DIRECTION_COUNT 		4

#define SCOUT_DIRECTION_ROW			0

#define SCOUT_DIRECTION_COLUMN		1

#define SCOUT_DIRECTION_DIMENSIONS	2

#define EUCLIDEAN_DISTANCE(NODE, SCOUT)										\
	sqrt																	\
	(																		\
		pow ((NODE.row - SCOUT -> target.r), 2) + 							\
		pow ((NODE.column - SCOUT -> target.c), 2)							\
	)

#define EUCLIDEAN_DISTANCE_P(R0,R1,C0,C1)									\
	sqrt																	\
	(																		\
		pow ((R0 - R1), 2) + pow ((C0 - C1), 2)								\
	)

static int f_direction [SCOUT_DIRECTION_COUNT][SCOUT_DIRECTION_DIMENSIONS] =
{
	{-1, 0}, {0, 1}, {1, 0}, {0, -1}
};

static nil scout_variant_c_expand_node
(
	nil * parent, nil * open_list, nil * closed_list, wanderlust_scout * s
)
{
	int 		d		= 0;
	int 		r		= SCOUT_DIRECTION_ROW;
	int 		c		= SCOUT_DIRECTION_COLUMN;
	cnode 		n		= {0, 0, 0, 0, 0, 0};
	cnode	*	p		= parent;
	cnode 	*	node	= 0;

	for (d = 0; d < SCOUT_DIRECTION_COUNT; d ++)
	{
		if (map_get_terrain (p->row, p->column, f_direction [d][r], f_direction [d][c]) != s->terrain)
		{
			continue;
		}

		n.row		= p->row 	+ f_direction [d] [r];
		n.column	= p->column	+ f_direction [d] [c];

		if (mfind (closed_list, & n, compare_nodes_by_row_and_column, 0))
		{
			continue;
		}

		if ((node = mfind (open_list, & n, compare_nodes_by_row_and_column, 0)))
		{
			if (node -> g > p->g + 1)
			{
				node->g 			= p->g + 1;
				node->f 			= p->g + 1 + node->h;
				node->predecessor	= parent;
			}

			continue;
		}

		if (! (node = scout_variant_c_build_cnode (n.row, n.column, p, EUCLIDEAN_DISTANCE (n, s), p->g + 1)))
		{
			continue;
		}

		madd (open_list, node);

		if (msize (open_list) > 1)
		{
			msort (open_list, compare_node_by_f);
		}
	}
}

static nil * scout_variant_c (wanderlust_scout * s, nil * closed_list)
{
	nil 	* open_list = 0;
	cnode	* prev		= 0;
	cnode	* next		= 0;

	if (! closed_list)
	{
		return 0;
	}

	if (! (open_list = mcreate (e_configuration.morph.doublelinkedlist)))
	{
		return 0;
	}

	prev = scout_variant_c_build_cnode
	(
		s -> source.r, s -> source.c, 0, EUCLIDEAN_DISTANCE_P(s -> target.r, s -> source.r,s -> target.c, s -> source.c), 0
	);

	if (! prev)
	{
		open_list = mdelete (open_list, free);
	}

	madd (open_list, prev);

	for (; msize (open_list);)
	{
		if (! (next = mpeek (open_list, 0)))
		{
			break;
		}

		mremove (open_list, next, compare_nodes_by_row_and_column, 0);

		if ((next -> row == s -> target.r) && (next -> column == s -> target.c))
		{
			break;
		}

		madd (closed_list, next);

		scout_variant_c_expand_node (next, open_list, closed_list, s);
	}

	open_list = mdelete (open_list, free);

	return closed_list;
}

/* ************************************************************************
 *
 * BANK
 *
 * ***********************************************************************/

static u16 (* f_next [WANDERLUST_DIRECTIONS]) (wanderlust_scout *) =
{
	scout_next_n, scout_next_e, scout_next_s, scout_next_w
};

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

u16 scout_next_a (wanderlust_scout * s)
{
	if (! s)
	{
		return 0xFF;
	}

	if (s -> direction < WANDERLUST_DIRECTIONS)
	{
		return f_next [s -> direction] (s);
	}

	return s -> failure;
}

nil * scout_next_b (wanderlust_scout * s)
{
	if (! s)
	{
		return 0;
	}
/*
	if (s -> direction >= WANDERLUST_DIRECTIONS)
	{
		return 0;
	}
*/
	return scout_variant_b (s);
}

nil	* scout_next_c (wanderlust_scout * s)
{
	if (! s)
	{
		goto FAILURE;
	}

	return scout_variant_c (s, mcreate (e_configuration.morph.singlelinkedlist));

	FAILURE:

		return e_configuration.null;
}
