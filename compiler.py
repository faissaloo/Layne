#!/usr/bin/env python3
# Copyright (C) 2017 Faissal Bensefia
#
# This file is part of Layne.
#
# Layne is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Layne is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Layne.  If not, see <http://www.gnu.org/licenses/>.

import sys
import os

txt=open("example.l").read()

#oquote = open quote
#odquote = open double quote
#str = a string
#whitespace = whitespace
#id = variables or keywords
#flt = float
#int = integer
#bin = binary number
#hex = hexadecimal number
#dop = dyadic operation
#mdop = special dyadics (can't be combined with =)
#uop = unary operation
#udop = unary or dyadic operation
#uadop = unary additional operator, an operation that is unary but can have an = or something placed infront to turn it into a dyadic operation
#uaiop = unary in place operation like $~x
#oparen = (
#cparen = )
#obrack = [
#cbrack = ]
#obrace = {
#cbrace = }

def tokenise():
	global txt
	tokens=[]
	i=0
	curType="whitespace"
	curToken=""

	lineNum=0
	colNum=0
	def appendToken():
		if curType!="whitespace":
			tokens.append((curType,curToken,colNum,lineNum))
	for i in txt:
		if i=="\n":
			lineNum+=1
			colNum=0
		else:
			colNum+=1
		#String quotes
		if i=="'":
			if curType!="oquote":
				appendToken()
				curType="oquote"
				curToken=i
			else:
				curToken=curToken[1:]
				curType="str"
				appendToken()
				curType="whitespace"
				curToken=""
		elif i=='"':
			if curType!="odquote":
				appendToken()
				curType="odquote"
				curToken=i
			else:
				curToken=curToken[1:]
				curType="str"
				appendToken()
				curType="whitespace"
				curToken=""

		elif curType=="oquote" or curType=="odquote":
			curToken+=i
		elif i in "\t \n":
			appendToken()
			curType="whitespace"
			curToken=""
		#Numbers
		elif (curType!="id") and ((i in "01") or (curType!="bin" and i in "23456789") or (curType=="int" and i==".") or (curToken=="0" and curType=="int" and (i=="b" or i=="x")) or (curType == "hex" and (i.upper() in "ABCDEF"))):
			if curType not in ["int","flt","bin","hex"]:
				appendToken()
				curType="int"
				curToken=""

			curToken+=i

			if i==".":
				curType="flt"
			elif i=="b":
				curType="bin"
				curToken=""
			elif i=="x":
				curType="hex"
				curToken=""
		#id (either keyword or variable name)
		elif i.isalnum() or i=="_" or curType=="oquote" or curType=="odquote":
			if curType!="id" and (curType !="oquote" and curType!="odquote"):
				appendToken()
				curType="id"
				curToken=""
			curToken+=i

		#Operations with multiple symbols
		elif i==">":
			if curType=="dop" and curToken==">":
				curToken=">>"
			else:
				appendToken()
				curType="dop"
				curToken=i
		elif i=="<":
			if curType=="dop" and curToken=="<":
				curToken="<<"
			else:
				appendToken()
				curType="dop"
				curToken=i
		elif i=="=":
			if curType=="dop" or curType=="udop" or curType=="uadop":
				curToken+=i
				curType="dop"
			else:
				appendToken()
				curType="mdop"
				curToken=i
		#Brackets
		elif i=="(":
			appendToken()
			curType="oparen"
			curToken=i
		elif i==")":
			appendToken()
			curType="cparen"
			curToken=i

		elif i=="[":
			appendToken()
			#Brack Obama
			curType="obrack"
			curToken=i
		elif i=="]":
			appendToken()
			curType="cbrack"
			curToken=i

		elif i=="{":
			appendToken()
			curType="obrace"
			curToken=i
		elif i=="}":
			appendToken()
			curType="cbrace"
			curToken=i
		#In place unary operations
		elif i=="$":
			curType="uaiop"
			curToken+=i
		#Raw simple dyadic operations
		elif i in "/*%|&^":
			appendToken()
			curType="dop"
			curToken=i
		#Can be unary or dyadic operations
		elif i in "+-":
			if curType=="uaiop":
				curToken+=i
			else:
				appendToken()
				curType="udop"
				curToken=i
		#Unary, but every time there's an = it's an assignment (or a comparison)
		elif i in "@!?":
			if curType=="uaiop":
				curToken+=i
			else:
				appendToken()
				curType="uadop"
				curToken=i
		#Unary operations
		elif i in "~":
			if curType=="uaiop":
				curToken+=i
			else:
				appendToken()
				curType="uop"
				curToken=i

		#Special dyadic operations (that can't be combined with =)
		elif i in ".,:":
			appendToken()
			curType="mdop"
			curToken=i
		else:
			print("\033[91mFatal error:\033[m Unexpected symbol at line "+str(lineNum)+"\n"+
				txt.split("\n")[lineNum]+"\n"+
				(" "*(colNum-1))+"^",file=sys.stderr)
			exit(255)

	appendToken()
	return tokens#[1:]

class literal():
	def __init__(self,contents):
		self.contents=contents

class strLiteral(literal):
	def __repr__(self):
		return '"'+self.contents+'"'
	def C(self):
		return 'create_str(dyn_str_from_cstr("'+self.contents+'"))'

class fltLiteral(literal):
	def __init__(self,contents):
		self.contents=float(contents)

	def __repr__(self):
		return str(self.contents)

	def C(self):
		return 'create_flt('+str(self.contents)+')'

class intLiteral(literal):
	def __init__(self,contents):
		self.contents=int(contents)

	def C(self):
		return "create_int("+str(self.contents)+")"

	def __repr__(self):
		return str(self.contents)

