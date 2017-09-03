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
#include "func_obj.h"
#include "none_obj.h"
#include "debug.h"
#include <stdio.h>

struct method_list func_methods={0,{}};
struct method_list factory_func_methods={0,{}};

struct dyn_obj* create_function(struct dyn_obj* (*function)(struct dyn_array*))
{
	object_setup(FUNCTION);
	init_methods(self,&func_methods);

	bind_member(self,"call",self);
	((struct func_obj*)self)->function=function;

	struct dyn_obj *copy_func;
	copy_func=GC_MALLOC(sizeof(struct func_obj));
	copy_func->members=hash_table_create(&string_eq,&hash_string);
	copy_func->size=sizeof(struct func_obj);
	copy_func->cur_type=FUNCTION;
	((struct func_obj*)copy_func)->function=func_copy;
	bind_member(copy_func,"call",copy_func);
	bind_member(copy_func,"copy",copy_func);

	bind_member(self,"copy",copy_func);
	bind_member(self,"parent",*type_parent_list[self->cur_type]);
	return self;
}

def_dyn_fn(func_copy) //copy(self,new_obj)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","dest"}),protect({FUNCTION,FUNCTION}));
	#endif
	((struct func_obj*)get_arg(1))->function=((struct func_obj*)get_arg(0))->function;
	return create_none();
}
