CC:=gcc
CFLAGS+= -I . -I datastructures -I objects -I parserdata -O3

all: Makefile parserdata/main.o utils.o debug.o objects/factory_obj.o objects/global_obj.o objects/bool_obj.o objects/int_obj.o objects/str_obj.o objects/flt_obj.o objects/dyn_objs.o objects/func_obj.o objects/none_obj.o objects/type_obj.o datastructures/dyn_arrays.o datastructures/hash_tables.o datastructures/dyn_str.o objects/dict_obj.o objects/lst_obj.o objects/lst_iter_obj.o objects/str_iter_obj.o objects/term_obj.o
	$(CC) $(CFLAGS) parserdata/main.o utils.o debug.o objects/factory_obj.o objects/global_obj.o objects/bool_obj.o objects/int_obj.o objects/str_obj.o objects/flt_obj.o objects/dyn_objs.o objects/func_obj.o objects/none_obj.o objects/type_obj.o datastructures/dyn_arrays.o datastructures/hash_tables.o datastructures/dyn_str.o objects/lst_obj.o objects/dict_obj.o objects/lst_iter_obj.o objects/str_iter_obj.o objects/term_obj.o -lgc -lm

#We need to have it so that if we already have an executable that was compiled without debug mode it should be recompiled entirely but not otherwise
dbg: CFLAGS+=-DDEBUG -g -pg -Og -rdynamic
dbg: all

parserdata/main.o: parserdata/main.c parserdata/main.h parserdata/obj_method_list_refs.txt parserdata/obj_type_parent_refs.txt parserdata/obj_type_factory_list_refs.txt parserdata/obj_enums.txt parserdata/obj_sizes.txt parserdata/obj_names.txt parserdata/obj_method_list_refs.txt parserdata/obj_type_parent_refs.txt
	$(CC) $(CFLAGS) -c parserdata/main.c -o parserdata/main.o

debug.o: debug.c debug.h
	$(CC) $(CFLAGS) -c debug.c -o debug.o

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

objects/global_obj.o: objects/global_obj.c objects/global_obj.h
	$(CC) $(CFLAGS) -c objects/global_obj.c -o objects/global_obj.o

objects/factory_obj.o: objects/factory_obj.c objects/factory_obj.h
	$(CC) $(CFLAGS) -c objects/factory_obj.c -o objects/factory_obj.o

objects/dict_obj.o: objects/dict_obj.c objects/dict_obj.h objects/dyn_objs.h objects/type_obj.h
	$(CC) $(CFLAGS) -c objects/dict_obj.c -o objects/dict_obj.o

objects/lst_obj.o: objects/lst_obj.c objects/lst_obj.h objects/dyn_objs.h objects/type_obj.h
	$(CC) $(CFLAGS) -c objects/lst_obj.c -o objects/lst_obj.o

objects/lst_iter_obj.o: objects/lst_iter_obj.c objects/lst_iter_obj.h objects/dyn_objs.h objects/type_obj.h
	$(CC) $(CFLAGS) -c objects/lst_iter_obj.c -o objects/lst_iter_obj.o

objects/str_iter_obj.o: objects/str_iter_obj.c objects/str_iter_obj.h objects/dyn_objs.h objects/type_obj.h
	$(CC) $(CFLAGS) -c objects/str_iter_obj.c -o objects/str_iter_obj.o

objects/term_obj.o: objects/term_obj.c objects/term_obj.h objects/dyn_objs.h objects/type_obj.h
	$(CC) $(CFLAGS) -c objects/term_obj.c -o objects/term_obj.o

objects/bool_obj.o: objects/bool_obj.c objects/bool_obj.h objects/dyn_objs.h objects/type_obj.h
	$(CC) $(CFLAGS) -c objects/bool_obj.c -o objects/bool_obj.o

objects/int_obj.o: objects/int_obj.c objects/int_obj.h objects/dyn_objs.h objects/type_obj.h objects/bool_obj.h objects/none_obj.h objects/str_obj.h
	$(CC) $(CFLAGS) -c objects/int_obj.c -o objects/int_obj.o

objects/str_obj.o: objects/str_obj.c objects/str_obj.h objects/dyn_objs.h objects/type_obj.h objects/int_obj.h
	$(CC) $(CFLAGS) -c objects/str_obj.c -o objects/str_obj.o

objects/flt_obj.o: objects/flt_obj.c objects/flt_obj.h objects/dyn_objs.h objects/type_obj.h
	$(CC) $(CFLAGS) -c objects/flt_obj.c -o objects/flt_obj.o

objects/dyn_objs.o: objects/dyn_objs.c objects/dyn_objs.h datastructures/dyn_arrays.h datastructures/dyn_str.h datastructures/hash_tables.h objects/func_obj.h debug.h parserdata/obj_enums.txt parserdata/obj_sizes.txt parserdata/obj_type_factory_list_refs.txt parserdata/obj_method_list_refs.txt parserdata/obj_type_parent_refs.txt parserdata/obj_names.txt parserdata/obj_type_parent_refs.txt
	$(CC) $(CFLAGS) -c objects/dyn_objs.c -o objects/dyn_objs.o

objects/func_obj.o: objects/func_obj.c objects/func_obj.h objects/dyn_objs.h objects/type_obj.h objects/none_obj.h
	$(CC) $(CFLAGS) -c objects/func_obj.c -o objects/func_obj.o

objects/none_obj.o: objects/none_obj.c objects/none_obj.h objects/dyn_objs.h objects/type_obj.h
	$(CC) $(CFLAGS) -c objects/none_obj.c -o objects/none_obj.o

objects/type_obj.o: objects/type_obj.c objects/type_obj.h objects/dyn_objs.h
	$(CC) $(CFLAGS) -c objects/type_obj.c -o objects/type_obj.o

datastructures/dyn_arrays.o: datastructures/dyn_arrays.c datastructures/dyn_arrays.h
	$(CC) $(CFLAGS) -c datastructures/dyn_arrays.c -o datastructures/dyn_arrays.o

datastructures/hash_tables.o: datastructures/hash_tables.c datastructures/hash_tables.h
	$(CC) $(CFLAGS) -c datastructures/hash_tables.c -o datastructures/hash_tables.o

datastructures/dyn_str.o: datastructures/dyn_str.c datastructures/dyn_str.h
	$(CC) $(CFLAGS) -c datastructures/dyn_str.c -o datastructures/dyn_str.o

clean:
	$(RM) parserdata/main.o utils.o objects/lst_obj.o objects/dict_obj.o objects/bool_obj.o objects/int_obj.o objects/str_obj.o objects/flt_obj.o objects/dyn_objs.o objects/func_obj.o objects/none_obj.o objects/type_obj.o datastructures/dyn_arrays.o datastructures/hash_tables.o datastructures/dyn_str.o objects/global_obj.o debug.o
