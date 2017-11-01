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

#ifndef LST_ITER_OBJ_H
#define LST_ITER_OBJ_H

#include "dyn_objs.h"
#include "type_obj.h"

struct dyn_obj *create_lst_iter_factory();

struct dyn_obj* create_lst_iter(struct dyn_array *to_iter);
struct lst_iter_obj
{
	struct type_obj parent;
	struct dyn_array *to_iter;
	iter_t index;
};

extern struct method_list lst_iter_methods;
decl_dyn_fn(lst_iter_new);
decl_dyn_fn(lst_iter_next);
#endif
