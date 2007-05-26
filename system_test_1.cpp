/* NPath complexity analyser for C++.
   Copyright (C) 2007  Eddy Pronk

Gnocchi is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

Gnocchi is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include <iostream>
#include <map>
#include "gcov_reader.hpp"

struct fixture : public reporter
{
	struct test_data
	{
		test_data(int np, int nppp, const std::string& s)
			: npath(np)
			, npathpp(nppp)
			, function(s)
		{
		}
		int npath;
		int npathpp;
		std::string function;
	};
	void on_function(const std::string& fn, int npath, int npathpp)
	{
		actual.insert(std::make_pair(fn, std::make_pair(npath,npathpp)));
	}
	void add_test(int npath, int npathpp, const::std::string& fn)
	{
		expected.insert(std::make_pair(fn, std::make_pair(npath,npathpp)));
	}
	bool check()
	{
		bool error = false;
		container::iterator pos(expected.begin());
		container::iterator end(expected.end());
		for(; pos != end; ++pos)
		{
			container::iterator found = actual.find(pos->first);
			if(found == actual.end())
			{
				std::cout << "not found " << pos->first << std::endl;
				error = true;
			}
			if(pos->second != found->second)
			{
				std::cout << "failed function \"" << pos->first << "\"" << std::endl;
				std::cout << "expected " << pos->second.first << " " << pos->second.second << std::endl;
				std::cout << "actual   " << found->second.first << " " << found->second.second << std::endl;
				error = true;
			}
		}
		return error;
	}
	typedef std::pair<int,int> value_type;
	typedef std::map<std::string, value_type> container;
	container actual;
	container expected;
};

int main()
{
	fixture f;
	f.add_test(1, 1, "action");
	f.add_test(1, 1, "func_empty");
	f.add_test(2, 2, "func_if_with_assignment");
	f.add_test(2, 3, "func_if_with_function_call");
	f.add_test(2, 2, "func_if_else");
	f.add_test(4, 4, "func_if_else_2");
	f.add_test(80, 80, "example_1");
	f.add_test(16, 31, "example_2");
	f.add_test(5, 5, "example_3");
	f.add_test(4, 4, "example_4");
	f.add_test(4, 4, "example_5");

	gcov_reader reader(f);
	reader.open("/home/epronk/gnocchi/trunk/build/CMakeFiles/test_input.dir/test_input.gcno");

	bool error = f.check();
	return (error == true);
}
