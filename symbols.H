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

#ifndef _SYMBOLS_H_
#define _SYMBOLS_H_

#include <map>
#include <list>
#include <string>
#include "native.h"
#include "object.h"
#include "sstring.h"
//#include "_macro.h"

class symbols
{
public:
	static symbols *get_instance();

	object *get(std::string name)
	{
		return table[name];
	}

private:       
	symbols() {
		install_primitives();
	}

	void install_primitives();

private:
	static symbols *instance;
	std::map<std::string, object*> table;
};

#endif