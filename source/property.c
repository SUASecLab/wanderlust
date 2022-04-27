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

#include "type.h"
#include "config.h"
#include "morph.h"
#include "property.h"

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
 * ************************************************************************/

static s08		f_initialized 	= WANDERLUST_FAILURE;

static nil	*	f_properties	= 0;

/* ************************************************************************
 *
 * COMPARISON
 *
 * ************************************************************************/

static u08 compare_property_by_hash (nil * item, nil * value)
{
	if (! item)
	{
		return 0;
	}

	if (! value)
	{
		return 0;
	}

	return ((wanderlust_property *) item)->hash == * ((u32 *) value);
}

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - CREATION, DELETION
 *
 * ************************************************************************/

static wanderlust_property * create_wanderlust_property_integer (lua_State * L)
{
	wanderlust_property * p = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto finish;
	}

	if (lua_gettop (L) < 3)
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 3)))
	{
		goto finish;
	}

	if (! (p = SDL_malloc (sizeof (wanderlust_property))))
	{
		goto finish;
	}

	p->data.integer = lua_tointeger (L, 3);

	SET_S32_CONST (p->hash, mhash (lua_tostring (L, 2)));
	SET_S32_CONST (p->type, e_configuration.property.integer);

	finish:

		lua_settop (L, 0);

		return p;
}

static wanderlust_property * create_wanderlust_property_string (lua_State * L)
{
	wanderlust_property * p = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto finish;
	}

	if (lua_gettop (L) < 3)
	{
		goto finish;
	}

	if (! (lua_isstring (L, 3)))
	{
		goto finish;
	}

	if (! (p = SDL_malloc (sizeof (wanderlust_property))))
	{
		goto finish;
	}

	if (! (p->data.string = (s08*)type_secure_copy_string (lua_tostring (L, 3))))
	{
		goto FAILURE;
	}

	SET_S32_CONST (p->hash, mhash (lua_tostring (L, 2)));
	SET_U32_CONST (p->type, e_configuration.property.string);

	goto finish;

	FAILURE:

		SDL_free (p);

		p = e_configuration.null;

	finish:

		lua_settop (L, 0);

		return p;
}

static wanderlust_property * create_wanderlust_property_point (lua_State * L)
{
	wanderlust_property * p = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto finish;
	}

	if (lua_gettop (L) < 4)
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 3)))
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 4)))
	{
		goto finish;
	}

	if (! (p = SDL_malloc (sizeof (wanderlust_property))))
	{
		goto finish;
	}

	p->data.point.x = lua_tonumber (L, 3);
	p->data.point.y = lua_tonumber (L, 4);

	SET_U32_CONST (p->hash, mhash (lua_tostring (L, 2)));
	SET_U32_CONST (p->type, e_configuration.property.point);

	finish:

		lua_settop (L, 0);

		return p;
}

static wanderlust_property * create_wanderlust_property_rectangle (lua_State * L)
{
	wanderlust_property * p = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto finish;
	}

	if (lua_gettop (L) < 6)
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 3)))
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 4)))
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 5)))
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 6)))
	{
		goto finish;
	}

	if (! (p = SDL_malloc (sizeof (wanderlust_property))))
	{
		goto finish;
	}

	p->data.rectangle.x = lua_tonumber (L, 3);
	p->data.rectangle.y = lua_tonumber (L, 4);
	p->data.rectangle.w = lua_tonumber (L, 5);
	p->data.rectangle.h = lua_tonumber (L, 6);

	SET_U32_CONST (p->hash, mhash (lua_tostring (L, 2)));
	SET_U32_CONST (p->type, e_configuration.property.rectangle);

	finish:

		lua_settop (L, 0);

		return p;
}

static nil update_wanderlust_property_integer (wanderlust_property * p, lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto finish;
	}

	if (lua_gettop (L) < 3)
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 3)))
	{
		goto finish;
	}

	p->data.integer = lua_tointeger (L, 3);

	finish:

		lua_settop (L,  0);
}

static nil update_wanderlust_property_string (wanderlust_property * p, lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto finish;
	}

	if (lua_gettop (L) < 3)
	{
		goto finish;
	}

	if (! (lua_isstring (L, 3)))
	{
		goto finish;
	}

	if (p->data.string)
	{
		SDL_free (p->data.string);
	}

	p->data.string = (s08*)type_secure_copy_string (lua_tostring (L, 3));

	finish:

		lua_settop (L, 0);
}

