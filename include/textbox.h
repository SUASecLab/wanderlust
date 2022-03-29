#ifndef WANDERLUST_TEXTBOX
#define WANDERLUST_TEXTBOX

/* ************************************************************************
 *
 * MACRO
 *
 * ***********************************************************************/

#define TEXTBOX_UL_CORNER_X 40

#define TEXTBOX_UL_CORNER_Y 184

#define TEXTBOX_LL_CORNER_X	40

#define TEXTBOX_LL_CORNER_Y 224

#define TEXTBOX_UR_CORNER_X 288

#define TEXTBOX_UR_CORNER_Y 184

#define TEXTBOX_LR_CORNER_X	288

#define TEXTBOX_LR_CORNER_Y 224

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 textbox_initialize (lua_State *);

nil textbox_terminate ();

s08 textbox_pending ();

s32 textbox_enqueue (lua_State *);

nil	textbox_handle ();

nil textbox_update ();

nil textbox_render ();

#endif
