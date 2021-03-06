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

#ifndef TYPE_OBJ_H
#define TYPE_OBJ_H

#include "dyn_objs.h"

struct type_obj
{
	struct dyn_obj parent;
};

struct dyn_obj* create_type();
decl_dyn_fn(type_new);
decl_dyn_fn(type_copy);
struct dyn_obj* create_type_factory();

extern struct method_list type_methods;
extern struct method_list factory_type_methods;
#endif
