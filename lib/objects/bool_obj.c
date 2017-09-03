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
#include <stdbool.h>

#include "dyn_objs.h"
#include "bool_obj.h"
#include "int_obj.h"
#include "str_obj.h"
#include "factory_obj.h"
#include <stdio.h>
#include "debug.h"

struct method_list factory_bool_methods={
	9,
	{
		{"new",factory_bool_new},
		{"bool",factory_bool_bool},
		{"int",factory_bool_int},
		{"str",factory_bool_str},
		{"bor",factory_bool_or},
		{"or",factory_bool_or},
		{"band",factory_bool_and},
		{"and",factory_bool_and},
		{"xor",factory_bool_xor}
	}
};

struct method_list bool_methods={
	9,
	{
		{"new",bool_new},
		{"bool",bool_bool},
		{"int",bool_int},
		{"str",bool_str},
		{"bor",bool_or},
		{"or",bool_or},
		{"band",bool_and},
		{"and",bool_and},
		{"xor",bool_xor}
	}
};

struct dyn_obj *create_bool_factory()
{
	object_setup(FACTORY);
	((struct factory_obj*)self)->type_to_create=BOOL;

	init_methods(self,&factory_bool_methods);

	bind_member(self,"parent",type_factory);
	return self;
}

bool get_bool_val(struct dyn_obj *obj)
{
	return ((struct bool_obj*)obj)->value;
}

def_dyn_fn(bool_new)
{
	#ifdef DEBUG
		arg_guard(1,2,protect({"self",""}),protect({BOOL,TYPE}));
	#endif
	//Value to create the int from
	if (args->filled>1)
	{
		return call_method(get_arg(1),"bool",dyn_array_create());
	}
	return create_int(0);
}

def_dyn_fn(bool_bool)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({BOOL}));
	#endif

	return create_bool(get_bool_val(SELF));
}

def_dyn_fn(bool_int)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({BOOL}));
	#endif

	return create_int(get_bool_val(SELF));
}

def_dyn_fn(bool_or) //or(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({BOOL,BOOL}));
	#endif

	//bool(self||operand)
	return create_bool(get_bool_val(SELF)||get_bool_val(get_arg(1)));
}

def_dyn_fn(bool_and) //and(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({BOOL,BOOL}));
	#endif

	//bool(self||operand)
	return create_bool(get_bool_val(SELF)&&get_bool_val(get_arg(1)));
}

def_dyn_fn(bool_xor) //xor(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({BOOL,BOOL}));
	#endif

	return create_bool(get_bool_val(SELF)^get_bool_val(get_arg(1)));
}

def_dyn_fn(bool_str)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({BOOL}));
	#endif

	struct dyn_str *new_str;
	new_str=dyn_str_create();
	dyn_str_cat_cstr(new_str,get_bool_val(SELF)?"true":"false");
	return create_str(new_str);
}

struct dyn_obj* create_bool(bool value)
{
	object_setup(BOOL);

	init_methods(self,&bool_methods);

	bind_member(self,"parent",get_member(global,"type"));
	((struct bool_obj*)self)->value=value;
	return self;
}
