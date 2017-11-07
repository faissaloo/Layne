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
#include <stdio.h>

#include "debug.h"
#include "dyn_objs.h"
#include "none_obj.h"
#include "bool_obj.h"
#include "str_obj.h"
#include "factory_obj.h"

struct method_list none_methods={
	3,
	{
		method_pair("new",none_new),
		method_pair("bool",none_bool),
		method_pair("str",none_str),
	}
};

struct dyn_obj* create_none()
{
	obj_setup_basic(NONE);
	return self;
}

def_dyn_fn(none_new)
{
	#ifdef DEBUG
		arg_guard(0,0,protect({}),protect({}));
	#endif
	return create_none();
}

def_dyn_fn(none_bool)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({BOOL}));
	#endif
	return create_bool(false);
}

def_dyn_fn(none_str)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({BOOL}));
	#endif
	return create_str(dyn_str_from_cstr("none"));
}
