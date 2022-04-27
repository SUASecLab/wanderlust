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

#ifndef WANDERLUST_ENTITY_HERO
#define WANDERLUST_ENTITY_HERO

nil					entity_hero_initialize ();

nil					entity_hero_terminate ();

wanderlust_entity * entity_hero_create (lua_State *);

nil					entity_hero_direct (wanderlust_entity *);

nil					entity_hero_delete (wanderlust_entity *);

nil					entity_hero_handle (wanderlust_entity *);

nil					entity_hero_update (wanderlust_entity *);

nil					entity_hero_render (wanderlust_entity *);

nil					entity_hero_adjust (wanderlust_entity *);

nil					entity_hero_select (wanderlust_entity *);

nil					entity_hero_recall (wanderlust_entity *);

nil					entity_hero_excite (wanderlust_entity *);

nil					entity_hero_expire (wanderlust_entity *);

nil					entity_hero_modify (wanderlust_entity *, lua_State *, u16);

#endif
