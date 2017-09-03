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

#ifndef DYN_ARRAYS_H
#define DYN_ARRAYS_H
#include <stdlib.h>
struct dyn_array
{
	unsigned int filled;
	unsigned int length;
	void *(*items)[];
};

struct dyn_array* dyn_array_create();
struct dyn_array* dyn_array_from(size_t count,void *initial[]);
void dyn_array_append(struct dyn_array *self,void *data);
void dyn_array_insert(struct dyn_array *self, signed int index, void *data);
void dyn_array_remove(struct dyn_array *self, signed int index);
void *dyn_array_get(struct dyn_array *self, signed int index);
void dyn_array_set(struct dyn_array *self, signed int index,void *data);
struct dyn_array* dyn_array_cut(struct dyn_array *self, int start, int end);
#endif
