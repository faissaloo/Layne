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
#include <stdio.h>
#include "debug.h"
#include "dyn_objs.h"
#include "dyn_arrays.h"
#include "array_obj.h"
#include "str_obj.h"
#include "int_obj.h"
#include "bool_obj.h"
#include "factory_obj.h"
#include "lst_iter_obj.h"
#include "none_obj.h"

struct method_list array_methods={
	10,
	{
		method_pair("new",array_new),
		method_pair("set",array_set),
		method_pair("get",array_get),
		method_pair("ins",array_ins),
		method_pair("rm",array_rm),
		method_pair("len",array_len),
		method_pair("str",array_str),
		method_pair("hash",array_hash),
		method_pair("iter",array_iter),
		method_pair("copy",array_copy)
	}
};

struct dyn_obj* create_array(struct dyn_array *ary)
{
	obj_setup_basic(ARRAY);
	((struct array_obj*)self)->data=ary;
	return self;
}

def_dyn_fn(array_new)
{
	#ifdef DEBUG
		arg_guard(0,1,protect({"x"}),protect({TYPE}));
	#endif
	if (arg_count>0)
	{
		return call_method_noargs(args[0],"array");
	}
	return create_array(NULL);
}

def_dyn_fn(array_copy)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","dest"}),protect({ARRAY,ARRAY}));
	#endif
	for (iter_t i=0;i<((struct array_obj*)SELF)->data->filled;i++)
	{
		dyn_array_set(((struct array_obj*)args[1])->data,i,copy_obj(dyn_array_get(((struct array_obj*)SELF)->data,i), NULL));
	}
	//((struct array_obj*)SELF)->data;
	return kw_none;
}

//Compares every item in the array
def_dyn_fn(array_eq)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","b"}),protect({ARRAY,ARRAY}));
	#endif
	if (((struct array_obj*)SELF)->data->filled!=((struct array_obj*)args[1])->data->filled)
	{
		return kw_false;
	}

	for (iter_t i=0;i<((struct array_obj*)SELF)->data->filled;i++)
	{
		if (!get_bool_val(call_method(dyn_array_get(((struct array_obj*)SELF)->data,i),"eq",1,(struct dyn_obj*[]){dyn_array_get(((struct array_obj*)args[1])->data,i)})))
		{
			return kw_false;
		}
	}
	return kw_true;
}

//array[x]=y
def_dyn_fn(array_set)
{
	#ifdef DEBUG
		arg_guard(3,3,protect({"self","index","src"}),protect({ARRAY,INT,TYPE}));
	#endif
	dyn_array_set(((struct array_obj*)SELF)->data,get_int_val(args[1]),args[2]);
	return kw_none;
}
//Array[x]
def_dyn_fn(array_get)
{
	if (arg_count==2)
	{
		return (struct dyn_obj*)dyn_array_get(((struct array_obj*)SELF)->data,get_int_val(args[1]));
	}
	else
	{
		return create_array(dyn_array_cut(((struct array_obj*)SELF)->data,get_int_val(args[1]),get_int_val(args[2])));
	}
}

def_dyn_fn(array_ins)
{
	#ifdef DEBUG
		arg_guard(3,3,protect({"self","index","src"}),protect({ARRAY,INT,TYPE}));
	#endif
	dyn_array_insert(((struct array_obj*)SELF)->data,get_int_val(args[1]),args[2]);
	return kw_none;
}

def_dyn_fn(array_rm)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","index"}),protect({ARRAY,INT}));
	#endif
	dyn_array_remove(((struct array_obj*)SELF)->data,get_int_val(args[1]));
	return kw_none;
}

def_dyn_fn(array_len)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({ARRAY}));
	#endif
	return create_int(((struct array_obj*)SELF)->data->filled);
}

def_dyn_fn(array_str)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({ARRAY}));
	#endif
	struct dyn_str *new_str;
	new_str=dyn_str_from_cstr("[");

	if (((struct array_obj*)SELF)->data->filled>=1)
	{
		dyn_str_cat(new_str,get_str_val(call_method_noargs(dyn_array_get(((struct array_obj*)SELF)->data,0),"str")));
	}

	for (iter_t i=1;i<((struct array_obj*)SELF)->data->filled;i++)
	{
		dyn_str_cat_cstr(new_str,",");
		dyn_str_cat(new_str,get_str_val(call_method_noargs(dyn_array_get(((struct array_obj*)SELF)->data,i),"str")));
	}

	dyn_str_cat_cstr(new_str,"]");
	return create_str(new_str);
}

def_dyn_fn(array_hash)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({ARRAY}));
	#endif
	hash_t hash;
	hash=0;
	for (iter_t i=0;i<((struct array_obj*)SELF)->data->filled;i++)
	{
		hash*=31;
		hash+=get_int_val(call_method_noargs(dyn_array_get(((struct array_obj*)SELF)->data,i),"hash"));
	}
	return create_int(hash);
}

def_dyn_fn(array_iter)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({ARRAY}));
	#endif
	return create_lst_iter(((struct array_obj*)SELF)->data);
}
