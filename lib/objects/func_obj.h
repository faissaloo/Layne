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

#ifndef FUNC_OBJ_H
#define FUNC_OBJ_H

#include "dyn_objs.h"
#include "type_obj.h"

struct dyn_obj* create_function(struct dyn_obj* (*function)(struct dyn_array*));
struct func_obj
{
	struct type_obj parent;
	struct dyn_obj* (*function)(struct dyn_array* args);
};

extern struct method_list func_methods;
extern struct method_list factory_func_methods;
decl_dyn_fn(func_copy);
#endif
