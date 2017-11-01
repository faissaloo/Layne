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
#include "term_obj.h"
#include "factory_obj.h"

struct method_list term_methods={
	1,
	{
		method_pair("new",term_new)
	}
};

struct dyn_obj* create_term()
{
	obj_setup_basic(TERM);
	return self;
}

def_dyn_fn(term_new)
{
	return create_term();
}
