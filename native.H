//
// Basic scheme interpreter
//
// Copyright 2012 Jose Luis P. Cardenas (jluis.pcardenas@gmail.com)
//
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef _NATIVE_H_
#define _NATIVE_H_ 0

#include <list>
#include <fstream>
#include "closure.h"
#include "environment.h"
#include "procedure.h"
#include "sboolean.h"
#include "sstring.h"
#include "util.h"

#define check_argument_type(f, t, a) if (!util::is<t>(a)) \
	throw scheme_exception(util::format("error in f: expected type #t, got '%s' '%s'.", util::get_type(a)));
#define call_native(p, a, e) ((native*)scheme::vsymbols->get(p))->invoke(a, e)
#define ARG(x) x.front(); x.pop_front();
#define ARG2DOUBLE(x) atof((((sstring*)x.front())->to_string()).c_str()); x.pop_front();
class native : public procedure
{
public:

	native(std::string id, object* (*proc)(std::list<object*>, environment* env), int min, int max) : procedure(id, NULL, env)
	{
		this->proc = proc;
		this->min = min;
		this->max = max;
	}

	native(std::string id, object* (*proc)(std::list<object*>, environment* env), int min) : procedure(id, NULL, env)
	{
		this->proc = proc;
		this->min = min;
		this->max = -1;
	}

	std::string to_string()
	{
		return util::format("#<procedure-%s>", this->id.c_str());
	}
	static object* apply(std::list<object*> lst, environment* env);

	object* invoke(std::list<object*> args, environment* env);


	static object* car(std::list<object*> lst, environment* env);

	static object* cdr(std::list<object*> lst, environment* env);

	static object* cons(std::list<object*> lst, environment* env);

	static object* define(std::list<object*> lst, environment* env);
	static object* display(std::list<object*> lst, environment* env);

	static object* equal(std::list<object*> lst, environment* env);

	static object* _if(std::list<object*> lst, environment* env);

	static object* lambda(std::list<object*> lst, environment* env);

	static object* list(std::list<object*> lst, environment* env);

	static object* islist(std::list<object*> lst, environment* env);

	static object* isnull(std::list<object*> lst, environment* env);
	static object* length(std::list<object*> lst, environment* env);
	static object* load(std::list<object*> lst, environment* env);

//	static object* macro(std::list<object*> lst, environment* env);

	static object* quote(std::list<object*> lst, environment* env);

	static object* binary_op(std::string op, std::list<object*> lst, environment* env);
private:
	object* (*proc)(std::list<object*> l, environment* env);
	int min;
	int max;                   
};
#endif