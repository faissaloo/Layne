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
#include "lst_obj.h"
#include "str_obj.h"
#include "int_obj.h"
#include "bool_obj.h"
#include "factory_obj.h"
#include "lst_iter_obj.h"
#include "none_obj.h"

struct method_list lst_methods={
	10,
	{
		method_pair("new",lst_new),
		method_pair("set",lst_set),
		method_pair("get",lst_get),
		method_pair("ins",lst_ins),
		method_pair("rm",lst_rm),
		method_pair("len",lst_len),
		method_pair("str",lst_str),
		method_pair("hash",lst_hash),
		method_pair("iter",lst_iter),
		method_pair("copy",lst_copy)
	}
};

struct dyn_obj* create_lst(struct dyn_array *ary)
{
	obj_setup_basic(LST);
	((struct lst_obj*)self)->data=ary;
	return self;
}

def_dyn_fn(lst_new)
{
	#ifdef DEBUG
		arg_guard(0,1,protect({"x"}),protect({TYPE}));
	#endif
	if (arg_count>0)
	{
		return call_method_noargs(args[0],"lst");
	}
	return create_lst(NULL);
}

def_dyn_fn(lst_copy)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","dest"}),protect({LST,LST}));
	#endif
	for (iter_t i=0;i<((struct lst_obj*)SELF)->data->filled;i++)
	{
		dyn_array_set(((struct lst_obj*)args[1])->data,i,copy_obj(dyn_array_get(((struct lst_obj*)SELF)->data,i), NULL));
	}
	//((struct lst_obj*)SELF)->data;
	return kw_none;
}

//Compares every item in the lst
def_dyn_fn(lst_eq)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","b"}),protect({LST,LST}));
	#endif
	if (((struct lst_obj*)SELF)->data->filled!=((struct lst_obj*)args[1])->data->filled)
	{
		return kw_false;
	}

	for (iter_t i=0;i<((struct lst_obj*)SELF)->data->filled;i++)
	{
		if (!get_bool_val(call_method(dyn_array_get(((struct lst_obj*)SELF)->data,i),"eq",1,(struct dyn_obj*[]){dyn_array_get(((struct lst_obj*)args[1])->data,i)})))
		{
			return kw_false;
		}
	}
	return kw_true;
}

//lst[x]=y
def_dyn_fn(lst_set)
{
	#ifdef DEBUG
		arg_guard(3,3,protect({"self","index","src"}),protect({LST,INT,TYPE}));
	#endif
	dyn_array_set(((struct lst_obj*)SELF)->data,get_int_val(args[1]),args[2]);
	return kw_none;
}
//lst[x]
def_dyn_fn(lst_get)
{
	if (arg_count==2)
	{
		return (struct dyn_obj*)dyn_array_get(((struct lst_obj*)SELF)->data,get_int_val(args[1]));
	}
	else
	{
		return create_lst(dyn_array_cut(((struct lst_obj*)SELF)->data,get_int_val(args[1]),get_int_val(args[2])));
	}
}

def_dyn_fn(lst_ins)
{
	#ifdef DEBUG
		arg_guard(3,3,protect({"self","index","src"}),protect({LST,INT,TYPE}));
	#endif
	dyn_array_insert(((struct lst_obj*)SELF)->data,get_int_val(args[1]),args[2]);
	return kw_none;
}

def_dyn_fn(lst_rm)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","index"}),protect({lst,INT}));
	#endif
	dyn_array_remove(((struct lst_obj*)SELF)->data,get_int_val(args[1]));
	return kw_none;
}

def_dyn_fn(lst_len)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({LST}));
	#endif
	return create_int(((struct lst_obj*)SELF)->data->filled);
}

def_dyn_fn(lst_str)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({LST}));
	#endif
	struct dyn_str *new_str;
	new_str=dyn_str_from_cstr("[");

	if (((struct lst_obj*)SELF)->data->filled>=1)
	{
		dyn_str_cat(new_str,get_str_val(call_method_noargs(dyn_array_get(((struct lst_obj*)SELF)->data,0),"str")));
	}

	for (iter_t i=1;i<((struct lst_obj*)SELF)->data->filled;i++)
	{
		dyn_str_cat_cstr(new_str,",");
		dyn_str_cat(new_str,get_str_val(call_method_noargs(dyn_array_get(((struct lst_obj*)SELF)->data,i),"str")));
	}

	dyn_str_cat_cstr(new_str,"]");
	return create_str(new_str);
}

def_dyn_fn(lst_hash)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({lst}));
	#endif
	hash_t hash;
	hash=0;
	for (iter_t i=0;i<((struct lst_obj*)SELF)->data->filled;i++)
	{
		hash*=31;
		hash+=get_int_val(call_method_noargs(dyn_array_get(((struct lst_obj*)SELF)->data,i),"hash"));
	}
	return create_int(hash);
}

def_dyn_fn(lst_iter)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({LST}));
	#endif
	return create_lst_iter(((struct lst_obj*)SELF)->data);
}
