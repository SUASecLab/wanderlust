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
