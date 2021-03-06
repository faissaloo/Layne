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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <gc.h>
#include "debug.h"
#include "dyn_objs.h"
#include "dyn_arrays.h"
#include "dyn_str.h"
#include "hash_tables.h"
#include "ll.h"

#include "global_obj.h"
#include "none_obj.h"
#include "int_obj.h"
#include "flt_obj.h"
#include "bool_obj.h"
#include "str_obj.h"
#include "lst_obj.h"
#include "dict_obj.h"
#include "func_obj.h"
#include "factory_obj.h"
#include "lst_iter_obj.h"
#include "str_iter_obj.h"
#include "term_obj.h"
#include "main.h"

const char *type_names[]={
	"type",
	"factory",
	"global",
	"none",
	"function",
	"int",
	"flt",
	"bool",
	"str",
	"lst",
	"dict",
	"term",
	"lst_iter",
	"str_iter"
	#include "obj_names.txt"
};

size_t type_sizes[]={
	sizeof(struct type_obj),
	sizeof(struct factory_obj),
	sizeof(struct global_obj),
	sizeof(struct none_obj),
	sizeof(struct func_obj),
	sizeof(struct int_obj),
	sizeof(struct flt_obj),
	sizeof(struct bool_obj),
	sizeof(struct str_obj),
	sizeof(struct lst_obj),
	sizeof(struct dict_obj),
	sizeof(struct term_obj),
	sizeof(struct lst_iter_obj),
	sizeof(struct str_iter_obj),
	#include "obj_sizes.txt"
};

//This only contains static methods
//'static' here meaning innate to the object and added in the standard way
struct method_list *type_method_lists[]={
	&type_methods,
	&factory_methods,
	&global_methods,
	&none_methods,
	&func_methods,
	&int_methods,
	&flt_methods,
	&bool_methods,
	&str_methods,
	&lst_methods,
	&dict_methods,
	&term_methods,
	&lst_iter_methods,
	&str_iter_methods
	#include "obj_method_list_refs.txt"
};

struct dyn_obj **type_factory_list[]={
	&type_factory,
	NULL, //Unimplemented (factory)
	NULL, //Unimplemented (global)
	NULL, //Unimplemented (none)
	NULL, //Unimplemented (func)
	&int_factory,
	&flt_factory,
	&bool_factory,
	&str_factory,
	NULL, //Unimplemeted (lst)
	NULL, //Unimplemented (dict)
	NULL, //Unimplemented (term)
	NULL, //Unimplemented (lst_iter)
	NULL //Unimplemented (str_iter)
	#include "obj_type_factory_list_refs.txt"
};

struct dyn_obj **type_parent_list[]={
	&type_factory, //Type
	&type_factory, //Factory
	&type_factory, //Global
	&type_factory, //None
	&type_factory, //Func
	&type_factory, //Int
	&type_factory, //Flt
	&type_factory, //Bool
	&type_factory, //Str
	&type_factory, //Lst
	&type_factory, //Dict
	&type_factory, //term
	&type_factory, //lst_iter
	&type_factory, //str_iter
	#include "obj_type_parent_refs.txt"
};

void bind_member(struct dyn_obj *obj, char member_name[], struct dyn_obj *member_obj)
{
	hash_table_add(obj->members,member_name,member_obj);
}

//Binds a function implemented in C to a function
void bind_method(struct dyn_obj *obj,char method_name[],struct dyn_obj* (*cfunc)(size_t arg_count, struct dyn_obj *args[]))
{
	bind_member(obj, method_name, create_function(cfunc));
}

//Returns one of the pooled boolean objects
struct dyn_obj* p_bool(int input)
{
	return input?kw_true:kw_false;
}

struct dyn_obj* get_member(struct dyn_obj *obj,char member_name[])
{
	void *member;
	member=hash_table_get(obj->members,member_name);
	#ifdef DEBUG
		if (member==NULL)
		{
			error(1, "No such member %s in instance of %s",member_name,type_names[obj->cur_type]);
		}
	#endif
	return member;
}

void init_methods(struct dyn_obj *self, const struct method_list *methods)
{
	for (iter_t i=1;i<=methods->count;i++)
	{
		bind_method(self,methods->method_pair[i-1].method_name,methods->method_pair[i-1].method);;
	}
}

void init_factory_methods(struct dyn_obj *self, const struct method_list *methods)
{
	for (iter_t i=1;i<=methods->count;i++)
	{
		bind_method(self,methods->method_pair[i-1].method_name,methods->method_pair[i-1].factory_method);;
	}
}

