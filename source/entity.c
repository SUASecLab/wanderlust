#include "type.h"
#include "config.h"
#include "font.h"
#include "task.h"
#include "morph.h"
#include "entity.h"

#include "entity.hero.h"
#include "entity.npc.h"
#include "entity.retrobutton.h"
#include "entity.primitive.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

static s08		f_initialized 	= 0;

static nil	*	f_entities		= 0;

/* ************************************************************************
 *
 * TASKS
 *
 * ***********************************************************************/

static nil task_entity_enqueue (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	menqueue (f_entities, item);
}

static nil task_entity_expire (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	entity_expire (item, e_configuration.null);
}

/* ************************************************************************
 *
 * BANKS
 *
 * ***********************************************************************/

static wanderlust_entity * (* f_create_entity [ENTITY_TYPE_COUNT]) (lua_State *) =
{
	entity_hero_create,
	entity_npc_create,
	entity_retrobutton_create,
	entity_primitive_create
};

static nil (* f_initialize_entity [ENTITY_TYPE_COUNT]) () =
{
	entity_hero_initialize,
	entity_npc_initialize,
	entity_retrobutton_initialize,
	entity_primitive_initialize
};

static nil (* f_terminate_entity [ENTITY_TYPE_COUNT]) () =
{
	entity_hero_terminate,
	entity_npc_terminate,
	entity_retrobutton_terminate,
	entity_primitive_terminate
};

static nil (* f_direct_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	entity_hero_direct,
	entity_npc_direct,
	entity_retrobutton_direct,
	entity_primitive_direct
};

static nil (* f_delete_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	entity_hero_delete,
	entity_npc_delete,
	entity_retrobutton_delete,
	entity_primitive_delete
};

static nil (* f_handle_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	entity_hero_handle,
	entity_npc_handle,
	entity_retrobutton_handle,
	entity_primitive_handle
};

static nil (* f_update_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	entity_hero_update,
	entity_npc_update,
	entity_retrobutton_update,
	entity_primitive_update,
};

static nil (* f_render_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	entity_hero_render,
	entity_npc_render,
	entity_retrobutton_render,
	entity_primitive_render
};

static nil (* f_adjust_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	entity_hero_adjust,
	entity_npc_adjust,
	entity_retrobutton_adjust,
	entity_primitive_adjust
};

static nil (* f_select_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	entity_hero_select,
	entity_npc_select,
	entity_retrobutton_select,
	entity_primitive_select,
};

static nil (* f_recall_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	entity_hero_recall,
	entity_npc_recall,
	entity_retrobutton_recall,
	entity_primitive_recall
};

static nil (* f_excite_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	entity_hero_excite,
	entity_npc_excite,
	entity_retrobutton_excite,
	entity_primitive_excite
};

static nil (* f_expire_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *) =
{
	entity_hero_expire,
	entity_npc_expire,
	entity_retrobutton_expire,
	entity_primitive_expire
};

static nil (* f_modify_entity [ENTITY_TYPE_COUNT]) (wanderlust_entity *, lua_State *, u16) =
{
	entity_hero_modify,
	entity_npc_modify,
	entity_retrobutton_modify,
	entity_primitive_modify
};

/* ************************************************************************
 *
 * INTERFACE
 *
 * ***********************************************************************/

s08 entity_initialize (lua_State * L)
{
	u32	index = 0;

	if (f_initialized == e_configuration.success)
	{
		goto SUCCESS;
	}

	f_initialized 	= e_configuration.success;
	f_entities		= e_configuration.null;

	if (! L)
	{
		goto FAILURE;
	}

	lua_register (L, WL_CREATE_ENTITY, entity_enqueue);

	if (! (f_entities = mcreate (e_configuration.morph.queue)))
	{
		goto FAILURE;
	}

	task_register (TASK_ENTITY_ENQUEUE, task_entity_enqueue);
	task_register (TASK_ENTITY_EXPIRE, 	task_entity_expire);

	for (index = 0; index < ENTITY_TYPE_COUNT; index++)
	{
		f_initialize_entity [index] ();
	}

	SUCCESS:

		return e_configuration.success;

	FAILURE:

		entity_terminate ();

		return e_configuration.failure;
}

nil entity_terminate ()
{
	u32 index;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	for (index = 0; index < ENTITY_TYPE_COUNT; index++)
	{
		f_terminate_entity [index] ();
	}

	f_entities		= mdelete (f_entities, entity_delete);
	f_initialized	= e_configuration.failure;
}

nil entity_fetch (nil * list)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! msize (f_entities))
	{
		return;
	}

	if (! list)
	{
		return;
	}

	if (mtype (list) != e_configuration.morph.doublelinkedlist)
	{
		return;
	}

	while (msize (f_entities))
	{
		madd (list, mdequeue (f_entities));
	}
}

s32 entity_enqueue (lua_State * L)
{
	wanderlust_entity * e = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! L)
	{
		goto FAILURE;
	}

	if (! (e = entity_create (L)))
	{
		goto FAILURE;
	}

	task_enqueue (TASK_ENTITY_ENQUEUE, e);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

#define ENTITY_CREATE_ARG_COUNT		1

#define ENTITY_CREATE_ARG_POS_TYPE	1

wanderlust_entity * entity_create (lua_State * L)
{
	u08 type;

	if (! L)
	{
		goto FAILURE;
	}

	if (! lua_gettop (L))
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, ENTITY_CREATE_ARG_POS_TYPE))
	{
		goto FAILURE;
	}

	type = lua_tointeger (L, ENTITY_CREATE_ARG_POS_TYPE);

	if (type >= ENTITY_TYPE_COUNT)
	{
		goto FAILURE;
	}

	return f_create_entity [type] (L);

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.null;
}

nil entity_delete (nil * item)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_delete_entity [((wanderlust_entity *) item) -> type] (item);
}

nil entity_direct (nil * item, nil * value)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_direct_entity [((wanderlust_entity *) item) -> type] (item);
}

nil entity_handle (nil * item, nil * value)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_handle_entity [((wanderlust_entity *) item) -> type] (item);
}

nil entity_update (nil * item, nil * value)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_update_entity [((wanderlust_entity *) item) -> type] (item);
}

nil entity_render (nil * item, nil * value)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_render_entity [((wanderlust_entity *) item) -> type] (item);
}

nil entity_adjust (nil * item, nil * value)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_adjust_entity [((wanderlust_entity *) item) -> type] (item);
}

nil	entity_select (nil * item, nil * value)
{
	wanderlust_entity	*	e = item;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! e)
	{
		return;
	}

	if (! value)
	{
		return;
	}

	h = * (u32 *) value;

	if (h == e -> hash)
	{
		f_select_entity [e -> type] (e);
	}
	else
	{
		f_recall_entity [e -> type] (e);
	}
}

nil	entity_recall (nil * item, nil * value)
{
	wanderlust_entity	*	e = item;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! e)
	{
		return;
	}

	if (! value)
	{
		return;
	}

	h = * (u32 *) value;

	if (h == e -> hash)
	{
		f_recall_entity [e -> type] (e);
	}
}

nil	entity_excite (nil * item, nil * value)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_excite_entity [((wanderlust_entity *) item) -> type] (item);
}

nil	entity_expire (nil * item, nil * value)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	f_expire_entity [((wanderlust_entity *) item) -> type] (item);
}

nil entity_modify (nil * item, lua_State * L, u16 index)
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! item)
	{
		return;
	}

	if (! L)
	{
		return;
	}

	f_modify_entity [((wanderlust_entity *) item) -> type] (item, L, index);
}
