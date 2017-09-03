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

#ifndef STR_OBJ_H
#define STR_OBJ_H

#include <stdbool.h>

#include "dyn_str.h"
#include "dyn_objs.h"
#include "type_obj.h"

struct dyn_obj *create_str_factory();
struct dyn_obj* create_str(struct dyn_str *data);
struct str_obj
{
	struct type_obj parent;
	struct dyn_str *data;
};

extern struct method_list str_methods;
extern struct method_list factory_str_methods;
decl_dyn_fn(str_new);
decl_dyn_fn(str_add);
decl_dyn_fn(str_cut);
decl_dyn_fn(str_hash);
decl_dyn_fn(str_int);
decl_dyn_fn(str_str);
decl_dyn_fn(str_get);
decl_dyn_fn(str_eq);

struct dyn_str* get_str_val(struct dyn_obj *obj);
#endif
