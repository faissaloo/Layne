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

#include "global_obj.h"
#include "none_obj.h"
#include "int_obj.h"
#include "flt_obj.h"
#include "bool_obj.h"
#include "str_obj.h"
#include "array_obj.h"
#include "func_obj.h"
#include "factory_obj.h"
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
	"array",
	"dict"
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
	sizeof(struct array_obj),
	0 //Unimplemented (dict)
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
	&array_methods,
	NULL //Unimplemented (dict)
	#include "obj_method_list_refs.txt"
};

struct method_list *type_factory_method_lists[]={
	&factory_type_methods,
	&factory_factory_methods,
	&factory_global_methods,
	&factory_none_methods,
	&factory_func_methods,
	&factory_int_methods,
	&factory_flt_methods,
	&factory_bool_methods,
	&factory_str_methods,
	&factory_array_methods,
	NULL //Unimplemented (dict)
	#include "obj_factory_method_list_refs.txt"
};

struct dyn_obj **type_factory_list[]={
	&type_factory,
	NULL, //Unimplemented (global)
	NULL, //Unimplemented (none)
	NULL, //Unimplemented (func)
	&int_factory,
	&flt_factory,
	&bool_factory,
	&str_factory,
	NULL, //Unimplemeted (array)
	NULL //Unimplemented (dict)
	#include "obj_type_factory_list_refs.txt"
};

struct dyn_obj **type_parent_list[]={
	&type_factory, //Type
	&type_factory, //Global
	&type_factory, //None
	&type_factory, //Func
	&type_factory, //Int
	&type_factory, //Flt
	&type_factory, //Bool
	&type_factory, //Str
	&type_factory, //Array
	&type_factory //Dict
	#include "obj_type_parent_refs.txt"
};

void bind_member(struct dyn_obj *obj, char member_name[], struct dyn_obj *member_obj)
{
	hash_table_add(obj->members,member_name,member_obj);
}

//Binds a function implemented in C to a function
void bind_method(struct dyn_obj *obj,char method_name[],struct dyn_obj* (*cfunc)(struct dyn_array*))
{
	bind_member(obj, method_name, create_function(cfunc));
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
	for (iter_t i=0;i<methods->count;i++)
	{
		bind_method(self,methods->method_pair[i].method_name,methods->method_pair[i].method);;
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
	to_return=GC_MALLOC(obj->size);
	memcpy(to_return,obj,obj->size);

	//Copy everything in the members hashtable
	//Think Prim's algorithm
	for (int i=0;i < obj->members->table_size;i++)
	{
		if ((*obj->members->items)[i]!=NULL)
		{
			for (int ii=0;ii < (*obj->members->items)[i]->filled;ii++)
			{
				struct dyn_obj *mmbr_to_copy;
				struct dyn_obj *copied_mmbr;
				struct hash_table_item *mmbr_hash_entry;

				mmbr_hash_entry=dyn_array_get((*obj->members->items)[i],ii);
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

	call_method(obj,"copy",dyn_array_from(1,(void *[]){to_return}));

	return to_return;
}

struct dyn_obj* call_function(struct dyn_obj *object_to_call, struct dyn_array *args)
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
	return ((struct func_obj*)func_to_call)->function(args);
}

struct dyn_obj* call_method(struct dyn_obj *obj, char method_name[], struct dyn_array *args)
{
	dyn_array_insert(args,0,obj);
	return call_function(get_member(obj,method_name),args);
}

bool is_child(struct dyn_obj *obj,enum type type_id)
{
	//Descend through the linked list of parents until we reach the TYPE object or a match
	for (struct dyn_obj *i=obj;!(i->cur_type==FACTORY && ((struct factory_obj*)i)->type_to_create==TYPE);i=get_member(i,"parent"))
	{
		if (type_id==obj->cur_type || (i->cur_type==FACTORY && ((struct factory_obj*)i)->type_to_create==type_id))
		{
			return true;
		}
	}
	return false;
}
