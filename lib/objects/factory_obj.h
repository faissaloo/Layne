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

#ifndef FACTORY_OBJ_H
#define FACTORY_OBJ_H
#include "dyn_objs.h"
#include "type_obj.h"

struct factory_obj
{
	struct type_obj parent;
	enum type type_to_create;
};

extern struct method_list factory_methods;
extern struct method_list factory_factory_methods;
#endif
