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
#include "global_obj.h"
#include "type_obj.h"
#include "int_obj.h"
#include "str_obj.h"
#include "type_obj.h"
#include "flt_obj.h"
#include "bool_obj.h"
#include "none_obj.h"

struct dyn_obj *global;
struct dyn_obj *type_factory;
struct dyn_obj *int_factory;
struct dyn_obj *flt_factory;
struct dyn_obj *bool_factory;
struct dyn_obj *str_factory;

struct method_list global_methods={0,{}};
struct method_list factory_global_methods={0,{}};

struct dyn_obj* create_global()
{
	object_setup(GLOBAL);
	init_methods(self,&global_methods);
	global=self;
	type_factory=create_type_factory();
	bool_factory=create_bool_factory();
	int_factory=create_int_factory();
	flt_factory=create_flt_factory();
	str_factory=create_str_factory();
	bind_member(self,"type",type_factory);
	bind_member(self,"int",int_factory);
	bind_member(self,"flt",flt_factory);
	bind_member(self,"bool",bool_factory);
	bind_member(self,"str",str_factory);
	return self;
}
