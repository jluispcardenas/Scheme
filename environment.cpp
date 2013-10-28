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


#include "environment.h"
#include <iostream>

environment::environment(std::list<object*> vars, std::list<object*> vals, environment *parent)
{
	if (!vars.empty() && !vals.empty()) {
		extend(vars, vals);
	}
	
	this->parent = parent;
}

object* environment::lookup(std::string name)
{
	if (table[name] == NULL)
	{
		if (parent != NULL)
			return parent->lookup(name);
		else
			throw scheme_exception(util::format("the name '%s' does not exist in the current context", name.c_str()));
	}
	else
	{
		return table[name];
    }
}
    	
void environment::extend(std::string var, object* val)
{
	table[var] = val;
}

void environment::extend(std::list<object*> vars, std::list<object*> vals)
{
	while (!vars.empty() && !vals.empty())
	{
		sstring* k = (sstring*)vars.front();
		sstring* v = (sstring*)vals.front();
		table[k->to_string()] = v;
		vars.pop_front();
		vals.pop_front();
	}
}