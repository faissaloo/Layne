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

#ifndef BOOL_OBJ_H
#define BOOL_OBJ_H

#include <stdbool.h>

#include "dyn_objs.h"
#include "type_obj.h"

struct dyn_obj *create_bool_factory();
bool get_bool_val(struct dyn_obj *obj);

struct dyn_obj* create_bool(bool value);
struct bool_obj
{
	struct type_obj parent;
	bool value;
};

extern struct method_list bool_methods;
extern struct method_list factory_bool_methods;
decl_dyn_fn(bool_new);
decl_dyn_fn(bool_bool);
decl_dyn_fn(bool_int);
decl_dyn_fn(bool_str);
decl_dyn_fn(bool_or); //||
decl_dyn_fn(bool_or); //|
decl_dyn_fn(bool_and); //&&
decl_dyn_fn(bool_and); //&
decl_dyn_fn(bool_xor); //^
#endif
