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

#include "dyn_arrays.h"
#include "hash_tables.h"
#include <string.h>
#include <gc.h>
//http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=(CategoryAlgorithmNotes)

//How much to expand by each time
#define block_size 128

//The main string hashing function, improve later
int ptrs_eq(void *a,void *b)
{
	return a==b;
}

hash_t hash_ptrs(void *in_ptr)
{
	return (hash_t)in_ptr;
}

int string_eq(void *a, void *b)
{
	return !strcmp((char*)a,(char*)b);
}

hash_t hash_string(void *in_str)
{
	hash_t hash=0;
	for (iter_t i=0;((char*)in_str)[i]!='\0';i++)
	{
		hash *= 31;
		hash += ((char*)in_str)[i];
	}
	return hash;
}

struct hash_table* hash_table_create(int (*cmp_func)(void *,void *),hash_t (*hash_func)(void *))
{
	struct hash_table *self=GC_MALLOC(sizeof(struct hash_table));
	self->table_size=block_size;
	self->items=GC_MALLOC(sizeof(struct hash_table_item*)*(self->table_size));
	self->cmp_func=cmp_func;
	self->hash_func=hash_func;
	return self;
}

struct hash_table* hash_table_from(int (*cmp_func)(void *,void *), hash_t (*hash_func)(void *), size_t count, struct ht_item_init initial[])
{
	struct hash_table *self=hash_table_create(cmp_func,hash_func);
	for (iter_t i=0;i<count;i++)
	{
		hash_table_add(self,initial[i].data,initial[i].key);
	}
	return self;
}

void hash_table_grow(struct hash_table *self)
{
	struct dyn_array *(*old_items)[]=self->items;
	self->items=GC_MALLOC(sizeof(struct dyn_array*)*(self->table_size+block_size));

	//Refill the new list with the old stuff
	for (iter_t i = 0;i < self->table_size;i++)
	{
		if ((*old_items)[i]!=NULL)
		{
			(*(self->items))[((struct hash_table_item*)dyn_array_get((*old_items)[i],0))->key_hash%self->table_size+block_size]=(*old_items)[i];
		}
	}
	//free(old_items);
	self->table_size+=block_size;
}

void hash_table_shrink(struct hash_table *self)
{
	struct dyn_array *(*old_items)[]=self->items;
	self->items=GC_MALLOC(sizeof(struct dyn_array*)*(self->table_size-block_size));
	for (int i = 0;i < self->table_size;i++)
	{
		if ((*old_items)[i]!=NULL)
		{

			(*(self->items))[((struct hash_table_item*)dyn_array_get((*old_items)[i],0))->key_hash%self->table_size-block_size]=(*old_items)[i];
		}
	}
	self->table_size-=block_size;
}

void hash_table_remove(struct hash_table *self, void *key)
{
	hash_t key_hash;
	key_hash=(self->hash_func)(key);
	unsigned int index;
	index=key_hash%self->table_size;
	if ((*(self->items))[index]->filled==0)
	{
		(*(self->items))[index]=NULL;
		if (--self->table_filled < self->table_size-block_size)
		{
			hash_table_shrink(self);
		}
	}
	else
	{
		for (iter_t i=0;i < (*(self->items))[index]->filled;i++)
		{
			if ((self->cmp_func)(((struct hash_table_item*)dyn_array_get((*(self->items))[index],i))->key,key))
			{
				dyn_array_remove((*(self->items))[index],i);
			}
		}
	}
}

void hash_table_add(struct hash_table *self, void *key, void *data)
{
	if (++self->table_filled >= self->table_size)
	{
		hash_table_grow(self);
	}

	struct hash_table_item *new_item=GC_MALLOC(sizeof(struct hash_table_item));
	new_item->key_hash=(self->hash_func)(key);
	new_item->key=key;
	new_item->data=data;
	unsigned int index;
	index=new_item->key_hash%self->table_size;
	if ((*(self->items))[index]==NULL)
	{
		(*(self->items))[index]=dyn_array_create();
	}
	else
	{
		//Overwrite if this item already exists
		for (iter_t i=0;i < (*(self->items))[index]->filled;i++)
		{
			if ((self->cmp_func)(((struct hash_table_item*)dyn_array_get((*(self->items))[index],i))->key,key))
			{
				dyn_array_set((*(self->items))[index],i,new_item);
				return;
			}
		}
	}
	//If we didn't find an already existing item with the hash
	dyn_array_append((*(self->items))[index],new_item);
}

void *hash_table_get(struct hash_table *self, void *key)
{
	struct dyn_array *array=(*(self->items))[(self->hash_func)(key)%self->table_size];
	if (array==NULL)
	{
		return NULL;
	}

	for (unsigned int i=0;i < array->filled;i++)
	{
		if ((self->cmp_func)(((struct hash_table_item*)dyn_array_get(array,i))->key,key))
		{
			return ((struct hash_table_item*)dyn_array_get(array,i))->data;
		}
	}
	return NULL;
}
