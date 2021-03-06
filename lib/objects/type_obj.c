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
#include "type_obj.h"
#include "none_obj.h"
#include "factory_obj.h"

struct method_list type_methods={
	2,
	{
		{"new",type_new},
		{"copy",type_copy}
	}
};

struct method_list factory_type_methods={
	2,
	{
		{"new",factory_type_new},
		{"copy",factory_type_copy}
	}
};

struct dyn_obj *create_type_factory()
{
	struct dyn_obj *self;
	self=GC_MALLOC(type_sizes[FACTORY]);
	self->members=hash_table_create(&string_eq,&hash_string);
	self->cur_type=FACTORY;
	type_factory=self;
	bind_member(self,"parent",*type_parent_list[FACTORY]);
	init_methods(self,&factory_type_methods);

	//global object can't give itself a parent that doesn't yet exist
	//until we have a cleaner solution we'll tell it what its parent is
	//We're kinda jerks
	bind_member(kw_global,"parent",self);
	bind_member(kw_global,"type",self);

	return self;
}

struct dyn_obj* create_type()
{
	obj_setup_basic(TYPE);
	return self;
}

def_dyn_fn(type_new)
{
	return create_type();
}

def_dyn_fn(type_copy)
{
	return kw_none;
}
