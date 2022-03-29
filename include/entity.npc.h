#ifndef WANDERLUST_ENTITY_NPC
#define WANDERLUST_ENTITY_NPC

nil					entity_npc_initialize ();

nil					entity_npc_terminate ();

wanderlust_entity * entity_npc_create (lua_State *);

nil					entity_npc_direct (wanderlust_entity *);

nil					entity_npc_delete (wanderlust_entity *);

nil					entity_npc_handle (wanderlust_entity *);

nil					entity_npc_update (wanderlust_entity *);

nil					entity_npc_render (wanderlust_entity *);

nil					entity_npc_adjust (wanderlust_entity *);

nil					entity_npc_select (wanderlust_entity *);

nil					entity_npc_recall (wanderlust_entity *);

nil					entity_npc_excite (wanderlust_entity *);

nil					entity_npc_expire (wanderlust_entity *);

nil					entity_npc_modify (wanderlust_entity *, lua_State *, u16);

#endif
