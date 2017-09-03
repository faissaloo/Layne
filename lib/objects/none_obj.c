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
#include "none_obj.h"

struct method_list none_methods={
	1,
	{
		{"new",none_new}
	}
};

struct method_list factory_none_methods={
	1,
	{
		{"new",factory_none_new}
	}
};

struct dyn_obj* create_none()
{
	object_setup(NONE);
	init_methods(self,&none_methods);
	bind_member(self,"parent",type_factory);
	return self;
}

def_dyn_fn(none_new)
{
	return create_none();
}
