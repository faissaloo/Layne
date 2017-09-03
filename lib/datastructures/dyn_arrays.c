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
#include "utils.h"
#include "dyn_arrays.h"

#define block_size 128

struct dyn_array* dyn_array_create()
{
	struct dyn_array *new_dyn_array=GC_MALLOC(sizeof(struct dyn_array));
	new_dyn_array->filled=0;
	new_dyn_array->length=block_size;
	new_dyn_array->items=GC_MALLOC(sizeof(void*)*new_dyn_array->length);
	return new_dyn_array;
}

struct dyn_array* dyn_array_from(size_t count,void *initial[])
{
	struct dyn_array *new_dyn_array=GC_MALLOC(sizeof(struct dyn_array));
	new_dyn_array->filled=count;
	new_dyn_array->length=((count/block_size)*block_size);
	new_dyn_array->items=GC_MALLOC(sizeof(void*)*new_dyn_array->length);
	memcpy(*new_dyn_array->items,initial,new_dyn_array->filled*sizeof(void*));
	return new_dyn_array;
}

void dyn_array_append(struct dyn_array *self,void *data)
{
	//I'm guessing the issue is that the last item is wrongly being copied to the place of the new item
	if (++self->filled>=self->length)
	{
		self->length+=block_size;
		self->items=GC_REALLOC(self->items,sizeof(void*)*self->length);
	}
	(*self->items)[self->filled-1]=data;
}

void dyn_array_insert(struct dyn_array *self, signed int index, void *data)
{
	//Could also be here because of how we call methods
	unsigned int start;
	unsigned int end;
	if (index>=0 && index <= self->filled)
	{
		start=index;
	}
	else if (-index<=self->filled)
	{
		start=self->filled+index;
	}
	else
	{
		return;
	}

	end=self->filled;

	if (++self->filled>=self->length)
	{
		self->length+=block_size;
		self->items=GC_REALLOC(self->items,sizeof(void*)*self->length);
	}

	for (iter_t i=end;i > start;i--)
	{
		(*self->items)[i]=(*self->items)[i-1];
	}
	(*self->items)[start]=data;
}

void dyn_array_remove(struct dyn_array *self, signed int index)
{
	iter_t i;
	unsigned int end;
	if (index>=0)
	{
		i=index;
		end=self->filled;
	}
	else
	{
		i=self->filled+index;
		end=self->filled;
	}
	//Test this
	for (;i < end-1;i++)
	{
		(*self->items)[i]=(*self->items)[i+1];
	}
	(*self->items)[end-1]=NULL;

	if (--self->filled < self->length-block_size)
	{
		self->length-=block_size;
		self->items=GC_REALLOC(self->items,sizeof(void*)*self->length);
	}

}

void *dyn_array_get(struct dyn_array *self, signed int index)
{
	if (index>=0 && index <= self->filled)
	{
		return (*self->items)[index];
	}
	else if (-index<=self->filled)
	{
		return (*self->items)[self->filled+index];
	}
	else
	{
		return NULL;
	}
}

struct dyn_array* dyn_array_cut(struct dyn_array *self, int start, int end)
{
	struct dyn_array *new_array;
	new_array=dyn_array_create();
	if (start<0)
	{
		start=self->filled-start;
	}
	if (end<0)
	{
		end=self->filled-end;
	}
	new_array->filled=end-start;
	new_array->length=(((new_array->filled)/block_size)*block_size);
	new_array->items=GC_MALLOC(sizeof(void*)*new_array->length);

	memcpy(*new_array->items,(*new_array->items)+(sizeof(void*)*start),new_array->filled*sizeof(void*));

	return new_array;
}

void dyn_array_set(struct dyn_array *self, signed int index,void *data)
{
	if (index>=0)
	{
		(*self->items)[index]=data;
	}
	else
	{
		(*self->items)[self->filled+index]=data;
	}
}

/*
int main()
{
	struct dyn_array *array=dyn_array_create();
	dyn_array_append(array,"B");
	dyn_array_remove(array,0);
	printf("%s",dyn_array_get(array,0));
	//array;
}
*/
