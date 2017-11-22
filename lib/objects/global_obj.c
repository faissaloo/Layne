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
#include <errno.h>
#include <string.h>

#include "dyn_objs.h"
#include "global_obj.h"
#include "type_obj.h"
#include "int_obj.h"
#include "str_obj.h"
#include "type_obj.h"
#include "flt_obj.h"
#include "bool_obj.h"
#include "none_obj.h"
#include "dyn_str.h"
#include "debug.h"

struct dyn_obj *kw_global;
struct dyn_obj *kw_true;
struct dyn_obj *kw_false;
struct dyn_obj *kw_none;
struct dyn_obj *type_factory;
struct dyn_obj *int_factory;
struct dyn_obj *flt_factory;
struct dyn_obj *bool_factory;
struct dyn_obj *str_factory;

struct method_list global_methods={2,
	{
		method_pair("print",global_print),
		method_pair("input",global_input)
	}
};

struct dyn_obj* create_global()
{
	obj_setup_basic(GLOBAL);
	kw_global=self;
	create_type_factory();
	create_bool_factory();
	create_int_factory();
	create_flt_factory();
	create_str_factory();
	bind_member(self,"type",type_factory);
	bind_member(self,"int",int_factory);
	bind_member(self,"flt",flt_factory);
	bind_member(self,"bool",bool_factory);
	bind_member(self,"str",str_factory);
	kw_true=create_bool(1);
	kw_false=create_bool(0);
	kw_none=create_none();
	return self;
}

def_dyn_fn(global_print)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","str"}),protect({GLOBAL,STR}));
	#endif
	printf("%s",*get_str_val(args[1])->raw);
	return kw_none;
}

def_dyn_fn(global_input)
{
	#ifdef DEBUG
		arg_guard(2,2,protect({"self","str"}),protect({GLOBAL,STR}));
	#endif
	printf("%s",*get_str_val(args[1])->raw);
	char buff[1024];
	struct dyn_str *new_str;
	new_str=dyn_str_create();
	while (fgets(buff,sizeof(buff),stdin)!=NULL){
		dyn_str_cat_cstr(new_str,buff);
		if ((*new_str->raw)[new_str->filled-1]=='\n')
		{
			break;
		}
	}
	new_str->filled--; //Cut out the newline
	return create_str(new_str);
}
