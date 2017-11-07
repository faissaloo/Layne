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

#include <stddef.h>
#include <stdio.h>
#include "dyn_str.h"
#include "utils.h"
#include <string.h>
#include <gc.h>
#include <stdlib.h>
#include <stdbool.h>

#define block_size 128

struct dyn_str* dyn_str_create()
{
	struct dyn_str *new_dyn_str;
	new_dyn_str=GC_MALLOC(sizeof(struct dyn_str));
	new_dyn_str->filled=0;
	new_dyn_str->length=block_size;
	new_dyn_str->raw=GC_MALLOC(sizeof(char)*(new_dyn_str->length));
	return new_dyn_str;
}

bool dyn_str_eq(struct dyn_str *a, struct dyn_str *b)
{
	if (a->filled==b->filled)
	{
		return memcmp(*a->raw,*b->raw,a->filled*sizeof(char))==0;
	}
	return false;
}

void dyn_str_cat_cstr(struct dyn_str *self,char str[])
{
	size_t len;
	len=strlen(str);
	self->filled+=len;
	//+1 because let's not forget the NULL
	if (self->filled+1 >= self->length)
	{
		//Gets the smallest multiple of block_size that will fit the new string
		self->length=(((self->filled+1)/block_size)+1*block_size);
		self->raw=GC_REALLOC(self->raw,sizeof(char)*self->length);
	}
	memcpy((*self->raw)+(sizeof(char)*(self->filled-len)),str,len*sizeof(char));
}

void dyn_str_cat(struct dyn_str *self,struct dyn_str *str)
{
	self->filled+=str->filled;
	//+1 because let's not forget the NULL
	if (self->filled+1 >= self->length)
	{
		//Gets the smallest multiple of block_size that will fit the new string
		self->length=(((self->filled+1)/block_size)+1*block_size);
		self->raw=GC_REALLOC(self->raw,sizeof(char)*self->length);
	}
	memcpy((*self->raw)+(sizeof(char)*(self->filled-str->filled)),*(str->raw),str->filled*sizeof(char));
	(*(self->raw))[self->filled]='\0';
}

struct dyn_str* dyn_str_cut(struct dyn_str *self, int start, int end)
{
	struct dyn_str *new_str;
	new_str=dyn_str_create();
	if (start<0)
	{
		start=self->filled-start;
	}
	if (end<0)
	{
		end=self->filled-end;
	}

	new_str->filled=end-start;
	new_str->length=(((new_str->filled+1)/block_size)+1*block_size);
	new_str->raw=GC_MALLOC(sizeof(char)*new_str->length);

	memcpy(*new_str->raw,(*self->raw)+(sizeof(char)*start),new_str->filled*sizeof(char));
	(*new_str->raw)[new_str->filled]='\0';
	return new_str;
}

struct dyn_str* dyn_str_get_char(struct dyn_str *self, int pos)
{
	//Returns a new string pointing to the particular character in the string so strings can be mutable
	struct dyn_str *new_str;
	new_str=dyn_str_create();
	if (pos<0)
	{
		pos=self->filled-pos;
	}
	new_str->length=2*block_size;
	new_str->raw=GC_MALLOC((sizeof(char)*2)*block_size);
	new_str->filled=1;
	memcpy(*(new_str->raw),*(self->raw)+(sizeof(char)*pos),sizeof(char));
	(*(new_str->raw))[1]='\0';
	return new_str;
}

struct dyn_str* dyn_str_from_cstr(char str[])
{
	struct dyn_str *to_ret;
	to_ret=dyn_str_create();
	dyn_str_cat_cstr(to_ret,str);
	return to_ret;
}

struct dyn_str* dyn_str_from_int(int n)
{
	struct dyn_str *new_str;
	char *buff;
	buff=GC_MALLOC(int_len(n));
	new_str=dyn_str_create();
	int_to_str(n,buff);
	dyn_str_cat_cstr(new_str,buff);
	return new_str;
}

//Make a less crap version of this at some point
//Issues:
// This depends on the block_size being big enough to reasonably store the float
// It's using snprintf which is slow as hell
// It uses strlen to figure out the length even though we could already know this
struct dyn_str* dyn_str_from_float(double n)
{
	struct dyn_str *new_str;
	new_str=dyn_str_create();
	snprintf(*new_str->raw,new_str->length,"%f",n);
	new_str->filled=strlen(*new_str->raw);
	return new_str;
}

int dyn_str_to_int(struct dyn_str *self)
{
	char *end;
	return strtol(*self->raw,&end,0);
}

double dyn_str_to_double(struct dyn_str *self)
{
	char *end;
	return strtod(*self->raw,&end);
}

hash_t dyn_str_hash(struct dyn_str *self)
{
	hash_t hash;
	hash=0;
	for (iter_t i=0;i < self->filled;i++)
	{
		hash*=31;
		hash+=(*self->raw)[i];
	}
	return hash;
}
