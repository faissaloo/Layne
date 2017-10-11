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
#include "dict_obj.h"
#include "str_obj.h"
#include "int_obj.h"
#include "bool_obj.h"
#include "factory_obj.h"
#include "none_obj.h"

struct method_list factory_dict_methods={
	3,
	{
		{"new",factory_dict_new},
		{"set",factory_dict_set},
		{"get",factory_dict_get}
	}
};

struct method_list dict_methods={
	3,
	{
		{"new",dict_new},
		{"set",dict_set},
		{"get",dict_get}
	}
};

int dyn_objs_eq(void *a,void *b)
{
	return get_bool_val(call_method((struct dyn_obj*)a,"eq",dyn_array_from(1,(void *[]){b})));
}

hash_t dyn_objs_hash(void *in_obj)
{
	return get_int_val(call_method_noargs((struct dyn_obj*)in_obj,"hash"));
}

struct hash_table* dyn_obj_ht_from(size_t count,struct ht_item_init initial[])
{
	return hash_table_from(dyn_objs_eq, dyn_objs_hash, count, initial);
}

struct dyn_obj* create_dict(struct hash_table *ht)
{
	object_setup(DICT);
	init_methods(self,&dict_methods);
	if (ht==NULL)
	{
		((struct dict_obj*)self)->data=hash_table_create(dyn_objs_eq,dyn_objs_hash);
	}
	else
	{
		((struct dict_obj*)self)->data=ht;
	}
	bind_member(self,"parent",type_factory);
	return self;
}

def_dyn_fn(dict_new)
{
	if (args->filled>1)
	{
		return call_method_noargs(get_arg(1),"dict");
	}
	return create_dict(NULL);
}

def_dyn_fn(dict_set)
{
	hash_table_add(((struct dict_obj*)SELF)->data,get_arg(1),get_arg(2));
	return create_none();
}

def_dyn_fn(dict_get)
{
	return (struct dyn_obj*)hash_table_get(((struct dict_obj*)SELF)->data,get_arg(1));
}
