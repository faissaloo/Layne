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

	#ifdef DEBUG
	#ifndef DEBUG_H
		#define DEBUG_H
		#include <stdlib.h>
		#ifdef __GLIBC__
			#include <execinfo.h>
		#endif
		#include "utils.h"
		#include "dyn_objs.h"


		//So we can pass stuff like {1,2,3} to macros
		#define protect(...) __VA_ARGS__

		#define BT_MAX 6
		#define error(errno, msg, ...)\
		{\
			void *trace[BT_MAX];\
			char **trace_strs;\
			size_t trace_len;\
			trace_len=backtrace(trace,BT_MAX);\
			trace_strs=backtrace_symbols(trace,trace_len);\
			puts("[Fatal Error]");\
			for (iter_t i=trace_len-1;i>0;i--)\
			{\
				printf("\t%s\n",trace_strs[i]);\
			}\
			printf("%s(): "msg"\n", __func__, ##__VA_ARGS__);\
			exit(errno);\
		}

		//Checks arguments for functions with fixed length arguments
		//NOTE: This is inconsistent, args->filled and args->length should both be size_ts
		#define arg_guard(min_args,max_args,names,types)\
		{\
			enum type argument_type_array[]=types;\
			const char *argument_names_array[]=names;\
			if (min_args!=max_args)\
			{\
				if (args->filled>max_args)\
				{\
					error(1,"Too many arguments, expected at most %i, got %i",max_args,args->filled);\
				}\
				else if (args->filled<min_args)\
				{\
					error(1,"Too few arguments, expected at least %i, got %i",min_args,args->filled);\
				}\
			}\
			else if(args->filled>max_args)\
			{\
				error(1,"Too many arguments, expected %i, got %i",max_args,args->filled)\
			}\
			else if(args->filled<max_args)\
			{\
				error(1,"Too few arguments, expected %i, got %i",max_args,args->filled)\
			}\
			for (iter_t current_argument_i=0;current_argument_i<args->filled;current_argument_i++)\
			{\
				if (!is_child(get_arg(current_argument_i),argument_type_array[current_argument_i]))\
				{\
					error(1,"%s (argument %li) is not an instance of %s, got %s",argument_names_array[current_argument_i],current_argument_i,type_names[argument_type_array[current_argument_i]],type_names[((struct dyn_obj*)get_arg(current_argument_i))->cur_type]);\
				}\
			}\
		}
	#endif
#endif
