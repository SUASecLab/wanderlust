#include "type.h"
#include "config.h"
#include "mnemonic.h"
#include "assembler.h"

/* ************************************************************************
 *
 * TYPEDEFINITIONS
 *
 * ************************************************************************/

typedef enum {
	ARG_REGISTER,
	ARG_REGISTER_PAIR,
	ARG_CONSTANT,
	ARG_BYTE,
	ARG_WORD,
	ARG_NOTHING
} argument_t;

typedef struct {
	argument_t	type;
	u16			data;		
} parameter_t;

typedef struct {
	u08 		base;
	parameter_t target;
	parameter_t source;
} mnemonic_t;

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

static s08 			f_initialized 	= 0;

static mnemonic_t	f_mnemonic 		= {0, {ARG_NOTHING, 0}, {ARG_NOTHING, 0}};

/* ************************************************************************
 *
 * PROTOTYPES
 *
 * ***********************************************************************/

static s32 mnemonic_generate ();

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 mnemonic_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success) {
		goto SUCCESS;
	}

	if (0) {
		goto FAILURE;
	}

	memset (&f_mnemonic, 0, sizeof (mnemonic_t));

	f_initialized = e_configuration.success;

	SUCCESS:

		return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

nil mnemonic_terminate ()
{
	if (f_initialized == e_configuration.failure) {
		return;
	}

	memset (&f_mnemonic, 0, sizeof (mnemonic_t));

	f_initialized = e_configuration.failure;
}

s32 mnemonic_reset (lua_State* L)
{
	memset (&f_mnemonic, 0, sizeof (mnemonic_t));

	return e_configuration.success;
}

#define MNEMONIC_SET_GENERAL_ARG_COUNT	1
#define MNEMONIC_SET_GENERAL_ARG_TYPE	1

s32 mnemonic_set_base (lua_State* L)
{
	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}

	f_mnemonic.base = (u08) lua_tonumber (
		L, MNEMONIC_SET_GENERAL_ARG_TYPE
	);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_target_register (lua_State* L)
{
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}

	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);

	if (data > 0x7) {
		goto FAILURE;
	}

	f_mnemonic.target.type = ARG_REGISTER;
	f_mnemonic.target.data = data * 0x08;

/*
	printf ("dst %02X\n", f_mnemonic.target.data);
*/

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_target_register_pair (lua_State* L)
{
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}

	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);
	
	if (data > 0x7) {
		goto FAILURE;
	}

	f_mnemonic.target.type = ARG_REGISTER_PAIR;
	f_mnemonic.target.data = data * 0x10;

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_source_register (lua_State* L)
{
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}

	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);

	f_mnemonic.source.type = ARG_REGISTER;
	f_mnemonic.source.data = data;

/*
	printf ("src %02X\n", f_mnemonic.source.data);
*/

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_source_register_pair (lua_State* L)
{
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}

	if (data > 0x7) {
		goto FAILURE;
	}

	f_mnemonic.target.type = ARG_REGISTER_PAIR;
	f_mnemonic.target.data = data * 0x10;

	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);
	
	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_target_constant (lua_State* L)
{
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}

	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);

	if (data > 0x7) {
		goto FAILURE;
	}

	f_mnemonic.target.type = ARG_CONSTANT;
	f_mnemonic.target.data = data * 0x08;
	
	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_source_byte_na (lua_State* L)
{	
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}

	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);

	f_mnemonic.source.type = ARG_BYTE;
	f_mnemonic.source.data = f_mnemonic.source.data & 0xF0;
	f_mnemonic.source.data = f_mnemonic.source.data + ((data & 0xF) << 4);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_source_byte_nb (lua_State* L)
{
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}

	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);
	
	f_mnemonic.source.type = ARG_BYTE;
	f_mnemonic.source.data = f_mnemonic.source.data & 0xF0;
	f_mnemonic.source.data = f_mnemonic.source.data + (data & 0xF);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_source_word_na (lua_State* L)
{
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}
	
	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);
	
	f_mnemonic.source.type = ARG_WORD;
	f_mnemonic.source.data = f_mnemonic.source.data & 0x0FFF;
	f_mnemonic.source.data = f_mnemonic.source.data + ((data & 0xF) << 0xC);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_source_word_nb (lua_State* L)
{
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}
	
	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);
	
	f_mnemonic.source.type = ARG_WORD;
	f_mnemonic.source.data = f_mnemonic.source.data & 0xF0FF;
	f_mnemonic.source.data = f_mnemonic.source.data + ((data & 0xF) << 0x8);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_source_word_nc (lua_State* L)
{
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}
	
	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);
	
	f_mnemonic.source.type = ARG_WORD;
	f_mnemonic.source.data = f_mnemonic.source.data & 0xFF0F;
	f_mnemonic.source.data = f_mnemonic.source.data + ((data & 0xF) << 0x4);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_source_word_nd (lua_State* L)
{
	u08 data = 0;

	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}
	
	data = lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE);
	
	f_mnemonic.source.type = ARG_WORD;
	f_mnemonic.source.data = f_mnemonic.source.data & 0xFFF0;
	f_mnemonic.source.data = f_mnemonic.source.data + (data & 0xF);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_source_byte (lua_State* L)
{
	if (lua_gettop (L) < MNEMONIC_SET_GENERAL_ARG_COUNT)
	{
		goto FAILURE;
	}
	
	if (! lua_isnumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE))
	{
		goto FAILURE;
	}

	assembler_set_byte_increment (
		0, 
		(u08)lua_tonumber (L, MNEMONIC_SET_GENERAL_ARG_TYPE)
	);

	return e_configuration.success;

	FAILURE:

		return e_configuration.failure;
}

