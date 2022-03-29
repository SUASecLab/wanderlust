#ifndef WANDERLUST_ENTITY_RETROBUTTON
#define WANDERLUST_ENTITY_RETROBUTTON

nil					entity_retrobutton_initialize ();

nil					entity_retrobutton_terminate ();

wanderlust_entity * entity_retrobutton_create (lua_State *);

nil					entity_retrobutton_direct (wanderlust_entity *);

nil					entity_retrobutton_delete (wanderlust_entity *);

nil					entity_retrobutton_handle (wanderlust_entity *);

nil					entity_retrobutton_update (wanderlust_entity *);

nil					entity_retrobutton_render (wanderlust_entity *);

nil					entity_retrobutton_adjust (wanderlust_entity *);

nil					entity_retrobutton_select (wanderlust_entity *);

nil					entity_retrobutton_recall (wanderlust_entity *);

nil					entity_retrobutton_excite (wanderlust_entity *);

nil					entity_retrobutton_expire (wanderlust_entity *);

nil					entity_retrobutton_modify (wanderlust_entity *, lua_State *, u16);

#endif
