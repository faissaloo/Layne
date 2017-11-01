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

#ifndef INT_OBJ_H
#define INT_OBJ_H

#include "dyn_objs.h"
#include "type_obj.h"

struct dyn_obj *create_int_factory();
int get_int_val(struct dyn_obj *obj);

struct dyn_obj* create_int(int value);
struct int_obj
{
	struct type_obj parent;
	signed int value;
};

extern struct method_list int_methods;
decl_dyn_fn(int_new);

decl_dyn_fn(int_int);
decl_dyn_fn(int_str);
decl_dyn_fn(int_bool);

decl_dyn_fn(int_add);
decl_dyn_fn(int_iadd);
decl_dyn_fn(int_sub);
decl_dyn_fn(int_isub);
decl_dyn_fn(int_mul);
decl_dyn_fn(int_imul);
decl_dyn_fn(int_div);
decl_dyn_fn(int_idiv);
decl_dyn_fn(int_mod);
decl_dyn_fn(int_imod);

decl_dyn_fn(int_eq);
decl_dyn_fn(int_lt);
decl_dyn_fn(int_le);
decl_dyn_fn(int_gt);
decl_dyn_fn(int_ge);

decl_dyn_fn(int_or);
decl_dyn_fn(int_ior);
decl_dyn_fn(int_and);
decl_dyn_fn(int_iand);
decl_dyn_fn(int_xor);
decl_dyn_fn(int_ixor);

decl_dyn_fn(int_copy);
decl_dyn_fn(int_hash);

decl_dyn_fn(int_neg);
decl_dyn_fn(int_ineg);
decl_dyn_fn(int_not);
decl_dyn_fn(int_inot);
#endif