static nil update_wanderlust_property_point (wanderlust_property * p, lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto finish;
	}

	if (lua_gettop (L) < 4)
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 3)))
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 4)))
	{
		goto finish;
	}

	p->data.point.x = lua_tonumber (L, 3);
	p->data.point.y = lua_tonumber (L, 4);

	finish:

		lua_settop (L, 0);
}

static nil update_wanderlust_property_rectangle (wanderlust_property * p, lua_State * L)
{
	if (f_initialized == e_configuration.failure)
	{
		goto finish;
	}

	if (lua_gettop (L) < 6)
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 3)))
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 4)))
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 5)))
	{
		goto finish;
	}

	if (! (lua_isnumber (L, 6)))
	{
		goto finish;
	}

	p->data.rectangle.x = lua_tonumber (L, 3);
	p->data.rectangle.y = lua_tonumber (L, 4);
	p->data.rectangle.w = lua_tonumber (L, 5);
	p->data.rectangle.h = lua_tonumber (L, 6);

	finish:

		lua_settop (L, 0);
}

static nil delete_wanderlust_property_integer (wanderlust_property * p)
{
	if (! p)
	{
		return;
	}

	SDL_free (p);
}

static nil delete_wanderlust_property_string (wanderlust_property * p)
{
	if (! p)
	{
		return;
	}

	if (p->data.string)
	{
		SDL_free (p->data.string);
	}

	SDL_free (p);
}

static nil delete_wanderlust_property_point (wanderlust_property * p)
{
	if (! p)
	{
		return;
	}

	SDL_free (p);
}

static nil delete_wanderlust_property_rectangle (wanderlust_property * p)
{
	if (! p)
	{
		return;
	}

	SDL_free (p);
}

/* ************************************************************************
 *
 * BANKS
 *
 * ************************************************************************/

static wanderlust_property * (* create [PROPERTY_TYPES]) (lua_State *) =
{
	create_wanderlust_property_integer,
	create_wanderlust_property_string,
	create_wanderlust_property_point,
	create_wanderlust_property_rectangle
};

static nil (* update [PROPERTY_TYPES]) (wanderlust_property *, lua_State *) =
{
	update_wanderlust_property_integer,
	update_wanderlust_property_string,
	update_wanderlust_property_point,
	update_wanderlust_property_rectangle
};

static nil (* delete [PROPERTY_TYPES]) (wanderlust_property *) =
{
	delete_wanderlust_property_integer,
	delete_wanderlust_property_string,
	delete_wanderlust_property_point,
	delete_wanderlust_property_rectangle
};

/* ************************************************************************
 *
 * FILE SCOPE FUNCTIONS - HELPING HANDS
 *
 * ************************************************************************/

static nil delete_wanderlust_property (nil * item)
{
	wanderlust_property * p = item;

	if (! p)
	{
		return;
	}

	delete [p->type] (p);
}

/* ************************************************************************
 *
 * INTERFACE
 *
 * ************************************************************************/

s08 property_initialize (lua_State * L)
{
	if (f_initialized == e_configuration.success)
	{
		return f_initialized;
	}

	f_initialized 	= e_configuration.success;
	f_properties	= e_configuration.null;

	if (! L)
	{
		goto FAILURE;
	}

	if (! (f_properties = mcreate (M_SINGLE_LINKED_LIST)))
	{
		goto FAILURE;
	}

	lua_register (L, WL_GET_PROPERTY, property_get_wanderlust_property);

	lua_register (L, WL_SET_PROPERTY, property_set_wanderlust_property);

	return e_configuration.success;

	FAILURE:

		property_terminate ();

		return e_configuration.failure;
}

nil property_terminate ()
{
	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (f_properties)
	{
		f_properties = mdelete (f_properties, delete_wanderlust_property);
	}

	f_initialized = e_configuration.failure;
}

nil	property_create_integer (const s08 * key, u32 integer)
{
	wanderlust_property 	*	p = 0;
	u32							h = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! key)
	{
		return;
	}

	h = (u32)mhash ((const char*)key);
	p = mfind (f_properties, & h, compare_property_by_hash, 0);

	if (p)
	{
		p->data.integer = integer;

		return;
	}

	if (! (p = SDL_malloc (sizeof (wanderlust_property))))
	{
		return;
	}

	SET_U32_CONST (p->type, e_configuration.property.integer);
	SET_U32_CONST (p->hash, h);

	p->data.integer = integer;

	madd (f_properties, p);
}

