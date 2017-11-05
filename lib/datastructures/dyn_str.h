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

#ifndef DYN_STR_H
#define DYN_STR_H
#include <stddef.h>
#include <stdbool.h>
struct dyn_str
{
	char (*raw)[];
	size_t filled;
	size_t length;
};

struct dyn_str* dyn_str_create();
void dyn_str_cat_cstr(struct dyn_str *self,char str[]);
void dyn_str_cat(struct dyn_str *self,struct dyn_str *str);
struct dyn_str* dyn_str_cut(struct dyn_str *self, int start, int end);
struct dyn_str* dyn_str_get_char(struct dyn_str *self, int pos);
struct dyn_str* dyn_str_from_cstr(char str[]);
struct dyn_str* dyn_str_from_int(int n);
struct dyn_str* dyn_str_from_float(double n);
unsigned long int dyn_str_hash(struct dyn_str *self);
int dyn_str_to_int(struct dyn_str *self);
bool dyn_str_eq(struct dyn_str *a, struct dyn_str *b);
#endif
