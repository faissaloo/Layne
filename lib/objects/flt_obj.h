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

#ifndef FLT_OBJ_H
#define FLT_OBJ_H

#include "dyn_objs.h"
#include "type_obj.h"

struct dyn_obj* create_flt_factory();
//Add a macro to switch from double to float for speed
struct dyn_obj* create_flt(double value);
struct flt_obj
{
	struct type_obj parent;
	double value;
};

extern struct method_list flt_methods;
extern struct method_list factory_flt_methods;
decl_dyn_fn(flt_new);

decl_dyn_fn(flt_int);
decl_dyn_fn(flt_flt);
decl_dyn_fn(flt_str);
decl_dyn_fn(flt_bool);

decl_dyn_fn(flt_add);
decl_dyn_fn(flt_sub);
decl_dyn_fn(flt_mul);
decl_dyn_fn(flt_div);
decl_dyn_fn(flt_mod);

decl_dyn_fn(flt_eq);
decl_dyn_fn(flt_lt);
decl_dyn_fn(flt_le);
decl_dyn_fn(flt_gt);
decl_dyn_fn(flt_ge);

decl_dyn_fn(flt_copy);
decl_dyn_fn(flt_hash);
#endif
