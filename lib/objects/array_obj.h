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

#ifndef ARRAY_OBJ_H
#define ARRAY_OBJ_H

#include "dyn_objs.h"
#include "type_obj.h"
#include "dyn_arrays.h"

struct dyn_obj *create_array_factory();
struct dyn_obj* create_array(struct dyn_array *ary);
struct array_obj
{
	struct type_obj parent;
	struct dyn_array *data;
};

extern struct method_list array_methods;
extern struct method_list factory_array_methods;
decl_dyn_fn(array_new);
decl_dyn_fn(array_cut);
decl_dyn_fn(array_set);
decl_dyn_fn(array_ins);
decl_dyn_fn(array_del);
decl_dyn_fn(array_get);
decl_dyn_fn(array_eq);
#endif
