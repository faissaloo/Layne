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

#include "dyn_objs.h"
#include "int_obj.h"
#include "str_obj.h"
#include "bool_obj.h"
#include "none_obj.h"
#include "debug.h"
#include <stdio.h>
#include "factory_obj.h"

struct dyn_obj *global;

struct method_list factory_int_methods={
	17,
	{
		{"new",factory_int_new},
		{"add",factory_int_add},
		{"sub",factory_int_sub},
		{"mul",factory_int_mul},
		{"div",factory_int_div},
		{"int",factory_int_int},
		{"str",factory_int_str},
		{"hash",factory_int_hash},
		{"copy",factory_int_copy},
		{"eq",factory_int_eq},
		{"lt",factory_int_lt},
		{"le",factory_int_le},
		{"gt",factory_int_gt},
		{"ge",factory_int_ge},
		{"or",factory_int_or},
		{"and",factory_int_and},
		{"xor",factory_int_xor}
	}
};
struct dyn_obj *create_int_factory()
{
	object_setup(FACTORY);
	((struct factory_obj*)self)->type_to_create=INT;
	bind_member(self,"parent",*type_parent_list[self->cur_type]);

	inherit_factory_setup();
	//init_methods(self,&factory_int_methods);
	return self;
}

int get_int_val(struct dyn_obj *obj)
{
	return ((struct int_obj*)obj)->value;
}

struct method_list int_methods={
	17,
	{
		{"new",int_new},
		{"add",int_add},
		{"sub",int_sub},
		{"mul",int_mul},
		{"div",int_div},
		{"int",int_int},
		{"str",int_str},
		{"hash",int_hash},
		{"copy",int_copy},
		{"eq",int_eq},
		{"lt",int_lt},
		{"le",int_le},
		{"gt",int_gt},
		{"ge",int_ge},
		{"or",int_or},
		{"and",int_and},
		{"xor",int_xor}
	}
};

struct dyn_obj* create_int(int value)
{
	object_setup(INT);
	bind_member(self,"parent",*type_parent_list[self->cur_type]);
	inherit_setup();
	((struct int_obj*)self)->value=value;
	return self;
}

def_dyn_fn(int_new)
{
	#ifdef DEBUG
		arg_guard(1,2,protect({"self","x"}),protect({INT,TYPE}));
	#endif

	//Value to create the int from
	if (args->filled>1)
	{
		return call_method(get_arg(1),"int",dyn_array_create());
	}
	return create_int(0);
}

def_dyn_fn(int_int)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({INT}));
	#endif

	return create_int(get_int_val(SELF));
}

def_dyn_fn(int_str)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({INT}));
	#endif

	return create_str(dyn_str_from_int(get_int_val(SELF)));
}

def_dyn_fn(int_bool)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({INT}));
	#endif

	return create_bool(get_int_val(SELF));
}

//Normal operations will create new objects, while in place stuff like += will modify an existing object
def_dyn_fn(int_copy) //copy(self,new_obj)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","dest"}),protect({INT,INT}));
	#endif

	//new_obj.function=self.function
	((struct int_obj*)get_arg(1))->value=get_int_val(SELF);
	return create_none();
}

def_dyn_fn(int_add) //add(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//int(self+operand)
	return create_int(get_int_val(SELF)+get_int_val(get_arg(1)));
}

def_dyn_fn(int_sub) //sub(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//int(self+operand)
	return create_int(get_int_val(SELF)-get_int_val(get_arg(1)));
}

def_dyn_fn(int_mul) //mul(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_int(get_int_val(SELF)*get_int_val(get_arg(1)));
}

def_dyn_fn(int_div) //div(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//int(self/operand)
	return create_int(get_int_val(SELF)/get_int_val(get_arg(1)));
}

def_dyn_fn(int_hash)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self"}),protect({INT}));
	#endif

	return create_int(get_int_val(SELF));
}

def_dyn_fn(int_eq)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_bool(get_int_val(SELF)==get_int_val(get_arg(1)));
}

def_dyn_fn(int_lt)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_bool(get_int_val(SELF)<get_int_val(get_arg(1)));
}

def_dyn_fn(int_le)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_bool(get_int_val(SELF)<=get_int_val(get_arg(1)));
}

def_dyn_fn(int_gt)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_bool(get_int_val(SELF)>get_int_val(get_arg(1)));
}

def_dyn_fn(int_ge)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_bool(get_int_val(SELF)>=get_int_val(get_arg(1)));
}

def_dyn_fn(int_and)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_int(get_int_val(SELF)&get_int_val(get_arg(1)));
}

def_dyn_fn(int_or)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_int(get_int_val(SELF)|get_int_val(get_arg(1)));
}

def_dyn_fn(int_xor)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_int(get_int_val(SELF)^get_int_val(get_arg(1)));
}
