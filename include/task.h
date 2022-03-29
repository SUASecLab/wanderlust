#ifndef WANDERLUST_TASKS
#define WANDERLUST_TASKS

/*******************************************************************************
 *
 * TASK IDENTIFIER
 *
 ******************************************************************************/

#define TASK_ENGINE_HALT			0x08

#define TASK_VIDEO_SHOW				0x10
#define TASK_VIDEO_HIDE				0x11
#define	TASK_VIDEO_FULLSCREEN		0x12
#define TASK_VIDEO_WINDOW			0x13
#define TASK_VIDEO_SCALING			0x14
#define TASK_VIDEO_SCALE_INC		0x15
#define TASK_VIDEO_SCALE_DEC		0x16

#define TASK_IMAGE_LOAD				0x18

#define TASK_ENTITY_ENQUEUE			0x20
#define TASK_ENTITY_EXPIRE			0x21

#define TASK_STATE_ENTER			0x28
#define TASK_STATE_LEAVE			0x29
#define TASK_STATE_FETCH			0x2A
#define TASK_STATE_CLEAR			0x2B
#define TASK_STATE_SELECT			0x2C
#define TASK_STATE_REMOVE			0x2D
#define TASK_STATE_SET_BG			0x2E

#define TASK_MAP_LOAD				0x30
#define TASK_MAP_SELECT				0x31
#define TASK_MAP_WARP				0x32

#define TASK_GAMEPAD_REBUILD		0x38

#define TASK_TRANSITION_ENQUEUE		0x40
#define TASK_TRANSITION_FINISH		0x41

#define TASK_FONT_CREATE			0x48

#define TASK_MENU_CREATE			0x50
#define TASK_MENU_APPEND			0x51
#define TASK_MENU_SELECT			0x52
#define TASK_MENU_REPEAL			0x53

#define TASK_TERMINAL_CREATE		0x54
#define TASK_TERMINAL_DELETE		0x55
#define TASK_TERMINAL_SHOW			0x56
#define TASK_TERMINAL_HIDE			0x57
#define TASK_TERMINAL_PUSH_BYTE		0x58
#define TASK_TERMINAL_UNDO_BYTE 	0x59
#define TASK_TERMINAL_CURSOR_INC	0x5A
#define TASK_TERMINAL_CURSOR_DEC	0x5B
#define TASK_TERMINAL_RUN			0x5C
#define TASK_TERMINAL_CLEAR			0x5D
#define TASK_TERMINAL_MODE_TEXT     0x5E
#define TASK_TERMINAL_MODE_DATA     0x5F

/*******************************************************************************
 *
 * INTERFACE
 *
 ******************************************************************************/

s08 task_initialize (lua_State *);

nil task_terminate ();

u32 task_pending ();

nil task_execute ();

s32 task_rundown (lua_State *);

nil task_discard ();

nil task_dismiss (u08);

nil task_register (u08, nil (*) (nil *));

nil task_enqueue (u08, nil *);

#endif
