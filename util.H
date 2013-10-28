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

#ifndef _UTIL_H_
#define _UTIL_H_

#include <sstream>
#include <stdarg.h>
#include <string>
#include "sboolean.h"
#include "sstring.h"
#include "pair.h"

class util
{
public:
	static bool is_atom(object *o);

	static bool is_string(object *o);

	static bool is_number(object *o);

	static const char* get_type(object* o);
	static int count(std::string str, const char c);
	static std::string stringtify(object* o);
	static std::string string_content(std::string str, char lv, char rv);


	static char* format(std::string format, ...);     
	template<typename T>
	static bool is(object *o)
	{
		T *s = dynamic_cast<T*>(o);      

		return (s);
	}

	template <typename T>
	static std::string to_string(T const& value) {
		std::stringstream sstr;
		sstr << value;
		return sstr.str();
	}
};

class array
{
public:
	template<typename T>
	static int index_of(T arr[], int s, T c)
	{
		for (int i = 0; i < s; i++) {
			if (arr[i] == c)
				return i;
		}
		return -1;
	}
};
#endif