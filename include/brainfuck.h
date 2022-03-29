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