nil	property_create_string (const s08 * key, const s08 * string)
{
	wanderlust_property 	*	p = 0;
	u32							h = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! key)
	{
		return;
	}

	if (! string)
	{
		return;
	}

	h = mhash ((const char*)key);
	p = mfind (f_properties, & h, compare_property_by_hash, 0);

	if (p)
	{
		SDL_free (p->data.string);
	} 
	else 
	{
		if (! (p = SDL_malloc (sizeof (wanderlust_property))))
		{
			return;
		}	
	
		SET_U32_CONST (p->type, e_configuration.property.string);
		SET_U32_CONST (p->hash, h);

		madd (f_properties, p);
	}

	p->data.string = (s08 *)string;
}

nil	property_create_point (const s08 * key, u32 x, u32 y)
{
	wanderlust_property 	*	p = 0;
	u32							h = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! key)
	{
		return;
	}

	h = mhash ((const char*)key);
	p = mfind (f_properties, & h, compare_property_by_hash, 0);

	if (p)
	{
		return;
	}

	if (! (p = SDL_malloc (sizeof (wanderlust_property))))
	{
		return;
	}

	SET_U32_CONST (p->type, e_configuration.property.point);
	SET_U32_CONST (p->hash, h);

	p->data.point.x = x;
	p->data.point.y = y;

	madd (f_properties, p);
}

nil	property_create_rectangle (const s08 * key, u32 x, u32 y, u32 w, u32 h)
{
	wanderlust_property 	*	p 		= 0;
	u32							hash 	= 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! key)
	{
		return;
	}

	hash 	= mhash ((const char*)key);
	p 		= mfind (f_properties, & hash, compare_property_by_hash, 0);

	if (p)
	{
		return;
	}

	if (! (p = SDL_malloc (sizeof (wanderlust_property))))
	{
		return;
	}

	SET_U32_CONST (p->type, e_configuration.property.rectangle);
	SET_U32_CONST (p->hash, hash);

	p->data.rectangle.x = x;
	p->data.rectangle.y = y;
	p->data.rectangle.w = w;
	p->data.rectangle.h = h;

	madd (f_properties, p);
}

s32 property_delete_wanderlust_property (lua_State * L)
{
	wanderlust_property			*	property	= 0;
	u32								hash		= 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < 2)
	{
		goto FAILURE;
	}

	if (! lua_isstring(L, 1))
	{
		goto FAILURE;
	}

	hash = mhash (lua_tostring (L, 1));

	if (! (property = mfind (f_properties, &hash, compare_property_by_hash, 0)))
	{
		goto FAILURE;
	}

	mremove (f_properties, &hash, compare_property_by_hash, 0);

	lua_settop (L, 0);

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32	property_set_wanderlust_property (lua_State * L)
{
	wanderlust_property_type 		type		= 0;
	wanderlust_property			*	property	= 0;
	u32								hash		= 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (lua_gettop (L) < 2)
	{
		goto FAILURE;
	}

	if (! lua_isnumber (L, 1))
	{
		goto FAILURE;
	}
	
	if (! lua_isstring (L, 2))
	{
		goto FAILURE;
	}

	if ((type = lua_tointeger (L, 1)) >= e_configuration.property.types)
	{
		goto FAILURE;
	}

	hash 		= mhash (lua_tostring (L, 2));
	property 	= mfind (f_properties, & hash, compare_property_by_hash, 0);

	if (property)
	{
		update [property->type] (property, L);
	}
	else
	{
		if (! (property = create [type] (L)))
		{
			goto FAILURE;
		}

		madd (f_properties, property);
	}

	lua_pop (L, lua_gettop (L));

	return e_configuration.success;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

s32	property_get_wanderlust_property (lua_State * L)
{
	wanderlust_property	*	property	= 0;
	u32						hash		= 0;
	u32						value		= 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! lua_gettop (L))
	{
		goto FAILURE;
	}

	/*
	 * PROPERTY NAME
	 */
	if (! lua_isstring (L, 1))
	{
		goto FAILURE;
	}

	hash 		= mhash (lua_tostring (L, 1));
	property 	= mfind (f_properties, &hash, compare_property_by_hash, 0);

	if (! property)
	{
		goto FAILURE;
	}

	switch (property->type)
	{
		case PROPERTY_INTEGER:

			lua_pushnumber (L, property->data.integer);

			value = 1;

			break;

		case PROPERTY_STRING:

			lua_pushstring (L, (const char*)property->data.string);

			value = 1;

			break;

		case PROPERTY_POINT:

			lua_pushnumber (L, property->data.point.x);
			lua_pushnumber (L, property->data.point.y);

			value = 2;

			break;

		case PROPERTY_RECTANGLE:

			lua_pushnumber (L, property->data.rectangle.x);
			lua_pushnumber (L, property->data.rectangle.y);
			lua_pushnumber (L, property->data.rectangle.w);
			lua_pushnumber (L, property->data.rectangle.h);

			value = 4;

			break;

		default:

			goto FAILURE;
	}

	return value;

	FAILURE:

		lua_settop (L, 0);

		return e_configuration.failure;
}

