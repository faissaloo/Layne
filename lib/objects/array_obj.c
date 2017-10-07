// Copyright (C) 2017 Faissal Bensefia
//
// This file is part of Layne.
//
// Layne is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Layne is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Layne.  If not, see <http://www.gnu.org/licenses/>.

#include <gc.h>
#include <stdlib.h>
#include "dyn_objs.h"
#include "dyn_arrays.h"
#include "array_obj.h"
#include "str_obj.h"
#include "int_obj.h"
#include "bool_obj.h"
#include "factory_obj.h"
#include "none_obj.h"

struct method_list factory_array_methods={
	6,
	{
		{"new",factory_array_new},
		{"cut",factory_array_cut},
		{"set",factory_array_set},
		{"get",factory_array_get},
		{"ins",factory_array_ins},
		{"del",array_del}
	}
};

struct method_list array_methods={
	6,
	{
		{"new",array_new},
		{"cut",array_cut},
		{"set",array_set},
		{"get",array_get},
		{"ins",array_ins},
		{"del",array_del}
	}
};

struct dyn_obj* create_array(struct dyn_array *ary)
{
	object_setup(ARRAY);
	init_methods(self,&array_methods);
	((struct array_obj*)self)->data=ary;
	bind_member(self,"parent",type_factory);
	return self;
}

def_dyn_fn(array_new)
{
	if (args->filled>1)
	{
		return call_method(get_arg(1),"array",dyn_array_create());
	}
	return create_array(NULL);
}

def_dyn_fn(array_cut)
{
	return create_array(dyn_array_cut(((struct array_obj*)SELF)->data,get_int_val(get_arg(1)),get_int_val(get_arg(2))));
}

//Compares every item in the array
def_dyn_fn(array_eq)
{
	if (((struct array_obj*)SELF)->data->filled!=((struct array_obj*)get_arg(1))->data->filled)
	{
		return create_bool(false);
	}

	for (iter_t i=0;i<((struct array_obj*)SELF)->data->filled;i++)
	{
		struct dyn_array *current_item;
		current_item=dyn_array_create();
		dyn_array_append(current_item,dyn_array_get(((struct array_obj*)get_arg(1))->data,i));
		if (!get_bool_val(call_method(dyn_array_get(((struct array_obj*)SELF)->data,i),"eq",current_item)))
		{
			return create_bool(false);
		}
	}
	return create_bool(true);
}

//array[x]=y
def_dyn_fn(array_set)
{
	dyn_array_set(((struct array_obj*)SELF)->data,get_int_val(get_arg(1)),get_arg(2));
	return create_none();
}
//Array[x]
def_dyn_fn(array_get)
{
	return (struct dyn_obj*)dyn_array_get(((struct array_obj*)SELF)->data,get_int_val(get_arg(1)));
}

def_dyn_fn(array_ins)
{
	dyn_array_insert(((struct array_obj*)SELF)->data,get_int_val(get_arg(1)),get_arg(2));
	return create_none();
}

def_dyn_fn(array_del)
{
	dyn_array_remove(((struct array_obj*)SELF)->data,get_int_val(get_arg(1)));
	return create_none();
}
