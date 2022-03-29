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
