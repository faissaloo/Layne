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
#include "str_iter_obj.h"
#include "term_obj.h"
#include "str_obj.h"
#include "bool_obj.h"
#include "none_obj.h"
#include "debug.h"
#include <stdio.h>
#include "factory_obj.h"

struct dyn_obj *global;

struct method_list str_iter_methods={
	2,
	{
		method_pair("new",str_iter_new),
		method_pair("next",str_iter_next)
	}
};
struct dyn_obj *create_str_iter_factory()
{
	fact_setup_basic(STR_ITER);
	return self;
}

struct dyn_obj* create_str_iter(struct dyn_str *to_iter)
{
	obj_setup_basic(STR_ITER);
	((struct str_iter_obj*)self)->to_iter=to_iter;
	((struct str_iter_obj*)self)->index=0;
	return self;
}

def_dyn_fn(str_iter_new)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"str"}),protect({STR_ITER}));
	#endif

	//Value to create the int from
	return call_method_noargs(args[0],"iter");
}

def_dyn_fn(str_iter_next)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({STR_ITER}));
	#endif

	if (((struct str_iter_obj*)SELF)->to_iter->filled>((struct str_iter_obj*)SELF)->index)
	{
		return create_str(dyn_str_get_char(((struct str_iter_obj*)SELF)->to_iter,((struct str_iter_obj*)SELF)->index++));//dyn_array_get(((struct str_iter_obj*)SELF)->to_iter,((struct str_iter_obj*)SELF)->index++);
	}
	else
	{
		return create_term();
	}
}
