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
#include "flt_obj.h"
#include "str_obj.h"
#include "bool_obj.h"
#include "none_obj.h"
#include "debug.h"
#include <stdio.h>
#include "factory_obj.h"

struct dyn_obj *global;

struct method_list int_methods={
	32,
	{
		method_pair("new",int_new),
		method_pair("add",int_add),
		method_pair("iadd",int_iadd),
		method_pair("sub",int_sub),
		method_pair("isub",int_isub),
		method_pair("mul",int_mul),
		method_pair("imul",int_imul),
		method_pair("div",int_div),
		method_pair("idiv",int_idiv),
		method_pair("int",int_int),
		method_pair("str",int_str),
		method_pair("hash",int_hash),
		method_pair("copy",int_copy),
		method_pair("eq",int_eq),
		method_pair("lt",int_lt),
		method_pair("le",int_le),
		method_pair("gt",int_gt),
		method_pair("ge",int_ge),
		method_pair("or",int_or),
		method_pair("ior",int_ior),
		method_pair("and",int_and),
		method_pair("iand",int_iand),
		method_pair("xor",int_xor),
		method_pair("ixor",int_ixor),
		method_pair("neg",int_neg),
		method_pair("ineg",int_ineg),
		method_pair("not",int_not),
		method_pair("inot",int_inot),
		method_pair("mod",int_mod),
		method_pair("imod",int_imod),
		method_pair("flt",int_flt),
		method_pair("bool",int_bool)
	}
};

struct dyn_obj *create_int_factory()
{
	fact_setup_basic(INT);
	int_factory=self;
	return self;
}

int get_int_val(struct dyn_obj *obj)
{
	return ((struct int_obj*)obj)->value;
}

struct dyn_obj* create_int(int value)
{
	obj_setup_basic(INT);
	((struct int_obj*)self)->value=value;
	return self;
}

def_dyn_fn(int_new)
{
	#ifdef DEBUG
		arg_guard(0,1,protect({"x"}),protect({TYPE}));
	#endif

	//Value to create the int from
	if (arg_count>0)
	{
		return call_method_noargs(args[0],"int");
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
	((struct int_obj*)args[1])->value=get_int_val(SELF);
	return create_none();
}

def_dyn_fn(int_add) //add(self,x)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//int(self+operand)
	return create_int(get_int_val(SELF)+get_int_val(args[1]));
}

def_dyn_fn(int_iadd)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//self+=operand
	((struct int_obj*)SELF)->value+=get_int_val(args[1]);
	return SELF;
}

def_dyn_fn(int_sub) //sub(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//int(self+operand)
	return create_int(get_int_val(SELF)-get_int_val(args[1]));
}

def_dyn_fn(int_isub)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//self-=operand
	((struct int_obj*)SELF)->value-=get_int_val(args[1]);
	return SELF;
}

def_dyn_fn(int_mul) //mul(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_int(get_int_val(SELF)*get_int_val(args[1]));
}

def_dyn_fn(int_imul)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//self*=operand
	((struct int_obj*)SELF)->value*=get_int_val(args[1]);
	return SELF;
}

def_dyn_fn(int_div) //div(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//int(self/operand)
	return create_int(get_int_val(SELF)/get_int_val(args[1]));
}

def_dyn_fn(int_idiv)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//self/=operand
	((struct int_obj*)SELF)->value/=get_int_val(args[1]);
	return SELF;
}

def_dyn_fn(int_mod) //div(self,operand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//int(self%operand)
	return create_int(get_int_val(SELF)%get_int_val(args[1]));
}

def_dyn_fn(int_imod)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//self%=operand
	((struct int_obj*)SELF)->value%=get_int_val(args[1]);
	return SELF;
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

	return create_bool(get_int_val(SELF)==get_int_val(args[1]));
}

def_dyn_fn(int_lt)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif
	return create_bool(get_int_val(SELF)<get_int_val(args[1]));
}

def_dyn_fn(int_le)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_bool(get_int_val(SELF)<=get_int_val(args[1]));
}

def_dyn_fn(int_gt)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_bool(get_int_val(SELF)>get_int_val(args[1]));
}

def_dyn_fn(int_ge)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_bool(get_int_val(SELF)>=get_int_val(args[1]));
}

def_dyn_fn(int_and)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_int(get_int_val(SELF)&get_int_val(args[1]));
}

def_dyn_fn(int_iand)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//self&=operand
	((struct int_obj*)SELF)->value&=get_int_val(args[1]);
	return SELF;
}

def_dyn_fn(int_or)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_int(get_int_val(SELF)|get_int_val(args[1]));
}

def_dyn_fn(int_ior)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//self|=operand
	((struct int_obj*)SELF)->value|=get_int_val(args[1]);
	return SELF;
}

def_dyn_fn(int_xor)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	return create_int(get_int_val(SELF)^get_int_val(args[1]));
}

def_dyn_fn(int_ixor)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","x"}),protect({INT,INT}));
	#endif

	//self^=operand
	((struct int_obj*)SELF)->value^=get_int_val(args[1]);
	return SELF;
}

def_dyn_fn(int_neg)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({INT}));
	#endif

	return create_int(-get_int_val(SELF));
}

def_dyn_fn(int_ineg)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({INT}));
	#endif

	//self=-self
	((struct int_obj*)SELF)->value=-((struct int_obj*)SELF)->value;
	return SELF;
}

def_dyn_fn(int_not)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({INT}));
	#endif

	return create_int(~get_int_val(SELF));
}

def_dyn_fn(int_inot)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({INT}));
	#endif

	//self=-self
	((struct int_obj*)SELF)->value=~((struct int_obj*)SELF)->value;
	return SELF;
}

def_dyn_fn(int_flt)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({INT}));
	#endif
	return create_flt((double)get_int_val(SELF));
}
