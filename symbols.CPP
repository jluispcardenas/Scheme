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


#include "symbols.h"
#include "native.h"

symbols* symbols::instance(NULL);

symbols* symbols::get_instance()
{
	if (instance == NULL)
		instance = new symbols();

	return instance;
}

void symbols::install_primitives()
{
	table["#t"] = (object*)new sboolean(true);
	table["#f"] = (object*)new sboolean(false);

	table["apply"] = (object*)new native("apply", &native::apply, 2);
	table["car"] = (object*)new native("car", &native::car, 1);
	table["cdr"] = (object*)new native("cdr", &native::cdr, 1);
	table["cons"] = (object*)new native("cons", &native::cons, 2);
	table["define"] = (object*)new native("define", &native::define, 2);
	table["display"] = (object*)new native("display", &native::display, 1);
	table["equal?"] = (object*)new native("equal?", &native::equal, 2);
	table["if"] = (object*)new native("if", &native::_if, 3);
	table["lambda"] = (object*)new native("lambda", &native::lambda, 2);
	table["length"] = (object*)new native("length", &native::length, 1);
	table["list"] = (object*)new native("list", &native::list, -1);
	table["list?"] = (object*)new native("list?", &native::islist, 1);
	table["load"] = (object*)new native("load", &native::load, 1);

	table["null?"] = (object*)new native("null?", &native::isnull, 1);
	//table["macro"] = (object*)new native("macro", &native::macro, 2);

	table["quote"] = (object*)new native("quote", &native::quote, -1);

	table["+"] = (object*)new native("+", NULL, -1);
	table["-"] = (object*)new native("-", NULL, -1);
	table["*"] = (object*)new native("*", NULL, -1);
	table["/"] = (object*)new native("/", NULL, -1);

	table[">"] = (object*)new native(">", NULL, 2);
	table["<"] = (object*)new native("<", NULL, 2);
	table[">="] = (object*)new native(">=", NULL, 2);
	table["<="] = (object*)new native("<=", NULL, 2);
	table["="] = (object*)new native("=", NULL, 2);
}