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
#include <math.h>
#include <stdio.h>

#include "dyn_objs.h"
#include "flt_obj.h"
#include "int_obj.h"
#include "str_obj.h"
#include "bool_obj.h"
#include "none_obj.h"
#include "debug.h"
#include "factory_obj.h"

struct method_list factory_flt_methods={
	16,
	{
		{"new",factory_flt_new},
		{"add",factory_flt_add},
		{"sub",factory_flt_sub},
		{"mul",factory_flt_mul},
		{"div",factory_flt_div},
		{"mod",factory_flt_mod},
		{"int",factory_flt_int},
		{"flt",factory_flt_flt},
		{"str",factory_flt_str},
		{"hash",factory_flt_hash},
		{"copy",factory_flt_copy},
		{"eq",factory_flt_eq},
		{"lt",factory_flt_lt},
		{"le",factory_flt_le},
		{"gt",factory_flt_gt},
		{"ge",factory_flt_ge}
	}
};

struct dyn_obj *create_flt_factory()
{
	object_setup(FACTORY);
	((struct factory_obj*)self)->type_to_create=FLT;
	//init_methods(self,&factory_flt_methods);

	bind_member(self,"parent",*type_parent_list[self->cur_type]);
	inherit_factory_setup();
	return self;
}

double get_flt_val(struct dyn_obj *obj)
{
	return ((struct flt_obj*)obj)->value;
}

struct method_list flt_methods={
	16,
	{
		{"new",flt_new},
		{"add",flt_add},
		{"sub",flt_sub},
		{"mul",flt_mul},
		{"div",flt_div},
		{"mod",flt_mod},
		{"int",flt_int},
		{"flt",flt_flt},
		{"str",flt_str},
		{"hash",flt_hash},
		{"copy",flt_copy},
		{"eq",flt_eq},
		{"lt",flt_lt},
		{"le",flt_le},
		{"gt",flt_gt},
		{"ge",flt_ge}
	}
};

struct dyn_obj* create_flt(double value)
{
	object_setup(FLT);
	init_methods(self,&flt_methods);

	bind_member(self,"parent",get_member(global,"type"));
	((struct flt_obj*)self)->value=value;
	return self;
}

def_dyn_fn(flt_new)
{
	#ifdef DEBUG
		arg_guard(1,2,protect({"self","x"}),protect({FLT,TYPE}));
	#endif

	//Value to create the int from
	if (args->filled>1)
	{
		return call_method(get_arg(1),"flt",dyn_array_create());
	}
	return create_flt(0);
}

def_dyn_fn(flt_int)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({FLT}));
	#endif

	return create_int(get_flt_val(SELF));
}

def_dyn_fn(flt_flt)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({FLT}));
	#endif

	return create_flt(get_flt_val(SELF));
}

def_dyn_fn(flt_str)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({FLT}));
	#endif

	return create_str(dyn_str_from_int(get_flt_val(SELF)));
}

def_dyn_fn(flt_bool)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({FLT}));
	#endif

	return create_bool(get_flt_val(SELF));
}

//Normal operations will create new objects, while in place stuff like += will modify an existing object
def_dyn_fn(flt_copy) //copy(self,new_obj)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","dest"}),protect({INT,INT}));
	#endif

	//new_obj.function=self.function
	((struct flt_obj*)get_arg(1))->value=get_flt_val(SELF);
	return create_none();
}

def_dyn_fn(flt_add) //add(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif

	//flt(self+x)
	return create_flt(get_flt_val(SELF)+get_flt_val(get_arg(1)));
}

def_dyn_fn(flt_sub) //sub(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self-x)
	return create_flt(get_flt_val(SELF)-get_flt_val(get_arg(1)));
}

def_dyn_fn(flt_mul) //mul(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self*x)
	return create_flt(get_flt_val(SELF)*get_flt_val(get_arg(1)));
}

def_dyn_fn(flt_div) //div(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self/x)
	return create_flt(get_flt_val(SELF)/get_flt_val(get_arg(1)));
}

def_dyn_fn(flt_mod) //mod(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self%operand)
	return create_flt(fmod(get_flt_val(SELF),get_flt_val(get_arg(1))));
}

def_dyn_fn(flt_hash)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self"}),protect({FLT}));
	#endif
	//We need a way to compute the hash
	return create_int(get_flt_val(SELF));
}

def_dyn_fn(flt_eq) //eq(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self==x)
	return create_bool(get_flt_val(SELF)==get_flt_val(get_arg(1)));
}

def_dyn_fn(flt_lt) //lt(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self<x)
	return create_bool(get_flt_val(SELF)<get_flt_val(get_arg(1)));
}

def_dyn_fn(flt_le) //le(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//bool(self<=x)
	return create_bool(get_flt_val(SELF)<=get_flt_val(get_arg(1)));
}

def_dyn_fn(flt_gt) //qt(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//bool(self>x)
	return create_bool(get_flt_val(SELF)>get_flt_val(get_arg(1)));
}

def_dyn_fn(flt_ge) //ge(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//bool(self>=x)
	return create_bool(get_flt_val(SELF)>=get_flt_val(get_arg(1)));
}