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


#include "scheme.h"

symbols* scheme::vsymbols(NULL);

object* scheme::evaluate(object* exp, environment* env)
{
	if (util::is_atom(exp))
	{
		if (exp == NULL || util::is_string(exp) || util::is_number(exp)) // constant
		{
			return exp;
		}
		else if (vsymbols->get((std::string)*((sstring*)exp)) != NULL) // symbol
		{
			return  vsymbols->get((std::string)*((sstring*)exp));
		}
		else
		{
			return env->lookup((std::string)*((sstring*)exp));
		}
	}
	else if (util::is<pair>(exp)) // application
	{
		return apply((std::string)*((sstring*)((pair*)exp)->car), ((pair*)exp)->cdr, env);
	}
	else
	{
		throw scheme_exception(util::format("Unknown expression type -- %s.", ((std::string)*((sstring*)exp)).c_str()));
	}
}


object* scheme::apply(std::string method, pair* arguments, environment *env)
{
	if (vsymbols->get(method) != NULL && util::is<procedure>(vsymbols->get(method)))
	{
		std::string specials[] = {"if", "define", "cond", "macro", "quote", "lambda"}; 
		bool eval = array::index_of(specials, 6, method) == -1;

		std::list<object*> args = scheme::va_list(arguments, env, eval);
		return ((native*)vsymbols->get(method))->invoke(args, env);
	}
	else if (util::is<closure>(env->lookup(method)))
	{
		closure* proc = (closure*)env->lookup(method);
		std::list<object*> args = scheme::va_list(arguments, env, true);
		return proc->invoke(args, env);
	}
	/*else if (util::is<_macro>(env->lookup(method)))
	{
	_macro* proc = (_macro*)env->lookup(method);
	return proc->invoke(arguments, env);
	}*/
	else
	{
		throw scheme_exception(util::format("unknown procedure type -- %s", method.c_str()));
	}
}

std::list<object*> scheme::va_list(pair *args, environment *env, bool eval)
{
	if (args == NULL)
		return std::list<object*>();

	object* car = eval ? evaluate(args->car, env) : args->car;
	std::list<object*> lst;
	lst.push_back(car);

	std::list<object*> cdrs = scheme::va_list(args->cdr, env, eval);
	while (cdrs.size() > 0) {
		object* o = cdrs.front();
		lst.push_back(o);
		cdrs.pop_front();
	}
	return lst;
}

int scheme::read_input(std::istream *in, std::ostream *out, environment* env)
{
	std::string exp = "";

	while (!in->eof()) {
		std::string input = "";
		if (exp == "" && out != NULL) *out << "> ";

		getline(*in, input);

		for (int i = 0; i < input.length(); i++)
		{
			exp += input[i];
			int lp = util::count(exp, '(');
			int rp = util::count(exp, ')');
			int qn = util::count(exp, '"');

			if (lp == rp && (qn % 2) == 0 && exp.length() > 1)
			{
				try
				{
					object *result = scheme::evaluate(parser::parse(exp), env);
					if (result != NULL && out != NULL)
						*out << ">> " << result->to_string() << std::endl;
				}

				catch (scheme_exception& e) {
					std::cerr << "exception: " << e.what() << std::endl;
				}
				exp = "";
			}
			else if (rp > lp)
			{
				std::cerr << "unexpected close-parenthesis." << std::endl;
				exp = "";
			}
		}
	}

	if (!exp.empty()) {
		std::cerr << "unexpected end in expression" << std::endl;
		return -1;
	}

	return 0;
}

int main()
{
	scheme::vsymbols = symbols::get_instance();

	environment *env = new environment(std::list<object*>(), std::list<object*>(), NULL);

	std::cout << "Scheme interpreter 0.010 by JLPC" << std::endl;

	scheme::read_input(&std::cin, &std::cout, env);

	delete env;
	delete scheme::vsymbols;

	return EXIT_SUCCESS;
}