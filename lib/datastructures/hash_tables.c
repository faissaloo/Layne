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

#include <string.h>
#include <gc.h>

#include "ll.h"
#include "dyn_arrays.h"
#include "hash_tables.h"

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
	struct hash_table *self;
	self=GC_MALLOC(sizeof(struct hash_table));
	self->table_size=block_size;
	self->items=GC_MALLOC(sizeof(struct ll_item*)*(self->table_size));
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
	struct ll_item *(*old_items)[]=self->items;
	self->items=GC_MALLOC(sizeof(struct ll_item*)*(self->table_size+block_size));

	//Refill the new list with the old stuff
	for (iter_t i = 0;i < self->table_size;i++)
	{
		if ((*old_items)[i]!=NULL)
		{
			(*(self->items))[((struct hash_table_item*)((*old_items)[i]->data))->key_hash%(self->table_size+block_size)]=(*old_items)[i];
		}
	}
	self->table_size+=block_size;
}

void hash_table_shrink(struct hash_table *self)
{
	struct ll_item *(*old_items)[]=self->items;
	self->items=GC_MALLOC(sizeof(struct ll_item*)*(self->table_size-block_size));
	for (int i = 0;i < self->table_size;i++)
	{
		if ((*old_items)[i]!=NULL)
		{
			(*(self->items))[((struct hash_table_item*)((*old_items)[i]->data))->key_hash%(self->table_size-block_size)]=(*old_items)[i];
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
	if ((*(self->items))[index]->next==NULL)
	{
		(*(self->items))[index]=NULL;
		if (--self->table_filled < self->table_size-block_size)
		{
			hash_table_shrink(self);
		}
	}
	else
	{
		struct ll_item **indirect;
		indirect=&(*(self->items))[index];
		while ((self->cmp_func)(((struct hash_table_item*)(*indirect)->data)->key,key))
		{
			indirect=&(*indirect)->next;
		}
		*indirect=(*indirect)->next;
	}
}

void hash_table_add(struct hash_table *self, void *key, void *data)
{
	if (++self->table_filled >= self->table_size)
	{
		hash_table_grow(self);
	}

	struct hash_table_item *new_item;
	new_item=GC_MALLOC(sizeof(struct hash_table_item));
	new_item->key_hash=(self->hash_func)(key);
	new_item->key=key;
	new_item->data=data;
	unsigned int index;
	index=new_item->key_hash%self->table_size;
	if ((*(self->items))[index]==NULL)
	{
		(*(self->items))[index]=GC_MALLOC(sizeof(struct ll_item));
		(*(self->items))[index]->data=new_item;
		(*(self->items))[index]->next=NULL;
	}
	else
	{
		//Overwrite if this item already exists
		struct ll_item *i=(*(self->items))[index];
		for (;i->next!=NULL;i=i->next)
		{
			if ((self->cmp_func)(((struct hash_table_item*)i->data)->key,key))
			{
				i->data=new_item;
				return;
			}
		}
		//Append if item does not exist
		i->next=GC_MALLOC(sizeof(struct ll_item));
		i->next->data=new_item;
		i->next->next=NULL;
	}
}

void *hash_table_get(struct hash_table *self, void *key)
{
	struct ll_item *ll=(*(self->items))[(self->hash_func)(key)%self->table_size];

	while (ll)
	{
		if ((self->cmp_func)(((struct hash_table_item*)ll->data)->key,key))
		{
			return ((struct hash_table_item*)ll->data)->data;
		}
		ll=ll->next;
	}

	return NULL;
}