s32 mnemonic_set_apply (lua_State* L)
{

	if (mnemonic_generate () == e_configuration.failure) {
		goto FAILURE;
	}

	return mnemonic_reset (L);

	FAILURE:

		return e_configuration.failure;
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS
 *
 * ***********************************************************************/

#define F(SUFFIX) GEN_ ## SUFFIX

static nil F (__) () 
{
	assembler_set_byte (0, f_mnemonic.base);
	assembler_continue (0);
}

static nil F (R_) () 
{
	assembler_set_byte (0, f_mnemonic.base + f_mnemonic.source.data);
	assembler_continue (0);
}

static nil F (RB) ()
{
	assembler_set_byte (0, f_mnemonic.base + f_mnemonic.target.data);
	assembler_set_byte (1, f_mnemonic.source.data);
	assembler_continue (0);
}

static nil F (RR) ()
{
	assembler_set_byte (0, f_mnemonic.base + f_mnemonic.target.data + f_mnemonic.source.data);
	assembler_continue (0);
}

static nil F (P_) ()
{
	assembler_set_byte (0, f_mnemonic.base + f_mnemonic.target.data);
	assembler_continue (0);
}

static nil F (PW) ()
{
	assembler_set_byte (0, f_mnemonic.base + f_mnemonic.target.data);
	assembler_set_byte (1, f_mnemonic.source.data & 0xFF);
	assembler_set_byte (2, f_mnemonic.source.data >> 0x8);
	assembler_continue (0);
}

static nil F (C_) ()
{
	assembler_set_byte (0, f_mnemonic.base + f_mnemonic.target.data);

	assembler_continue (0);
}

static nil F (B_) ()
{
	assembler_set_byte (0, f_mnemonic.base);

	if (f_mnemonic.source.type == ARG_BYTE) {
		assembler_set_byte (1, f_mnemonic.source.data);
	}

	assembler_continue (0);
}

static nil F (W_) ()
{
	assembler_set_byte (0, f_mnemonic.base);
	assembler_set_byte (1, f_mnemonic.source.data & 0xFF);
	assembler_set_byte (2, f_mnemonic.source.data >> 0x8);

	assembler_continue (0);
}

static nil F (_R) ()
{
	assembler_set_byte (0, f_mnemonic.base + f_mnemonic.target.data);
	assembler_continue (0);
}

static nil F (MC) ()
{
	assembler_set_cursor (
		f_mnemonic.source.data
    );
}

static nil (* f_generate []) () = {
	F (__), F (R_), F (RB), F (RR), F (P_), 
	F (PW), F (C_), F (B_), F (W_), F (_R), F (MC)
};

static s32 mnemonic_generate ()
{
	f_generate [assembler_get_type (f_mnemonic.base)] ();

	return e_configuration.success;
}