class binLiteral(literal):
	def __init__(self,contents):
		self.contents=int(contents,2)

	def __repr__(self):
		return bin(self.contents)

	def C(self):
		return "create_int("+str(self.contents)+")"

class hexLiteral(literal):
	def __init__(self,contents):
		self.contents=int(contents,16)

	def __repr__(self):
		return hex(self.contents)

	def C(self):
		return "create_int("+str(self.contents)+")"

class variable():
	def __init__(self,name):
		self.name=name

	def C(self):
		return self.name
	def __repr__(self):
		return self.name

class unaryOp():
	def __init__(self,operand):
		self.operand=operand

class dyadicOp():
	def __init__(self,operand1,operand2):
		self.operand1=operand1
		self.operand2=operand2
#Checks whether item on the left is an instance of the type on the right
class isOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" ?= "+self.operand2.__repr__()+")"

class dotOp(dyadicOp):
	def C(self):
		return "get_member("+self.operand1.C()+',"'+self.operand2.C()+'")'

	def __repr__(self):
		return "("+self.operand1.__repr__()+" . "+self.operand2.__repr__()+")"

class commaOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" , "+self.operand2.__repr__()+")"

class mulOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" * "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"mul",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class divOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" / "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"div",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class modOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" % "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"mod",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

#Bitwise operations
class bitLshOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" << "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"lsh",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class bitRshOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" >> "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"rsh",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class bitAndOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" & "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"and",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class bitOrOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" | "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"or",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class bitXorOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" ^ "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"xor",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class addOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" + "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"add",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'
class minusOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" - "+self.operand2.__repr__()+")"

class bitNotOp(unaryOp):
	def __repr__(self):
		return "~ "+self.operand.__repr__()

class uAddOp(unaryOp):
	def __repr__(self):
		return "+ "+self.operand.__repr__()

class negOp(unaryOp):
	def __repr__(self):
		return "- "+self.operand.__repr__()
	def C(self):
		return 'call_method_noargs('+self.operand.C()+',"neg")'

class andOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" AND "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"band",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class orOp(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" OR "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"bor",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class notOp(unaryOp):
	def __repr__(self):
		return "NOT "+self.operand.__repr__()
	def C(self):
		return 'call_method_noargs('+self.operand.C()+',"not")'

#For when you want to pass by copy and not by reference
class copyOp(unaryOp):
	def __repr__(self):
		return "@ "+self.operand.__repr__()
	def C(self):
		return 'call_method_noargs('+self.operand.C()+',"copy")'

class unaryAssignment(unaryOp):
	pass

class iPosAssign(unaryAssignment):
	def __repr__(self):
		return "$+ "+self.operand.__repr__()
	def C(self):
		return 'call_method_noargs('+self.operand.C()+',"ipos")'

class iNegAssign(unaryAssignment):
	def __repr__(self):
		return "$- "+self.operand.__repr__()
	def C(self):
		return 'call_method_noargs('+self.operand.C()+',"ineg")'

class iNotAssign(unaryAssignment):
	def __repr__(self):
		return "$! "+self.operand.__repr__()
	def C(self):
		return 'call_method_noargs('+self.operand.C()+',"ibnot")'

class iBitNotAssign(unaryAssignment):
	def __repr__(self):
		return "$~ "+self.operand.__repr__()
	def C(self):
		return 'call_method_noargs('+self.operand.C()+',"inot")'

class assignment(dyadicOp):
	pass

class copyAssign(assignment):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" @= "+self.operand2.__repr__()+")"

class addAssign(assignment):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" += "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"iadd",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class minusAssign(assignment):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" -= "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"isub",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class mulAssign(assignment):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" *= "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"imul",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class divAssign(assignment):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" /= "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"idiv",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class bitAndAssign(assignment):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" &= "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"iand",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class bitOrAssign(assignment):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" |= "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"ior",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class bitXorAssign(assignment):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" ^= "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"ixor",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class bitRshAssign(assignment):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" >>= "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"irsh",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class bitLshAssign(assignment):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" <<= "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"ilsh",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class greaterThan(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" > "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"gt",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class greaterThanEqualTo(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" >= "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"ge",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class lessThan(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" < "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"lt",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class lessThanEqualTo(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" <= "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"le",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class notEqualTo(dyadicOp):
	def __repr__(self):
		return "("+self.operand1.__repr__()+" != "+self.operand2.__repr__()+")"
	def C(self):
		return 'call_method('+self.operand1.C()+',"ne",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'

class retStatement(unaryOp):
	def __repr__(self):
		return "RETURN "+self.operand.__repr__()
	def C(self):
		return "return "+self.operand.C()+";"

#Meaning depends on context, in an expression it is a comparison
#in a function declaration it's a default, otherwise it's an
#assignment
class equalOp(dyadicOp):
	def __init__(self,operand1,operand2):
		super(type(self), self).__init__(operand1,operand2)
		#Assume it to be a comparison unless told otherwise
		self.isCmp=True
		self.isAssign=False
		#Function default
		self.isDefault=False

	def __repr__(self):
		if self.isCmp:
			return "("+self.operand1.__repr__()+" == "+self.operand2.__repr__()+")"
		elif self.isAssign:
			return "("+self.operand1.__repr__()+" = "+self.operand2.__repr__()+")"
		else:
			return "("+self.operand1.__repr__()+" := "+self.operand2.__repr__()+")"

	def C(self):
		if self.isCmp:
			return 'call_method('+self.operand1.C()+',"eq",dyn_array_from(1,(void *[]){'+self.operand2.C()+'}))'
		elif self.isAssign:
			#Do something different if the thing we're assigning to is an item in a list
			if isinstance(self.operand1,arrayIndex):
				return 'call_method('+self.operand1.lstExpr.C()+',"set",dyn_array_from('+str(len(self.operand1.brackExpr)+1)+',(void *[]){'+(",".join([i.C() for i in self.operand1.brackExpr])) +","+self.operand2.C()+'}))'
			elif isinstance(self.operand1,dotOp):
				return 'bind_member('+self.operand1.operand1.C()+',"'+self.operand1.operand2.C()+'",'+self.operand2.C()+')'
			else:
				return self.operand1.C()+'='+self.operand2.C()
		else:
			return ''

class parenthExpr():
	def __init__(self,parenthTok):
		if len(parenthTok)>0:
			self.tree=parenthTok[0]
		else:
			self.tree=None
	def __repr__(self):
		return str(self.tree)

	def C(self):
		if self.tree:
			return "("+self.tree.C()+")"
		else:
			return "()"

class brackExpr(list):
	def __init__(self,tree):
		super(type(self), self).__init__()
		if len(tree)>0:
			self.tree=tree[0]
		else:
			self.tree=None

		current=self.tree
		while isinstance(current,commaOp) or isinstance(current,equalOp):
			if not isinstance(current,commaOp):
				break
			#We need to extract variable names from current
			#Maybe we should make functions to traverse trees and extract items of a given type
			self.insert(0,current.operand2)
			current=current.operand1
		#Fix for empty parameters, make less hacky later
		if current!=tree:
			self.insert(0,current)

	def __repr__(self):
		return str(self.tree)

	def C(self):
		return 'create_array(dyn_array_from('+str(len(self))+',(void *[]){'+(','.join([i.C() for i in self]))+'}))'

class blockExpr(list):
	def __init__(self,tree):
		self.tree=tree

	def __repr__(self):
		return "{\n"+("\n".join([str(i) for i in self.tree]))+"\n}"

	def C(self):
		return "{\n"+("\n".join([i.C()+";" for i in self.tree]))+"\n}"

class parenthTok(list):
	pass

class brackTok(list):
	pass

#Braces enclosed code block
class blockTok(list):
	pass

class arrayIndex():
	def __init__(self,lstExpr,brackExpr):
		self.lstExpr=lstExpr
		self.brackExpr=brackExpr

	def __repr__(self):
		return str(self.lstExpr)+"["+str(self.brackExpr)+"]"

	def C(self):
		return 'call_method('+self.lstExpr.C()+',"get",dyn_array_from('+str(len(self.brackExpr))+',(void *[]){'+(",".join([i.C() for i in self.brackExpr])) +'}))'

class funcCall():
	def __init__(self,name,parenthExpr):
		self.name=name
		self.parenthExpr=parenthExpr
	def __repr__(self):
		return str(self.name)+"("+str(self.parenthExpr)+")"

	def C(self):
		args=[]
		i=self.parenthExpr.tree
		while isinstance(i,commaOp):
			args.insert(0,i.operand2.C())
			i=i.operand1
		if i:
			args.insert(0,i.C())
		if isinstance(self.name,dotOp):
			return "call_method("+self.name.operand1.C()+',"'+self.name.operand2.C()+'",dyn_array_from('+str(len(args))+',(void *[]){'+(','.join(args))+'}))'
		else:
			return "call_function("+self.name.C()+',dyn_array_from('+str(len(args)+1)+',(void *[]){create_none(),'+(','.join(args))+'}))'
class statement():
	pass

#Can still access local variables outside it
#Variables declared inside can be accessed outside
class inheritContextStatement(statement):
	pass

#Doesn't inherit outside local variables
#variables declared inside can't be accessed from outside
class newContextStatement(statement):
	pass

class ifStatement(inheritContextStatement):
	def __init__(self,expr,code):
		self.expr=expr
		self.code=code
	def __repr__(self):
		return "IF "+str(self.expr)+"\n"+str(self.code)
	def C(self):
		return 'if (get_bool_val(call_method_noargs('+self.expr.C()+',"bool")))\n'+self.code.C()

class forStatement(inheritContextStatement):
	def __init__(self,iterator,toIterate,code):
		self.iterator=iterator
		self.toIterate=toIterate
		self.code=code
	def __repr__(self):
		return "FOR "+str(self.iterator)+" IN "+str(self.toIterate)+"\n"+str(self.code)

class whileStatement(inheritContextStatement):
	def __init__(self,expr,code):
		self.expr=expr
		self.code=code
	def __repr__(self):
		return "WHILE "+str(self.expr)+"\n"+str(self.code)

class objStatement(newContextStatement):
	def __init__(self,name,code,parent=None):
		self.localvars=[]
		self.name=name
		self.code=code
		self.parent=parent
		self.parentContext=None
		self.enum=None
		self.fullname="" #Including the path, like enum without the prefix
		self.methods=[]
		self.new=None

	def __repr__(self):
		return "OBJ "+self.name.name+" FROM "+(str(self.parent) if self.parent else "")+""+"\n"+str(self.code)

	def C(self):
		return 'struct dyn_obj *'+self.name.C()+'=*type_factory_list['+self.enum+'];\n'

class funcParamList(list):
	def __init__(self,tree):
		super(type(self), self).__init__()
		current=tree
		while isinstance(current,commaOp) or isinstance(current,equalOp):
			if not isinstance(current,commaOp):
				break
			#We need to extract variable names from current
			#Maybe we should make functions to traverse trees and extract items of a given type
			self.insert(0,funcParam(current.operand2))
			current=current.operand1
		#Fix for empty parameters, make less hacky later
		if current!=tree:
			self.insert(0,funcParam(current))

	def minLen(self):
		return sum([not i.default for i in self])+1

	def maxLen(self):
		return len(self)+1

	def __repr__(self):
		return "("+(", ".join([str(i) for i in self]))+")"

class funcParam():
	def __init__(self,expr):
		self.default=None
		current=expr
		self.type=None
		while current:
			if isinstance(current,variable):
				self.var=current
				break
			elif isinstance(current,isOp):
				self.type=current.operand2
			elif isinstance(current,equalOp):
				self.default=current.operand2
			current=current.operand1


	def __repr__(self):
		return str(self.var)+("?="+str(self.type) if self.type else "")+("="+str(self.default) if self.default else "")

class funcStatement(newContextStatement):
	def __init__(self,prototype,code):
		self.localvars=[]
		self.prototype=prototype
		self.name=self.prototype.name
		self.code=code
		self.parentContext=None
		self.cfunc=None

		self.param=funcParamList(self.prototype.parenthExpr.tree)

	def minLen(self):
		return self.param.minLen()

	def maxLen(self):
		return self.param.maxLen()

	#This just binds the real name of the C function to the object with bind_method
	def C(self):
		return 'struct dyn_obj *'+self.name.C()+' = create_function('+self.cfunc+');\n'

	def __repr__(self):
		return "FUNCTION "+str(self.prototype)+"\n"+str(self.code)

class elseStatement(inheritContextStatement):
	def __init__(self,initial,secondary):
		self.initial=initial
		self.secondary=secondary
	def __repr__(self):
		return "\n"+str(self.initial)+"\nELSE\n"+str(self.secondary)

class ast():
	def __init__(self,tokens,sourcecode):
		self.cursor=0
		self.tree=tokens
		self.definedObjects=[]
		self.definedFunctions=[]
		#Left to right or right to left
		self.ltr=True
		self.sourcecode=sourcecode.split("\n")

	def __repr__(self):
		return "\n".join([str(i) for i in self])

	def __getitem__(self,a):
		if a>=0 and a<len(self):
			return self.tree[a]
		else:
			raise IndexError()

	def __len__(self):
		return len(self.tree)

	def __iter__(self):
		if self.ltr:
			self.cursor=-1
			return self
		else:
			self.cursor=len(self.tree)
			return self

	def __next__(self):
		if self.ltr:
			if self.cursor<len(self)-1:
				self.cursor+=1
				return self[self.cursor]
			else:
				raise StopIteration()
		else:
			if self.cursor>0:
				self.cursor-=1
				return self[self.cursor]
			else:
				raise StopIteration()


	def consumeSubExpr(self,obj,noAssign=False):
		self.tree.insert(self.cursor,obj(parseLayer(self[self.cursor],noAssign)))
		self.tree.pop(self.cursor+1)

	def consumeItem(self,obj,callFirst=None):
		self.tree.insert(self.cursor,obj(self[self.cursor][1]))
		self.tree.pop(self.cursor+1)

	def isUnary(self):
		return self.cursor-1<0 or type(self[self.cursor-1]) is tuple

	def consumeUnary(self,obj):
		if type(self[self.cursor+1]) is tuple:
			print("\033[91mFatal error:\033[m Invalid syntax at line "+str(self[self.cursor+1][3])+"\n"+
				self.sourcecode[self[self.cursor+1][3]]+"\n"+
				(" "*(self[self.cursor+1][2]-1))+"^",file=sys.stderr)
			exit(255)
		self.tree.insert(self.cursor,obj(self[self.cursor+1]))
		self.tree.pop(self.cursor+1)
		self.tree.pop(self.cursor+1)

	def consumeDyadic(self,obj):
		if type(self[self.cursor+1]) is tuple:
			print("\033[91mFatal error:\033[m Invalid syntax at line "+str(self[self.cursor+1][3])+"\n"+
				self.sourcecode[self[self.cursor+1][3]]+"\n"+
				(" "*(self[self.cursor+1][2]-1))+"^",file=sys.stderr)
			exit(255)
		elif type(self[self.cursor-1]) is tuple:
			print("\033[91mFatal error:\033[m Invalid syntax at line "+str(self[self.cursor-1][3])+"\n"+
				txt.split("\n")[self[self.cursor-1][3]]+"\n"+
				(" "*(self[self.cursor-1][2]-1))+"^",file=sys.stderr)
			exit(255)

		self.tree.insert(self.cursor,obj(self[self.cursor-1],self[self.cursor+1]))
		self.tree.pop(self.cursor-1)
		self.tree.pop(self.cursor)
		self.tree.pop(self.cursor)
		self.cursor-=1

	def C(self):
		return ''.join([i.C()+";" for i in self])

#Parses a parenthesis layer (including the root)
def parseLayer(tokens,noAssign=False):
	#noAssign is whether or not this layer can have
	#= as an assignment
	global txt
	tree=ast(tokens,txt)
	for i in tree:
		if type(i) is parenthTok:
			tree.consumeSubExpr(parenthExpr,True)
		elif type(i) is brackTok:
			tree.consumeSubExpr(brackExpr,True)
		elif type(i) is blockTok:
			tree.consumeSubExpr(blockExpr)

	for i in tree:
		if type(i) is tuple:
			if i[0]=="id":
				if i[1] not in ["from","and","or","not","if","obj","fn","ret","for","while","import","else"]:
					tree.consumeItem(variable)
	for i in tree:
		if type(i) is tuple:
			if i[0]=="str":
				tree.consumeItem(strLiteral)
			elif i[0]=="int":
				tree.consumeItem(intLiteral)
			elif i[0]=="flt":
				tree.consumeItem(fltLiteral)
			elif i[0]=="bin":
				tree.consumeItem(binLiteral)
			elif i[0]=="hex":
				tree.consumeItem(hexLiteral)

	for i in tree:
		if type(i) is tuple:
			if i[0]=="mdop":
				if i[1]==".":
					tree.consumeDyadic(dotOp)
		#Array indexing
		elif (not tree.isUnary()) and isinstance(tree[tree.cursor],brackExpr):
			tree.tree.insert(tree.cursor,arrayIndex(tree[tree.cursor-1],tree[tree.cursor]))
			tree.tree.pop(tree.cursor-1)
			tree.tree.pop(tree.cursor)
			tree.cursor-=1
		#Function calls
		elif (not tree.isUnary()) and isinstance(tree[tree.cursor],parenthExpr):
			tree.tree.insert(tree.cursor,funcCall(tree[tree.cursor-1],tree[tree.cursor]))
			tree.tree.pop(tree.cursor-1)
			tree.tree.pop(tree.cursor)
			tree.cursor-=1

	for i in tree:
		if type(i) is tuple:
			if i[0]=="dop":
				if i[1]=="*":
					tree.consumeDyadic(mulOp)
				elif i[1]=="/":
					tree.consumeDyadic(divOp)
				elif i[1]=="%":
					tree.consumeDyadic(modOp)


	for i in tree:
		if type(i) is tuple:
			if i[0]=="dop":
				if i[1]==">>":
					tree.consumeDyadic(bitRshOp)
				elif i[1]=="<<":
					tree.consumeDyadic(bitLshOp)

	for i in tree:
		if type(i) is tuple:
			if i[0]=="dop":
				if i[1]=="&":
					tree.consumeDyadic(bitAndOp)

	for i in tree:
		if type(i) is tuple:
			if i[0]=="dop":
				if i[1]=="^":
					tree.consumeDyadic(bitXorOp)

	for i in tree:
		if type(i) is tuple:
			if i[0]=="dop":
				if i[1]=="|":
					tree.consumeDyadic(bitOrOp)

	tree.ltr=False
	for i in tree:
		#print(i)
		if type(i) is tuple:
			if i[0]=="uaiop":
				if i[1]=="$+": #Should I just remove this one? It doesn't do anything
					tree.consumeUnary(iPosAssign)

				elif i[1]=="$-":
					tree.consumeUnary(iNegAssign)

				elif i[1]=="$~":
					tree.consumeUnary(iBitNotAssign)
				elif i[1]=="$!":
					tree.consumeUnary(iNotAssign)


			elif i[0]=="udop":
				if i[1]=="+":
					if tree.isUnary():
						tree.consumeUnary(uAddOp)
					else:
						tree.consumeDyadic(addOp)
				elif i[1]=="-":
					if tree.isUnary():
						tree.consumeUnary(negOp)
					else:
						tree.consumeDyadic(minusOp)
			elif i[0]=="uop":
				if i[1]=="~":
					if tree.isUnary():
						tree.consumeUnary(bitNotOp)
			elif i[0]=="uadop":
				if i[1]=="!":
					if tree.isUnary():
						tree.consumeUnary(notOp)
				elif i[1]=="@":
					if tree.isUnary():
						tree.consumeUnary(copyOp)
	tree.ltr=True
	for i in tree:
		if type(i) is tuple:
			if i[0]=="id":
				if i[1]=="and":
						tree.consumeDyadic(andOp)
				elif i[1]=="or":
						tree.consumeDyadic(orOp)

	for i in tree:
		if type(i) is tuple:
			if i[0]=="dop":
				if i[1]=="?=":
					tree.consumeDyadic(isOp)
				elif i[1]=="!=":
					tree.consumeDyadic(notEqualTo)
				elif i[1]=="<=":
					tree.consumeDyadic(lessThanEqualTo)
				elif i[1]==">=":
					tree.consumeDyadic(greaterThanEqualTo)
				elif i[1]=="<":
					tree.consumeDyadic(lessThan)
				elif i[1]==">":
					tree.consumeDyadic(greaterThan)
	tree.ltr=False
	for i in tree:
		if type(i) is tuple:
			if i[0]=="mdop":
				if i[1]=="=":
					tree.consumeDyadic(equalOp)
	tree.ltr=True
	if not noAssign:
		for i in tree:
			if type(i) is tuple:
				if i[0]=="dop":
					if i[1]=="@=":
						tree.consumeDyadic(copyAssign)
					elif i[1]=="+=":
						tree.consumeDyadic(addAssign)
					elif i[1]=="-=":
						tree.consumeDyadic(minusAssign)
					elif i[1]=="*=":
						tree.consumeDyadic(mulAssign)
					elif i[1]=="/=":
						tree.consumeDyadic(divAssign)
					elif i[1]=="&=":
						tree.consumeDyadic(bitAndAssign)
					elif i[1]=="^=":
						tree.consumeDyadic(bitXorAssign)
					elif i[1]=="|=":
						tree.consumeDyadic(bitOrAssign)
					elif i[1]==">>=":
						tree.consumeDyadic(bitRshAssign)
					elif i[1]=="<<=":
						tree.consumeDyadic(bitLshAssign)

	for i in tree:
		if type(i) is tuple:
			if i[0]=="mdop":
				if i[1]==",":
					tree.consumeDyadic(commaOp)
	for i in tree:
		if type(i) is tuple:
			if i[0]=="id":
				if i[1]=="ret":
					tree.consumeUnary(retStatement)

	tree.ltr=False
	for i in tree:
		if type(i) is tuple:
			if i[0]=="id":
				if i[1]=="if":
					tree.tree.insert(tree.cursor,ifStatement(tree[tree.cursor+1],tree[tree.cursor+2]))
					tree.tree.pop(tree.cursor+1)
					tree.tree.pop(tree.cursor+1)
					tree.tree.pop(tree.cursor+1)
				elif i[1]=="for":
					tree.tree.insert(tree.cursor,forStatement(tree[tree.cursor+1],tree[tree.cursor+2],tree[tree.cursor+3]))
					tree.tree.pop(tree.cursor+1)
					tree.tree.pop(tree.cursor+1)
					tree.tree.pop(tree.cursor+1)
					tree.tree.pop(tree.cursor+1)
				elif i[1]=="while":
					tree.tree.insert(tree.cursor,whileStatement(tree[tree.cursor+1],tree[tree.cursor+2]))
					tree.tree.pop(tree.cursor+1)
					tree.tree.pop(tree.cursor+1)
					tree.tree.pop(tree.cursor+1)
				#These two things below will create a new namespace, as a result we must
				#provide the parent namespace with this information
				#We'll have to see if the code contained within the object will create another namespace
				#either by checking the contents of the subast's definedObjects and definedFunctions
				#or by checking directly if those are functions or objects (because these are the only things)
				#that create new namespaces
				#Maybe make a method for checking if things create new namespaces
				elif i[1]=="obj":
					if (type(tree[tree.cursor+2]) is tuple) and tree[tree.cursor+2][0]=="id" and tree[tree.cursor+2][1]=="from":
						tree.tree.insert(tree.cursor,objStatement(tree[tree.cursor+1],tree[tree.cursor+4],tree[tree.cursor+3]))
						tree.definedObjects.append(tree[tree.cursor])
						tree.tree.pop(tree.cursor+1)
						tree.tree.pop(tree.cursor+1)
						tree.tree.pop(tree.cursor+1)
						tree.tree.pop(tree.cursor+1)
						tree.tree.pop(tree.cursor+1)
					else:
						tree.tree.insert(tree.cursor,objStatement(tree[tree.cursor+1],tree[tree.cursor+2]))
						tree.definedObjects.append(tree[tree.cursor])
						tree.tree.pop(tree.cursor+1)
						tree.tree.pop(tree.cursor+1)
						tree.tree.pop(tree.cursor+1)
				elif i[1]=="fn":
					tree.tree.insert(tree.cursor,funcStatement(tree[tree.cursor+1],tree[tree.cursor+2]))
					tree.definedFunctions.append(tree[tree.cursor])
					tree.tree.pop(tree.cursor+1)
					tree.tree.pop(tree.cursor+1)
					tree.tree.pop(tree.cursor+1)

	tree.ltr=True
	for i in tree:
		if type(i) is tuple:
			if i[0]=="id":
				if i[1]=="else":
					tree.consumeDyadic(elseStatement)

	#Take note of assignments
	if not noAssign:
		for i in tree:
			if type(i) is equalOp:
				i.isCmp=False
				i.isAssign=True
				i.isDefault=False
	return tree

def handleBrack(lst):
	toRet=[]
	i=0
	while i < len(lst):
		if lst[i][0]=="oparen":
			returned=handleParen(lst[i+1:])
			toRet.append(returned[0])
			i+=returned[1]+1
		elif lst[i][0]=="obrace":
			returned=handleBlocks(lst[i+1:])
			toRet.append(returned[0])
			i+=returned[1]+1
		elif lst[i][0]=="obrack":
			returned=handleBrack(lst[i+1:])
			toRet.append(returned[0])
			i+=returned[1]+1
		elif lst[i][0]=="cbrack":
			return brackTok(toRet),i
		else:
			toRet.append(lst[i])
		i+=1
	return parenthTok(toRet)

#Creates a parenthesis tree
def handleParen(lst):
	toRet=[]
	i=0
	while i < len(lst):
		if lst[i][0]=="obrace":
			returned=handleBlocks(lst[i+1:])
			toRet.append(returned[0])
			i+=returned[1]+1
		elif lst[i][0]=="obrack":
			returned=handleBrack(lst[i+1:])
			toRet.append(returned[0])
			i+=returned[1]+1
		elif lst[i][0]=="oparen":
			returned=handleParen(lst[i+1:])
			toRet.append(returned[0])
			i+=returned[1]+1
		elif lst[i][0]=="cparen":
			return parenthTok(toRet),i
		else:
			toRet.append(lst[i])
		i+=1
	return parenthTok(toRet)

#Creates a code block tree
def handleBlocks(lst):
	toRet=[]
	i=0
	while i < len(lst):
		if lst[i][0]=="oparen":
			returned=handleParen(lst[i+1:])
			toRet.append(returned[0])
			i+=returned[1]+1
		elif lst[i][0]=="obrack":
			returned=handleBrack(lst[i+1:])
			toRet.append(returned[0])
			i+=returned[1]+1
		elif lst[i][0]=="obrace":
			returned=handleBlocks(lst[i+1:])
			toRet.append(returned[0])
			i+=returned[1]+1
		elif lst[i][0]=="cbrace":
			return blockTok(toRet),i
		else:
			toRet.append(lst[i])
		i+=1
	return blockTok(toRet)

#Grabs all the objects that a variable can be (in addition to builtins)
#Functions are actually objects, we need to wrap them
#We will use this to generate the variant type


parsedSource=parseLayer(handleBlocks(tokenise()))
#print(parseLayer(handleBlocks(tokenise())))
class declGen():
	def __init__(self,syntree,context=[]):
		self.tree=syntree
		#This name list should seperate the actual object and its parents
		#We've changed this so it generates the function declarations and object declarations using one object with different methods
		self.names=[]
		if isinstance(syntree,equalOp):
			self.names.append(context+[syntree.operand1])
		elif isinstance(syntree,blockExpr) or isinstance(syntree,ast):
			for i in syntree.tree:
				if isinstance(i,equalOp):
					self.names.append(context+[i.operand1])
				elif isinstance(i,objStatement):
					self.names.append(context+[i])
					self.names.extend(declGen(i.code,context+[i]))
				elif isinstance(i,funcStatement):
					self.names.append(context+[i])
					self.names.extend(declGen(i.code,context+[i]))
				elif isinstance(i,inheritContextStatement):
					self.names.extend(declGen(i.code,context))
				elif isinstance(i,blockExpr):
					self.names.extend(declGen(i,context))
		elif isinstance(syntree,objStatement):
			self.names.append(context+[syntree])
			self.names.extend(declGen(syntree.code,context+[syntree]))
		elif isinstance(syntree,funcStatement):
			self.names.append(context+[syntree])
			self.names.extend(declGen(syntree.code,context+[syntree]))
		elif isinstance(syntree,inheritContextStatement):
			self.names.extend(declGen(syntree.code,context))

		self.objNames=[i for i in self.names if isinstance(i[-1],objStatement)]

	def __getitem__(self,a):
		return self.names[a]

	def __iter__(self):
		self.index=-1
		return self

	def __len__(self):
		return len(self.names)

	def __next__(self):
		if self.index<len(self)-1:
			self.index+=1
			return self[self.index]
		else:
			raise StopIteration()

	def genObjEnums(self): #Declare the enums for internal use
		genedEnums=[]
		for i in self.names:
			if isinstance(i[-1],objStatement):
				i[-1].fullname="_".join([str(ii.name) for ii in i])
				i[-1].enum="OBJ_"+i[-1].fullname
				genedEnums.append(i[-1].enum)
		return "\t,\n\t"+(",\n\t".join(genedEnums))+"\n"

	def genObjNames(self): #Declare type_names with the correct values
		return '\t,\n\t"'+('",\n\t"'.join([str(i[-1].name) for i in self.names if isinstance(i[-1],objStatement)]))+'"\n'

	def genHeader(self):
		to_ret="#ifndef MAIN_H\n#define MAIN_H\n"
		for i in self.names:
			if isinstance(i[-1],objStatement):
				to_ret+='extern struct method_list OBJ_'+('_'.join([str(ii.name) for ii in i]))+'_methods;\n'
				to_ret+='extern struct method_list factory_OBJ_'+('_'.join([str(ii.name) for ii in i]))+'_methods;\n'
		to_ret+="#endif\n"
		return to_ret

	def genObjMethodListRefs(self):
		if self.objNames:
			return '\t,\n\t&'+(',\n\t&'.join(["OBJ_"+"_".join([str(ii.name) for ii in i])+"_methods" for i in self.names if isinstance(i[-1],objStatement)]))+'\n'
		else:
			return ""

	def genObjFactoryMethodListRefs(self):
		if self.objNames:
			return '\t,\n\t&'+(',\n\t&'.join(["factory_OBJ_"+"_".join([str(ii.name) for ii in i])+"_methods" for i in self.names if isinstance(i[-1],objStatement)]))+'\n'
		else:
			return ""
	def genMain(self):
		to_ret='#include <stdio.h>\n#include "main.h"\n#include <gc.h>\n#include "global_obj.h"\n#include "dyn_objs.h"\n#include "factory_obj.h"\n#include "func_obj.h"\n#include "int_obj.h"\n#include "str_obj.h"\n#include "type_obj.h"\n#include "bool_obj.h"\n#include "none_obj.h"\n#include "array_obj.h"\n#include "debug.h"\n'
		for i in self.names:
			if isinstance(i[-1],objStatement):
				has_new=False
				for ii in self.names:
					if isinstance(ii[-1],funcStatement):
						if ii[-1].name.name=="new":
							i[-1].new=ii
						elif i==ii[:-1]:
							i[-1].methods.append(ii)
				if not i[-1].new:
					to_ret+="decl_dyn_fn(FN_"+i[-1].fullname+"_new);\n"
		declared_vars={}
		#Backwards declarations
		for i in self.names:
			if isinstance(i[-1],funcStatement):
				to_ret+='decl_dyn_fn(FN_'+("_".join([str(ii.name) for ii in i]))+');\n'
			elif isinstance(i[-1],variable):
				if ".".join([str(ii.name) for ii in i]) not in declared_vars:
					if len(i)==1:
						to_ret+='struct dyn_obj *'+i[-1].C()+";"
					else:
						i[-2].localvars.append(i[-1])
					declared_vars[".".join([str(ii.name) for ii in i])]=True

		#Method lists
		for i in self.names:
			if isinstance(i[-1],objStatement):
				to_ret+="struct method_list "+i[-1].enum+"_methods = {\n"
				to_ret+="\t"+str(len(i[-1].methods)+1)+",\n\t{\n"
				for ii in i[-1].methods:
					to_ret+='\t\t{"'+str(ii[-1].name)+'",FN_'+("_".join([str(iii.name) for iii in ii]))+"},\n"
				to_ret+='\t\t{"new",FN_'+i[-1].fullname+"_new},\n"
				to_ret+="\t}\n};\n\n"

				to_ret+="struct method_list factory_"+i[-1].enum+"_methods = {\n"
				to_ret+="\t"+str(len(i[-1].methods)+1)+",\n\t{\n"
				for ii in i[-1].methods:
					to_ret+='\t\t{"'+str(ii[-1].name)+'",factory_FN_'+("_".join([str(iii.name) for iii in ii]))+"},\n"
				to_ret+='\t\t{"new",factory_FN_'+i[-1].fullname+"_new},\n"
				to_ret+="\t}\n};\n\n"

		for i in self.names:
			if isinstance(i[-1],objStatement):
				to_ret+="void create_"+i[-1].enum+"_factory()\n{\n\tobject_setup(FACTORY);\n\t((struct factory_obj*)self)->type_to_create="+i[-1].enum+';\n\tbind_member(self,"parent",'+(i[-1].parent.C() if i[-1].parent else "type_factory")+');\n\ttype_factory_list['+i[-1].enum+']=&self;\n\tinherit_factory_setup();\n}\n'

		#Method implementations
		#Search for things in our list that match [PATH TO THE OBJECT][A FUNCTION]
		def funcGen(ii):
			to_ret=""
			to_ret+=("def_dyn_fn(FN_"+
				("_".join([str(iii.name) for iii in ii]))+
				')\n{\n\t#ifdef DEBUG\n\t\targ_guard('+str(ii[-1].minLen())+','+str(ii[-1].maxLen())+',protect({"self"'+
				(''.join([',"'+str(iii.var)+'"' for iii in ii[-1].param]))+
				'}),protect({TYPE'+(''.join([(",((struct factory_obj*)"+str(iii.type.C())+")->type_to_create") if iii.type else ",TYPE" for iii in ii[-1].param]))+
				'}));\n\t#endif\n')
			to_ret+='\tstruct dyn_obj* self=get_arg(0);\n'
			for iii,j in enumerate(ii[-1].param):
				if j.default:
					to_ret+='\tstruct dyn_obj* '+str(j.var)+'=args->filled>='+str(iii+1)+'?get_arg('+str(iii+1)+'):'+j.default.C()+';\n'
				else:
					to_ret+='\tstruct dyn_obj* '+str(j.var)+'=get_arg('+str(iii+1)+');\n'
			to_ret+="".join(["struct dyn_obj *"+iii.C()+";\n" for iii in ii[-1].localvars])
			to_ret+=ii[-1].code.C()+"\n}\n\n"
			return to_ret

		for i in self.names:
			if isinstance(i[-1],funcStatement):
				i[-1].cfunc="FN_"+("_".join([str(ii.name) for ii in i]))

		for i in self.names:
			if isinstance(i[-1],objStatement):
				for ii in self.names:
					if i==ii[:-1] and isinstance(ii[-1],funcStatement) and ii[-1].name.name!="new":
						to_ret+=funcGen(ii)

				if i[-1].new:
					to_ret+="def_dyn_fn(FN_"+i[-1].fullname+'_new)\n{\n\t#ifdef DEBUG\n\t\targ_guard('+str(i[-1].new[-1].minLen())+','+str(i[-1].new[-1].maxLen())+',protect({"self"'+(''.join([',"'+str(ii.var)+'"' for ii in i[-1].new[-1].param]))+'}),protect({TYPE'+(''.join([(",((struct factory_obj*)"+str(ii.type.C())+")->type_to_create") if ii.type else ",TYPE" for ii in i[-1].new[-1].param]))+'}));\n\t#endif\n\tobject_setup('+i[-1].enum+');\n\tbind_member(self,"parent",*type_parent_list[self->cur_type]);\n\tinherit_setup();\n'+i[-1].new[-1].code.C()+'\n}\n'
				else:
					to_ret+="def_dyn_fn(FN_"+i[-1].fullname+'_new)\n{\n\tstruct dyn_obj *self = call_method(*type_parent_list[self->cur_type],"new",args);\n\tself->cur_type = '+i[-1].enum+';\n\tbind_member(self,"parent",*type_parent_list[self->cur_type]);\n\tinit_methods(self,type_method_lists[self->cur_type]);\n\treturn self;\n}\n'
			elif (isinstance(i[-1],funcStatement) and len(i)==1) or (len(i)>1 and isinstance(i[-1],funcStatement) and isinstance(i[-2],funcStatement)): #Top level global functions
				to_ret+=funcGen(i)
		to_ret+="int main()\n{\n\tGC_INIT();\n\tcreate_global();\n\tstruct dyn_obj *self;\n\tself=global;\n"
		for i in self.names:
			if isinstance(i[-1],objStatement):
				to_ret+="create_"+i[-1].enum+"_factory();"
		to_ret+=self.tree.C()+"\n}\n"
		return to_ret

	def genObjSizes(self):
		if self.objNames:
			#The objects with parents are going to have their sizes changed at runtime when their factory objects are created
			return "\t,\n\t"+",\n\t".join(["sizeof(struct type_obj)" for i in self.names if isinstance(i[-1],objStatement)])
		else:
			return ""
#print(parsedSource)
d=declGen(parsedSource)
with open("lib/parserdata/obj_enums.txt","w") as f:
	f.write(d.genObjEnums())
with open("lib/parserdata/obj_sizes.txt","w") as f:
	f.write(d.genObjSizes())
with open("lib/parserdata/obj_names.txt","w") as f:
	f.write(d.genObjNames())
with open("lib/parserdata/obj_method_list_refs.txt","w") as f:
	f.write(d.genObjMethodListRefs())
with open("lib/parserdata/obj_factory_method_list_refs.txt","w") as f:
	f.write(d.genObjFactoryMethodListRefs())
with open("lib/parserdata/main.h","w") as f:
	f.write(d.genHeader())
with open("lib/parserdata/main.c","w") as f:
	f.write(d.genMain())
os.system("sh -c 'cd lib;make clean;make'")