struct dyn_obj* copy_obj(struct dyn_obj *obj, struct hash_table *spanned_objs)
{
	//First layer of a call will have spanned_objs as a NULL
	if (spanned_objs==NULL)
	{
		spanned_objs=hash_table_create(&ptrs_eq,&hash_ptrs);
	}
	struct dyn_obj *to_return;
	//Lets create a get_type_size function later and use that
	to_return=GC_MALLOC(type_sizes[obj->cur_type]);
	memcpy(to_return,obj,type_sizes[obj->cur_type]);

	//Copy everything in the members hashtable
	//Think Prim's algorithm
	for (int i=0;i < obj->members->table_size;i++)
	{
		if ((*obj->members->items)[i]!=NULL)
		{
			for (struct ll_item *ii=(*obj->members->items)[i];ii->next!=NULL;ii=ii->next)
			{
				struct dyn_obj *mmbr_to_copy;
				struct dyn_obj *copied_mmbr;
				struct hash_table_item *mmbr_hash_entry;

				mmbr_hash_entry=ii->data;
				mmbr_to_copy=mmbr_hash_entry->data;
				if (hash_table_get(spanned_objs,mmbr_to_copy)==NULL)
				{
					//If the object hasn't yet been copied we need to copy it and add it
					hash_table_add(spanned_objs,mmbr_to_copy,mmbr_to_copy);
					copied_mmbr = copy_obj(mmbr_to_copy,spanned_objs);
					//It's gonna infinite loop cus we haven't added it before calling it
					hash_table_add(to_return->members,mmbr_hash_entry->key,copied_mmbr);
				}
				else
				{
					//If we've already copied this and the hash table is just referencing the same item twice we'll just reference it
					hash_table_add(to_return->members,mmbr_hash_entry->key,mmbr_to_copy);
				}
			}
		}
	}

	call_method(obj,"copy",1,(struct dyn_obj*[]){to_return});

	return to_return;
}

struct dyn_obj* call_function(struct dyn_obj *object_to_call, size_t arg_count, struct dyn_obj *args[])
{
	struct dyn_obj *func_to_call;
	//We start with the main object then descend to the most basic callable
	func_to_call=object_to_call;

	while (func_to_call->cur_type!=FUNCTION)
	{
		func_to_call=hash_table_get(func_to_call->members,"call");;
		#ifdef DEBUG
			if (func_to_call==NULL)
			{
				error(1, "Instance of %s is not callable",type_names[object_to_call->cur_type]);
			}
		#endif
	}
	return ((struct func_obj*)func_to_call)->function(arg_count,args);
}

struct dyn_obj* call_method(struct dyn_obj *obj, char method_name[], size_t arg_count, struct dyn_obj *args[])
{
	struct dyn_obj **new_args;
	new_args=GC_MALLOC(sizeof(struct dyn_obj*)*(arg_count+1));
	*new_args=obj;
	if (arg_count>0)
	{
		memcpy(&new_args[1],args,sizeof(struct dyn_obj*)*arg_count);
	}
	return call_function(get_member(obj,method_name),arg_count+1,new_args);
}

//Calls a method which only passes self to the object
struct dyn_obj* call_method_noargs(struct dyn_obj *obj, char method_name[])
{
	return call_function(get_member(obj,method_name),1,(struct dyn_obj*[]){obj});
}

//Calls a method which only passes self to the object if it exists
struct dyn_obj* call_method_noargs_silent(struct dyn_obj *obj, char method_name[])
{
	struct dyn_obj *callable;
	callable=hash_table_get(obj->members,method_name);
	if (callable)
	{
		return call_function(callable,1,(struct dyn_obj*[]){obj});
	}
	else
	{
		return kw_none;
	}
}

//Registers the destructor for the object
void reg_destructor(struct dyn_obj *obj)
{
	GC_register_finalizer(obj,(GC_finalization_proc)call_method_noargs_silent,"del",0,0);
}

bool is_child(struct dyn_obj *obj,enum type type_id)
{
	if (type_id==TYPE)
	{
		return true;
	}
	struct dyn_obj *i=obj;
	//Deal with the first item being an actual object rather than a factory
	if (type_id!=FACTORY)
	{
		if (obj->cur_type==type_id)
		{
			return true;
		}
		else
		{
			i=*type_parent_list[obj->cur_type];
		}
	}
	//At this point the object in i WILL be a factory, we can go jump through the table til we find a match or the TYPE object
	for (;((struct factory_obj*)i)->type_to_create!=TYPE;i=*type_parent_list[obj->cur_type])
	{
		if (((struct factory_obj*)i)->type_to_create==type_id)
		{
			return true;
		}
	}
	return false;
}
