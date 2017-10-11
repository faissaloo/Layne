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

#ifndef DICT_OBJ_H
#define DICT_OBJ_H

#include "dyn_objs.h"
#include "type_obj.h"
#include "hash_tables.h"

struct dyn_obj *create_dict_factory();
struct dyn_obj* create_dict(struct hash_table *ht);
struct dict_obj
{
	struct type_obj parent;
	struct hash_table *data;
};

extern struct method_list dict_methods;
extern struct method_list factory_dict_methods;
decl_dyn_fn(dict_new);
decl_dyn_fn(dict_get);
decl_dyn_fn(dict_set);

struct hash_table* dyn_obj_ht_from(size_t count,struct ht_item_init initial[]);
#endif
