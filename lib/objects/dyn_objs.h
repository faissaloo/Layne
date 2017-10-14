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

#ifndef DYN_OBJS_H
#define DYN_OBJS_H

#include "dyn_str.h"
#include "dyn_arrays.h"
#include "hash_tables.h"
#include "utils.h"

//Uncomment for error checking etc
//#define SAFE
//Gets the main obj

#define struct_cast(strct,variable) *((strct*)&variable)

//Creates a function meant to be used by the user
#define decl_dyn_fn(name) struct dyn_obj* name(struct dyn_array *args);\
struct dyn_obj* factory_##name(struct dyn_array *args);
//Like dyn_fn but all it does is pop the self and pass the rest of its arguments on to a function
//Like how you can do int.__method__(x) in python instead of x.__method__() this is mainly so
//children can call a parent's equivelant of a method as needed
#define def_dyn_fn(name) struct dyn_obj* name(struct dyn_array *args);\
struct dyn_obj* factory_##name(struct dyn_array *args)\
{\
	dyn_array_remove(args,0);\
	return name(args);\
}\
struct dyn_obj* name(struct dyn_array *args)

#define get_arg(x) dyn_array_get(args,x)

//Gets self for methods
#define SELF get_arg(0)

//Without method inheritence or destructor support
#define obj_setup_basic(typecode)\
	struct dyn_obj *self;\
	self=GC_MALLOC(type_sizes[typecode]);\
	self->members=hash_table_create(&string_eq,&hash_string);\
	self->size=type_sizes[typecode];\
	self->cur_type=typecode;\
	bind_member(self,"parent",*type_parent_list[typecode]);\
	init_methods(self,type_method_lists[typecode]);

//This little function here will go through the 'parents' linked list backwards and apply every method, with ones closer to the object replacing the ones further
//We're going to be lazy and use an array instead of a linked list to reverse the linked list for now but we should make a linked list type later
#define object_setup(typecode)\
	struct dyn_obj *self;\
	self=GC_MALLOC(type_sizes[typecode]);\
	self->members=hash_table_create(&string_eq,&hash_string);\
	self->size=type_sizes[typecode];\
	self->cur_type=typecode;\
	bind_member(self,"parent",*type_parent_list[typecode]);\
	struct dyn_array *temp_array;\
	temp_array=dyn_array_create();\
	for (struct dyn_obj *i=self;!(i->cur_type==FACTORY && ((struct factory_obj*)i)->type_to_create==TYPE);i=get_member(i,"parent"))\
	{\
		if (i->cur_type==FACTORY)\
		{\
			dyn_array_append(temp_array,type_method_lists[((struct factory_obj*)i)->type_to_create]);\
		}\
		else\
		{\
			dyn_array_append(temp_array,type_method_lists[i->cur_type]);\
		}\
	}\
	for (iter_t i=0;i<temp_array->filled;i++)\
	{\
		init_methods(self,dyn_array_get(temp_array,i));\
	}\
	reg_destructor(self);

#define factory_setup(typecode)\
	struct dyn_obj *self;\
	self=GC_MALLOC(type_sizes[FACTORY]);\
	self->members=hash_table_create(&string_eq,&hash_string);\
	self->size=type_sizes[FACTORY];\
	self->cur_type=FACTORY;\
	((struct factory_obj*)self)->type_to_create=typecode;\
	bind_member(self,"parent",*type_parent_list[typecode]);\
	type_factory_list[((struct factory_obj*)self)->type_to_create]=&self;\
	struct dyn_array *temp_array;\
	temp_array=dyn_array_create();\
	for (struct dyn_obj *i=self;!(i->cur_type==FACTORY && ((struct factory_obj*)i)->type_to_create==TYPE);i=get_member(i,"parent"))\
	{\
		if (i->cur_type==FACTORY)\
		{\
			dyn_array_append(temp_array,type_method_lists[((struct factory_obj*)i)->type_to_create]);\
			type_sizes[((struct factory_obj*)self)->type_to_create]=max(type_sizes[((struct factory_obj*)self)->type_to_create],type_sizes[((struct factory_obj*)i)->type_to_create]);\
		}\
		else\
		{\
			dyn_array_append(temp_array,type_method_lists[i->cur_type]);\
			type_sizes[((struct factory_obj*)self)->type_to_create]=max(type_sizes[((struct factory_obj*)self)->type_to_create],type_sizes[i->cur_type]);\
		}\
	}\
	\
	for (iter_t i=0;i<temp_array->filled;i++)\
	{\
		init_factory_methods(self,dyn_array_get(temp_array,i));\
	}

enum type
{
	TYPE, //The big daddy of our datatypes
	FACTORY, //A factory is like 'int' or something, it makes new instances
	GLOBAL,
	NONE,
	FUNCTION,
	INT,
	FLT,
	BOOL,
	STR,
	ARRAY,
	DICT
	#include "obj_enums.txt"
};

//We need a thing that matches up enums with default lists of methods
//A struct that holds method name and associated function for use in an array
struct method_pair
{
	char *method_name;
	struct dyn_obj* (*method)(struct dyn_array*);
	struct dyn_obj* (*factory_method)(struct dyn_array*);
};

struct method_list
{
	size_t count;
	struct method_pair method_pair[];
};

#define method_pair(name,func) {name,func,factory_##func}

void init_methods(struct dyn_obj *self, const struct method_list *methods);
void init_factory_methods(struct dyn_obj *self, const struct method_list *methods);

extern const char *type_names[];
extern size_t type_sizes[];
extern struct dyn_obj **type_parent_list[];
extern struct dyn_obj **type_factory_list[];
extern struct method_list *type_method_lists[];

struct dyn_obj
{
	//Pointers to all members of the object,
	//which will all be dyn_objs
	struct hash_table *members;
	size_t size; //Size (for copying)

	//The data stored is a function
	enum type cur_type;
};
/*
struct array_obj
{
	struct type_obj parent;
	struct dyn_array data;
};

struct dict_obj
{
	struct type_obj parent;

};*/

void bind_member(struct dyn_obj *obj, char member_name[], struct dyn_obj *member_obj);
void bind_method(struct dyn_obj *obj,char method_name[],struct dyn_obj* (*cfunc)(struct dyn_array*));
struct dyn_obj* get_member(struct dyn_obj *obj,char member_name[]);
struct dyn_obj* copy_obj(struct dyn_obj *obj,struct hash_table *spanned_objs);
struct dyn_obj* call_method(struct dyn_obj *obj, char method_name[], struct dyn_array *args);
struct dyn_obj* call_method_noargs(struct dyn_obj *obj, char method_name[]);
struct dyn_obj* call_function(struct dyn_obj *object_to_call, struct dyn_array *args);
bool is_child(struct dyn_obj *obj,enum type type_id);
void reg_destructor(struct dyn_obj *obj);

extern struct dyn_obj *global;
extern struct dyn_obj *type_factory;
extern struct dyn_obj *int_factory;
extern struct dyn_obj *flt_factory;
extern struct dyn_obj *bool_factory;
extern struct dyn_obj *str_factory;

#endif
