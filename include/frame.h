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
