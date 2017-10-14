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
#include "str_obj.h"
#include "int_obj.h"
#include "bool_obj.h"
#include "factory_obj.h"

struct method_list str_methods={
	9,
	{
		method_pair("new",str_new),
		method_pair("add",str_add),
		method_pair("iadd",str_iadd),
		method_pair("cut",str_cut),
		method_pair("hash",str_hash),
		method_pair("int",str_int),
		method_pair("str",str_str),
		method_pair("get",str_get),
		method_pair("eq",str_eq)
	}
};

struct dyn_obj *create_str_factory()
{
	factory_setup(STR);
	return self;
}

struct dyn_str* get_str_val(struct dyn_obj *obj)
{
	return ((struct str_obj*)obj)->data;
}

struct dyn_obj* create_str(struct dyn_str *data)
{
	obj_setup_basic(STR);
	((struct str_obj*)self)->data=dyn_str_create();

	if (data!=NULL)
	{
		dyn_str_cat(((struct str_obj*)self)->data,data);
	}

	return self;
}

def_dyn_fn(str_eq)
{
	return create_bool(dyn_str_eq(get_str_val(SELF),get_str_val(get_arg(1))));
}

def_dyn_fn(str_new)
{
	if (args->filled>1)
	{
		return call_method(get_arg(1),"str",dyn_array_create());
	}
	return create_str(NULL);
}

def_dyn_fn(str_add)
{
	struct dyn_obj *new_str;
	new_str=create_str(get_str_val(SELF));
	dyn_str_cat(get_str_val(new_str),get_str_val(get_arg(1)));
	return new_str;
}

def_dyn_fn(str_iadd)
{
	dyn_str_cat(get_str_val(SELF),get_str_val(get_arg(1)));
	return SELF;
}

def_dyn_fn(str_get)
{
	return create_str(dyn_str_get_char(get_str_val(SELF), get_int_val(get_arg(1))));
}

def_dyn_fn(str_cut) //cut(self,start,stop)
{
	return create_str(dyn_str_cut(get_str_val(SELF),get_int_val(get_arg(1)),get_int_val(get_arg(2))));
}

def_dyn_fn(str_hash)
{
	return create_int(dyn_str_hash(get_str_val(SELF)));
}

def_dyn_fn(str_int)
{
	//The dyn_str library should have a to integer function
	return create_int(dyn_str_to_int(SELF));
}

def_dyn_fn(str_str)
{
	return create_str(get_str_val(SELF));
}
