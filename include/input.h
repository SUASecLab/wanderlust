#ifndef WANDERLUST_INPUT
#define WANDERLUST_INPUT

/* ************************************************************************
 *
 * CONSTANTS
 *
 * ***********************************************************************/

typedef enum
{
	BUTTON_X,
	BUTTON_A,
	BUTTON_B,
	BUTTON_Y,
	BUTTON_BACK,
	BUTTON_GUIDE,
	BUTTON_START,
	BUTTON_L,
	BUTTON_R,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_FULLSCREEN,
	BUTTON_DEC_WINSIZE,
	BUTTON_INC_WINSIZE,
	BUTTON_COUNT
} wanderlust_button_id;

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 input_initialize (lua_State *);

nil input_terminate ();

nil input_button_up (u16, u08);

nil input_button_down (u16, u08);

nil input_device_insert ();

nil input_device_remove (s32);

nil input_axis_set_motion (u08, s16);

s16 input_axis_get_motion (u08);

u08 input_any_key_down ();

s08 input_get_button (wanderlust_button_id);

s08 input_get_buffered_button (wanderlust_button_id);

#endif
