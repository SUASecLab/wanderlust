#ifndef WANDERLUST_FRAME
#define WANDERLUST_FRAME

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

wanderlust_frame *	frame_create_wanderlust_frame (u16, u16, u16, u16, u16, SDL_Texture*);

nil					frame_delete_wanderlust_frame (wanderlust_frame *);

nil					frame_resize_wanderlust_frame (wanderlust_frame *, u16, u16, u16, u16);

#endif
