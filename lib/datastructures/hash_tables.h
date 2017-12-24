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

#ifndef HASH_TABLES_H
#define HASH_TABLES_H

#include "dyn_arrays.h"
#include "../utils.h"

struct ht_item_init
{
	void *data;
	void *key;
};

struct hash_table_item
{
	void *data;
	void *key;
	unsigned int key_hash;
};

struct hash_table
{
	//We're using an array of linked lists of hash_table_items here in case of hash collisions
	struct ll_item *(*items)[];
	//Old version, without hash collision handling
	//struct hash_table_item *(*items)[];
	unsigned int table_size;
	unsigned int table_filled;

	int (*cmp_func)(void *, void *);
	hash_t (*hash_func)(void *);
};

int ptrs_eq(void *a,void *b);
hash_t hash_ptrs(void *in_ptr);
int string_eq(void *a,void *b);
hash_t hash_string(void *in_str);
struct hash_table* hash_table_create(int (*cmp_func)(void *,void *),hash_t (*hash_func)(void *));
struct hash_table* hash_table_from(int (*cmp_func)(void *,void *), hash_t (*hash_func)(void *), size_t count,struct ht_item_init initial[]);
void hash_table_grow(struct hash_table *self);
void hash_table_shrink(struct hash_table *self);
void hash_table_remove(struct hash_table *self, void *key);
void hash_table_add(struct hash_table *self, void *key, void *data);
void *hash_table_get(struct hash_table *self, void *key);

#endif
