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
#include "lst_iter_obj.h"
#include "term_obj.h"
#include "str_obj.h"
#include "bool_obj.h"
#include "none_obj.h"
#include "debug.h"
#include <stdio.h>
#include "factory_obj.h"

struct dyn_obj *global;

struct method_list lst_iter_methods={
	2,
	{
		method_pair("new",lst_iter_new),
		method_pair("next",lst_iter_next)
	}
};
struct dyn_obj *create_lst_iter_factory()
{
	fact_setup_basic(LST_ITER);
	return self;
}

struct dyn_obj* create_lst_iter(struct dyn_array *to_iter)
{
	obj_setup_basic(LST_ITER);
	((struct lst_iter_obj*)self)->to_iter=to_iter;
	((struct lst_iter_obj*)self)->index=0;
	return self;
}

def_dyn_fn(lst_iter_new)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"list"}),protect({LST_ITER}));
	#endif

	//Value to create the int from
	return call_method_noargs(args[0],"iter");
}

def_dyn_fn(lst_iter_next)
{
	#ifdef DEBUG
		arg_guard(1,1,protect({"self"}),protect({LST_ITER}));
	#endif

	if (((struct lst_iter_obj*)SELF)->to_iter->filled>((struct lst_iter_obj*)SELF)->index)
	{
		return dyn_array_get(((struct lst_iter_obj*)SELF)->to_iter,((struct lst_iter_obj*)SELF)->index++);
	}
	else
	{
		return create_term();
	}
}