u32 property_get_integer (const s08 * key)
{
	wanderlust_property	*	p = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! key)
	{
		goto FAILURE;
	}

	h = mhash ((const char*)key);
	p = mfind (f_properties, & h, compare_property_by_hash, 0);

	if (! p)
	{
		goto FAILURE;
	}

	if (p->type != e_configuration.property.integer)
	{
		goto FAILURE;
	}

	return p->data.integer;

	FAILURE:

		return e_configuration.zero;
}

nil	property_set_integer (const s08 * key, int value)
{
	wanderlust_property	*	p = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! key)
	{
		return;
	}

	h = mhash ((const char*)key);
	p = mfind (f_properties, &h, compare_property_by_hash, 0);

	if (! p)
	{
		return;
	}

	if (p->type != e_configuration.property.integer)
	{
		return;
	}

	p->data.integer = value;
}

const s08 * property_get_string (const s08 * key)
{
	wanderlust_property	*	p = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! key)
	{
		goto FAILURE;
	}

	h = mhash ((const char*)key);
	p = mfind (f_properties, &h, compare_property_by_hash, 0);

	if (! p)
	{
		goto FAILURE;
	}

	if (p->type != e_configuration.property.string)
	{
		goto FAILURE;
	}

	return p->data.string;

	FAILURE:

		return e_configuration.null;
}

nil	property_set_string (const s08 * key, const s08 * value)
{
	wanderlust_property	*	p = 0;
	u32						h = 0;
	s08					*	t = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! key)
	{
		return;
	}

	if (! value)
	{
		return;
	}

	h = mhash ((const char*)key);
	p = mfind (f_properties, &h, compare_property_by_hash, 0);

	if (! p)
	{
		return;
	}

	if (p->type != e_configuration.property.string)
	{
		return;
	}

	if (! (t = (s08*)type_secure_copy_string ((const char*)value)))
	{
		return;
	}

	if (p->data.string)
	{
		SDL_free (p->data.string);
	}

	p->data.string = t;
}

SDL_Point property_get_point (const s08 * key)
{
	static	SDL_Point 					d = {0, 0};
			wanderlust_property		*	p = 0;
			u32							h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! key)
	{
		goto FAILURE;
	}

	h = mhash ((const char*)key);
	p = mfind (f_properties, &h, compare_property_by_hash, 0);

	if (! p)
	{
		goto FAILURE;
	}

	if (p->type != e_configuration.property.point)
	{
		goto FAILURE;
	}

	return p->data.point;

	FAILURE:

		return d;
}

nil	property_set_point (const s08 * key, SDL_Point * value)
{
	wanderlust_property	*	p = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! key)
	{
		return;
	}

	h = mhash ((const char*)key);
	p = mfind (f_properties, &h, compare_property_by_hash, 0);

	if (! p)
	{
		return;
	}

	if (p->type != e_configuration.property.point)
	{
		return;
	}

	p->data.point.x = value->x;
	p->data.point.y = value->y;
}

SDL_Rect property_get_rectangle (const s08 * key)
{
	static	SDL_Rect 					d = {0, 0, 0, 0};
			wanderlust_property		*	p = 0;
			u32							h = 0;

	if (f_initialized == e_configuration.failure)
	{
		goto FAILURE;
	}

	if (! key)
	{
		goto FAILURE;
	}

	h = mhash ((const char*)key);

	p = mfind (f_properties, & h, compare_property_by_hash, 0);

	if (! p)
	{
		goto FAILURE;
	}

	if (p->type != e_configuration.property.rectangle)
	{
		goto FAILURE;
	}

	return p->data.rectangle;

	FAILURE:

		return d;
}

nil	property_set_rectangle (const s08 * key, SDL_Rect * value)
{
	wanderlust_property	*	p = 0;
	u32						h = 0;

	if (f_initialized == e_configuration.failure)
	{
		return;
	}

	if (! key)
	{
		return;
	}

	h = mhash ((const char*)key);
	p = mfind (f_properties, &h, compare_property_by_hash, 0);

	if (! p)
	{
		return;
	}

	if (p->type != e_configuration.property.rectangle)
	{
		return;
	}

	p->data.rectangle.x = value->x;
	p->data.rectangle.y = value->y;
	p->data.rectangle.w = value->w;
	p->data.rectangle.h = value->h;
}
