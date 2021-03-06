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
#define decl_dyn_fn(name) struct dyn_obj* name(size_t arg_count, struct dyn_obj *args[]);\
struct dyn_obj* factory_##name(size_t arg_count, struct dyn_obj *args[]);
//Like dyn_fn but all it does is pop the self and pass the rest of its arguments on to a function
//Like how you can do int.__method__(x) in python instead of x.__method__() this is mainly so
//children can call a parent's equivelant of a method as needed
#define def_dyn_fn(name) struct dyn_obj* name(size_t arg_count, struct dyn_obj *args[]);\
struct dyn_obj* factory_##name(size_t arg_count, struct dyn_obj *args[])\
{\
	return name(arg_count-1,&args[1]);\
}\
struct dyn_obj* name(size_t arg_count, struct dyn_obj *args[])

//Gets self for methods
#define SELF args[0]

//Without method inheritence or destructor support
#define obj_setup_basic(typecode)\
	struct dyn_obj *self;\
	self=GC_MALLOC(type_sizes[typecode]);\
	self->members=hash_table_create(&string_eq,&hash_string);\
	self->cur_type=typecode;\
	bind_member(self,"parent",*type_parent_list[typecode]);\
	init_methods(self,type_method_lists[typecode]);

#define fact_setup_basic(typecode)\
	struct dyn_obj *self;\
	self=GC_MALLOC(type_sizes[FACTORY]);\
	self->members=hash_table_create(&string_eq,&hash_string);\
	self->cur_type=FACTORY;\
	((struct factory_obj*)self)->type_to_create=typecode;\
	bind_member(self,"parent",*type_parent_list[typecode]);\
	type_factory_list[((struct factory_obj*)self)->type_to_create]=&self;

//This little function here will go through the 'parents' linked list backwards and apply every method, with ones closer to the object replacing the ones further
#define object_setup(typecode)\
	struct dyn_obj *self;\
	self=GC_MALLOC(type_sizes[typecode]);\
	self->members=hash_table_create(&string_eq,&hash_string);\
	self->cur_type=typecode;\
	bind_member(self,"parent",*type_parent_list[typecode]);\
	struct ll_item *cur_item;\
	cur_item=NULL;\
	struct ll_item *prev_item;\
	for (struct dyn_obj *i=self;!(i->cur_type==FACTORY && ((struct factory_obj*)i)->type_to_create==TYPE);i=get_member(i,"parent"))\
	{\
		prev_item=cur_item;\
		cur_item=GC_MALLOC(sizeof(struct ll_item));\
		if (prev_item)\
		{\
			prev_item->next=cur_item;\
		}\
		cur_item->prev=prev_item;\
		if (i->cur_type==FACTORY)\
		{\
			cur_item->data=type_method_lists[((struct factory_obj*)i)->type_to_create];\
		}\
		else\
		{\
			cur_item->data=type_method_lists[i->cur_type];\
		}\
	}\
	for (;cur_item!=NULL;cur_item=cur_item->prev)\
	{\
		init_methods(self,(const struct method_list *)cur_item->data);\
	}\
	reg_destructor(self);

#define factory_setup(typecode)\
	struct dyn_obj *self;\
	self=GC_MALLOC(type_sizes[FACTORY]);\
	self->members=hash_table_create(&string_eq,&hash_string);\
	self->cur_type=FACTORY;\
	((struct factory_obj*)self)->type_to_create=typecode;\
	bind_member(self,"parent",*type_parent_list[typecode]);\
	type_factory_list[((struct factory_obj*)self)->type_to_create]=&self;\
	struct ll_item *cur_item;\
	cur_item=NULL;\
	struct ll_item *prev_item;\
	for (struct dyn_obj *i=self;!(i->cur_type==FACTORY && ((struct factory_obj*)i)->type_to_create==TYPE);i=get_member(i,"parent"))\
	{\
		prev_item=cur_item;\
		cur_item=GC_MALLOC(sizeof(struct ll_item));\
		if (prev_item)\
		{\
			prev_item->next=cur_item;\
		}\
		cur_item->prev=prev_item;\
		if (i->cur_type==FACTORY)\
		{\
			cur_item->data=type_method_lists[((struct factory_obj*)i)->type_to_create];\
			type_sizes[((struct factory_obj*)self)->type_to_create]=max(type_sizes[((struct factory_obj*)self)->type_to_create],type_sizes[((struct factory_obj*)i)->type_to_create]);\
		}\
		else\
		{\
			cur_item->data=type_method_lists[i->cur_type];\
			type_sizes[((struct factory_obj*)self)->type_to_create]=max(type_sizes[((struct factory_obj*)self)->type_to_create],type_sizes[i->cur_type]);\
		}\
	}\
	for (;cur_item!=NULL;cur_item=cur_item->prev)\
	{\
		init_factory_methods(self,(const struct method_list *)cur_item->data);\
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
	LST,
	DICT,
	TERM,
	LST_ITER,
	STR_ITER
	#include "obj_enums.txt"
};

//We need a thing that matches up enums with default lists of methods
//A struct that holds method name and associated function for use in an array
struct method_pair
{
	char *method_name;
	struct dyn_obj* (*method)(size_t arg_count, struct dyn_obj *args[]);
	struct dyn_obj* (*factory_method)(size_t arg_count, struct dyn_obj *args[]);
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
	//The data stored is a function
	enum type cur_type;
};

void bind_member(struct dyn_obj *obj, char member_name[], struct dyn_obj *member_obj);
void bind_method(struct dyn_obj *obj,char method_name[],struct dyn_obj* (*cfunc)(size_t arg_count, struct dyn_obj *args[]));
struct dyn_obj* get_member(struct dyn_obj *obj,char member_name[]);
struct dyn_obj* copy_obj(struct dyn_obj *obj,struct hash_table *spanned_objs);
struct dyn_obj* call_method(struct dyn_obj *obj, char method_name[], size_t arg_count, struct dyn_obj *args[]);
struct dyn_obj* call_method_noargs(struct dyn_obj *obj, char method_name[]);
struct dyn_obj* call_function(struct dyn_obj *object_to_call, size_t arg_count, struct dyn_obj *args[]);
bool is_child(struct dyn_obj *obj,enum type type_id);
void reg_destructor(struct dyn_obj *obj);
struct dyn_obj* p_bool(int input);

extern struct dyn_obj *kw_global;
extern struct dyn_obj *kw_true;
extern struct dyn_obj *kw_false;
extern struct dyn_obj *kw_none;
extern struct dyn_obj *type_factory;
extern struct dyn_obj *int_factory;
extern struct dyn_obj *flt_factory;
extern struct dyn_obj *bool_factory;
extern struct dyn_obj *str_factory;
#endif
