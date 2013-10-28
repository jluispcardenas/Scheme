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


#include "util.h"

bool util::is_atom(object *o)
{
	return !is<pair>(o);
}

bool util::is_string(object *o)
{
	if (util::is<sstring>(o))
	{
		std::string s = (std::string)*((sstring*)o);
		if (s.length() > 1) {
			if (s[0] == '"' && s[s.length()-1] == '"')
				return true;
		}
	}
	return false;
}

bool util::is_number(object *o)
{
	std::string s = ((sstring*)o)->to_string();
	const char *p = s.c_str();
	char c;
	while ((c=*p++)) {
		if (!isdigit(c) && c != '.') return false;
	}
	return true;
}

int util::count(std::string str, const char c)
{
	int pos = 0, n = 0;
	while ((pos = str.find(c, pos)) != std::string::npos) {
		n++;
		pos++;
	}
	return n;
}

const char* util::get_type(object* o)
{
	if (is<pair>(o)) {
		return "pair";
	}
	else if (is<sstring>(o))
	{
		return "atom";
	}
	else if (is<sboolean>(o))
	{
		return "boolean";
	}
	else
	{
		return "undefined";
	}       
}

char* util::format(std::string format, ...)
{
	char *buffer = new char[250];
	va_list args;
	va_start (args, format);
	vsprintf (buffer, format.c_str(), args);
	va_end (args);

	return buffer;
}

std::string util::stringtify(object* o)
{
	if (o == NULL) return "";
	else if (util::is_atom(o))
	{
		return o->to_string();
	}
	else if (util::is<pair>(o))
	{
		pair* p = (pair*)o;
		std::string res = stringtify(p->car);
		std::string scdr = string_content(stringtify(p->cdr), '(', ')');
		if (!scdr.empty())
			res += ", " + scdr;

		return "(" + res + ")";
	}
	else
	{
		return "undefined?";
	}
}

std::string util::string_content(std::string str, char lv, char rv)
{
	if (!str.empty())
	{
		if (str[0] == lv)
			str = str.substr(1);
		if (str[str.length()-1] == rv)
			str = str.substr(0, str.length()-1);
	}

	return str;
}