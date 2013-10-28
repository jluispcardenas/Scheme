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


#include "native.h"
#include "scheme.h"

/*
native::native(std::string id, object* (*proc)(std::list<object*>, environment* env), int min, int max)
{
	this->proc = proc;
	this->min = min;
	this->max = max;
}*/

 object* native::invoke(std::list<object*> args, environment* env)
{
	if (min != -1 && (max == -1 && args.size() != min))
		throw scheme_exception(util::format("error in %s: incorrect number of arguments to procedure.", id.c_str()));

	if (this->proc == NULL)
		return native::binary_op(id, args, env);

	return this->proc(args, env);
}

object* native::apply(std::list<object*> args, environment* env)
{
	check_argument_type("apply", procedure, args.front()); 
	object* arg0 = ARG(args);
	check_argument_type("apply", pair, args.front()); 

	std::list<object*> nargs = scheme::va_list((pair*)args.front(), env, false);
	nargs.pop_back();

	return ((procedure*)arg0)->invoke(nargs, env);
}

object* native::car(std::list<object*> args, environment* env)
{
	if (!util::is<pair>(args.front()))
		throw scheme_exception(util::format("error in car: expected type pair, got '%s' '%s'.", util::get_type(args.front()), (args.front())->to_string().c_str()));
	return ((pair*)args.front())->car;
}

object* native::cdr(std::list<object*> args, environment* env)
{
	check_argument_type("cdr", pair, args.front());
	object*  o = ((pair*)args.front())->cdr;
	
	return o;
}
           
object* native::cons(std::list<object*> args, environment* env)
{
	pair* arg0 = (pair*)ARG(args);
	pair* arg1 = (pair*)ARG(args);
	return new pair(arg0, arg1);
}

object* native::define(std::list<object*> args, environment* env)
{
	object* arg0 = ARG(args);
	object* arg1 = ARG(args);

	if (util::is<sstring>(arg0))
	{
		sstring* name = ((sstring*)arg0);
		env->extend((std::string)*name, scheme::evaluate(arg1, env));
		return name;
	}
	else if (util::is<pair>(arg0))
	{
		sstring* name = (sstring*)((pair*)arg0)->car;
		std::list<object*> lst;
		lst.push_back(((pair*)arg0)->cdr);
		lst.push_back(arg1);
		env->extend((std::string)*name, call_native("lambda", lst, env));

		return name;
	}
	else
	{
		throw scheme_exception("error in define: ll-formed special form");
	}
}

object* native::display(std::list<object*> args, environment* env)
{
	object* arg0 = ARG(args);
	std::cout << arg0->to_string() << std::endl;
	return NULL;
}

object* native::equal(std::list<object*> args, environment* env)
{
	object* arg0 = ARG(args);
	object* arg1 = ARG(args);

	if (util::get_type(arg0) != util::get_type(arg1))
	{
		return new sboolean(false);
	}
	else if (util::is<pair>(arg0))
	{
		std::list<object*> la1;
		la1.push_back(arg0);
		std::list<object*> la2;
		la2.push_back(arg1);

		sstring* l = (sstring*)call_native("length", la1, env);
		sstring* l2 = (sstring*)call_native("length", la2, env);

		if (((std::string)*l) == ((std::string)*l2))
		{
			pair* t = (pair*)arg0;
			pair* t2 = (pair*)arg1;
			while (t != NULL && t->car != NULL)
			{
				std::list<object*> la;
				la.push_back(t->car);
				la.push_back(t2->car);

				if (!(bool)*((sboolean*)call_native("equal?", la, env)))
					return new sboolean(false);
				t = t->cdr;
				t2 = t2->cdr;
			}

			return new sboolean(true);
		}
	}
	else
	{
		return new sboolean(arg0->to_string() == arg1->to_string());
	}

	return new sboolean(false);
} 

object* native::_if(std::list<object*> args, environment* env)
{                      
	object* predicate = ARG(args);
	object* proc1 = ARG(args);
	object* proc2 = ARG(args);

	object* ret = scheme::evaluate(predicate, env);

	return (!util::is<sboolean>(ret) || util::is<sboolean>(ret) && *((sboolean*)ret) == true) 
		? scheme::evaluate(proc1, env) : scheme::evaluate(proc2, env);
}

object* native::lambda(std::list<object*> args, environment* env)
{
	object* arg0 = ARG(args);
	object* arg1 = ARG(args);

	return (object*)new closure(arg0, arg1, env);
}

object* native::load(std::list<object*> args, environment* env)
{
	check_argument_type("load", sstring, args.front());                   
	object* arg0 = ARG(args);
	std::string path = arg0->to_string();
	std::ifstream file;
	file.open (path.c_str(), std::ifstream::in);
	if (file.good())
	{
		scheme::read_input(&file, NULL, env);
	}
	else
	{
		throw scheme_exception("unable to open file");
	}
	file.close();

	return NULL;
}

object* native::length(std::list<object*> args, environment* env)
{
	check_argument_type("length", pair, args.front());                   

	pair* p = (pair*)ARG(args);
	int len = 0;
	while ((util::is<pair>(p)) && p->car != NULL)
	{
		len++;
		p = p->cdr;
	}

	return new sstring(util::to_string(len));
}

object* native::list(std::list<object*> args, environment* env)
{
	args.push_back(NULL);
	pair *p = NULL, *pt = NULL;
	while (!args.empty())
	{
		object* o = ARG(args);
		if (p == NULL) {
			p = new pair(o, NULL);
			pt = p;
		}
		else
		{
			pt->cdr = new pair(o, NULL);
			pt = pt->cdr;
		}
	}

	return (object*)p;   
}

object* native::islist(std::list<object*> args, environment* env)
{
	return new sboolean(util::is<pair>(args.front()));
}

object* native::isnull(std::list<object*> args, environment* env)
{
	object* arg0 = ARG(args);
	bool b = arg0 == NULL || (util::is<pair>(arg0) && ((pair*)arg0)->car == NULL);

	return new sboolean(b);
}


/*object* native::macro(std::list<object*> args, environment* env)
{
	object* arg0 = ARG(args);
	object* arg1 = ARG(args);

	return (object*)new _macro(arg0, arg1, env);
}
*/
object* native::quote(std::list<object*> args, environment* env)
{
	return call_native("list", args, env);
}


object* native::binary_op(std::string op, std::list<object*> args, environment* env)
{
	double n1 = 0.00;
	if (args.size() > 0)
		n1 = ARG2DOUBLE(args);

	if (op[0] == '<' || op[0] == '>' || op.find('=') != std::string::npos)  
	{
		double n2 = ARG2DOUBLE(args);   

		if (op == ">")
			return new sboolean(n1 > n2);
		else if (op == "<")
			return new sboolean(n1 < n2);
		else if (op == ">=")
			return new sboolean(n1 >= n2);
		else if (op == "<=")
			return new sboolean(n1 <= n2);
		else
			return new sboolean(n1 == n2);     
	}
	else
	{

		while (args.size() > 0)
		{
			double n = ARG2DOUBLE(args); 
			if (op == "+")
				n1 += n;
			else if (op == "-")
				n1 -= n;
			else if (op == "*")
				n1 *= n;
			else if (op == "/")
				n1 /= n;
		}
	}

	return new sstring(util::to_string(n1));
}