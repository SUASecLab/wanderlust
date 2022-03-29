#ifndef ENTITY_PRIMITIVE
#define ENTITY_PRIMITIVE

nil					entity_primitive_initialize ();

nil					entity_primitive_terminate ();

wanderlust_entity * entity_primitive_create (lua_State *);

nil					entity_primitive_direct (wanderlust_entity *);

nil					entity_primitive_delete (wanderlust_entity *);

nil					entity_primitive_handle (wanderlust_entity *);

nil					entity_primitive_update (wanderlust_entity *);

nil					entity_primitive_render (wanderlust_entity *);

nil					entity_primitive_adjust (wanderlust_entity *);

nil					entity_primitive_select (wanderlust_entity *);

nil					entity_primitive_recall (wanderlust_entity *);

nil					entity_primitive_excite (wanderlust_entity *);

nil					entity_primitive_expire (wanderlust_entity *);

nil					entity_primitive_modify (wanderlust_entity *, lua_State *, u16);

#endif
