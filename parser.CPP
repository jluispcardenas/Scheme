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


#include "parser.h"

object* parser::parse(std::string exp)
{
	std::queue<object*> tokens = lexer::tokenize(exp);
	std::stack<object*> stack;

	if (tokens.size() > 0)
		stack.push(sexp(tokens));

	if (stack.size() == 1)
	{
		return stack.top();
	}
	else
	{
		throw scheme_exception("empty stack");
	}
}

object* parser::sexp(std::queue<object*> &tokens)
{
	object* o = tokens.front();
	if (tokens.size() == 0)
	{
		return NULL;
	}
	else if (util::is<sstring>(o) && *((sstring*)o) == "\"")
	{
		tokens.pop();
		sstring* nx = (sstring*)tokens.front();
		tokens.pop();
		std::string ss = "";
		while (tokens.size() > 0 && *nx != "\"") {
			ss += (!ss.empty() ? " " : "") + ((std::string)*nx);
			nx = (sstring*)tokens.front();
			tokens.pop();
		}
		if (*nx != "\"")
			throw scheme_exception("Unterminated string constant");

		return new sstring("\"" + ss + "\"");
	}
	else if (util::is<sstring>(o) && *((sstring*)o) == "(")
	{
		tokens.pop();
		return parser::next_list(tokens);
	}
	else if (util::is<sstring>(o) && *((sstring*)o) == "'")
	{
		tokens.pop();
		return new pair(new sstring("quote"), (pair*)sexp(tokens));
	}
	else
	{
		tokens.pop();
		return o;
	}
}

object* parser::next_list(std::queue<object*> &tokens)
{
	object *cur = sexp(tokens);

	pair *p = NULL, *pt = NULL;
	while (tokens.size() > 0 && cur != NULL && *((sstring*)cur) != ")")
	{
		if (p == NULL)
		{
			p = new pair(cur, NULL);
			pt = p;
		}
		else
		{
			pt->cdr = new pair(cur, NULL);
			pt = pt->cdr;
		}
		cur = sexp(tokens);
	}

	return p;
}