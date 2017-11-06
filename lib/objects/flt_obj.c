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

struct dyn_obj *create_flt_factory()
{
	fact_setup_basic(FLT);
	flt_factory=self;
	return self;
}

double get_flt_val(struct dyn_obj *obj)
{
	return ((struct flt_obj*)obj)->value;
}

struct method_list flt_methods={
	21,
	{
		method_pair("new",flt_new),
		method_pair("add",flt_add),
		method_pair("sub",flt_sub),
		method_pair("mul",flt_mul),
		method_pair("div",flt_div),
		method_pair("mod",flt_mod),
		method_pair("add",flt_add),
		method_pair("isub",flt_isub),
		method_pair("imul",flt_imul),
		method_pair("idiv",flt_idiv),
		method_pair("imod",flt_imod),
		method_pair("int",flt_int),
		method_pair("bool",flt_bool),
		method_pair("flt",flt_flt),
		method_pair("str",flt_str),
		method_pair("hash",flt_hash),
		method_pair("copy",flt_copy),
		method_pair("eq",flt_eq),
		method_pair("lt",flt_lt),
		method_pair("le",flt_le),
		method_pair("gt",flt_gt),
		method_pair("ge",flt_ge)
	}
};

struct dyn_obj* create_flt(double value)
{
	obj_setup_basic(FLT);
	((struct flt_obj*)self)->value=value;
	return self;
}

def_dyn_fn(flt_new)
{
	#ifdef DEBUG
		arg_guard(1,2,protect({"self","x"}),protect({FLT,TYPE}));
	#endif

	//Value to create the flt from
	if (arg_count>1)
	{
		return call_method_noargs(args[1],"flt");
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

	return create_str(dyn_str_from_float(get_flt_val(SELF)));
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
	((struct flt_obj*)args[1])->value=get_flt_val(SELF);
	return create_none();
}

def_dyn_fn(flt_add) //add(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif

	//flt(self+x)
	return create_flt(get_flt_val(SELF)+get_flt_val(args[1]));
}

def_dyn_fn(flt_sub) //sub(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self-x)
	return create_flt(get_flt_val(SELF)-get_flt_val(args[1]));
}

def_dyn_fn(flt_mul) //mul(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self*x)
	return create_flt(get_flt_val(SELF)*get_flt_val(args[1]));
}

def_dyn_fn(flt_div) //div(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self/x)
	return create_flt(get_flt_val(SELF)/get_flt_val(args[1]));
}

def_dyn_fn(flt_mod) //mod(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self%operand)
	return create_flt(fmod(get_flt_val(SELF),get_flt_val(args[1])));
}

def_dyn_fn(flt_iadd) //add(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif

	//flt(self+x)
	((struct flt_obj*)SELF)->value+=get_flt_val(args[1]);
	return SELF;
}

def_dyn_fn(flt_isub) //sub(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self-x)
	((struct flt_obj*)SELF)->value-=get_flt_val(args[1]);
	return SELF;
}

def_dyn_fn(flt_imul) //mul(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self*x)
	((struct flt_obj*)SELF)->value*=get_flt_val(args[1]);
	return SELF;
}

def_dyn_fn(flt_idiv) //div(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self/x)
	((struct flt_obj*)SELF)->value=((struct flt_obj*)SELF)->value/get_flt_val(args[1]);
	return SELF;
}

def_dyn_fn(flt_imod) //mod(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self%operand)
	((struct flt_obj*)SELF)->value=fmod(((struct flt_obj*)SELF)->value,get_flt_val(args[1]));
	return SELF;
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
	return create_bool(get_flt_val(SELF)==get_flt_val(args[1]));
}

def_dyn_fn(flt_lt) //lt(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//flt(self<x)
	return create_bool(get_flt_val(SELF)<get_flt_val(args[1]));
}

def_dyn_fn(flt_le) //le(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//bool(self<=x)
	return create_bool(get_flt_val(SELF)<=get_flt_val(args[1]));
}

def_dyn_fn(flt_gt) //qt(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//bool(self>x)
	return create_bool(get_flt_val(SELF)>get_flt_val(args[1]));
}

def_dyn_fn(flt_ge) //ge(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({FLT,FLT}));
	#endif
	//bool(self>=x)
	return create_bool(get_flt_val(SELF)>=get_flt_val(args[1]));
}
