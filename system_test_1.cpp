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
#include "analyser.hpp"
#include "reporter.hpp"

struct fixture : public reporter
{
	bool error;
	fixture()
		: error(false)
	{
	}
	void on_function(FunctionData::ptr param)
	{
		std::cout << "on_function " << param->name << std::endl;
		actual_.insert(std::make_pair(param->name, param));
	}
	void add_test(int c, int npath, int npathpp, const::std::string& fn)
	{
		FunctionData::ptr data(new FunctionData);
		data->cyclomatic_complexity = c;
		data->npath_complexity = npath;
		data->npath_complexity_e = npathpp;
		expected_.insert(std::make_pair(fn, data));
	}
	void check(const std::string& name, int expected, int actual)
	{
		if(expected != actual)
		{
			std::cout << "FAIL : " << name
					  << " expected: " << expected
					  << " actual: " << actual << std::endl;
			error = true;
		}
	}
	bool check()
	{
		container::iterator pos(expected_.begin());
		container::iterator end(expected_.end());
		for(; pos != end; ++pos)
		{
			container::iterator actual = actual_.find(pos->first);
			if(actual == actual_.end())
			{
				std::cout << "not found " << pos->first << std::endl;
				error = true;
			}
			else
			{
				std::cout << "check: " << pos->first << std::endl;
				//check("cyclomatic_complexity", pos->second->cyclomatic_complexity, actual->second->cyclomatic_complexity);
				check("npath_complexity", pos->second->npath_complexity, actual->second->npath_complexity);
				//check("npath_complexity_2", pos->second->npath_complexity_2, actual->second->npath_complexity_2);
			}
		}
		return error;
	}
	typedef std::map<std::string, FunctionData::ptr> container;
	container actual_;
	container expected_;
};

int main()
{
	fixture f;

	f.add_test(0, 1, 1, "action");
	f.add_test(0, 1, 1, "func_empty");
	f.add_test(0, 2, 2, "func_if_with_assignment");
	f.add_test(0, 2, 3, "func_if_with_function_call");
	f.add_test(0, 2, 2, "func_if_else");
	f.add_test(0, 4, 4, "func_if_else_2");
	f.add_test(0, 80, 80, "example_1");
	f.add_test(0, 16, 31, "example_2");
	f.add_test(0, 5, 5, "example_3");
	f.add_test(0, 4, 4, "example_4");

	Analyser a(f);
	gcov_reader reader(a);
	reader.open("/home/epronk/gnocchi/trunk/CMakeFiles/test_input.dir/test_input.gcno");
	reader.open("/home/epronk/gnocchi/trunk/CMakeFiles/test_input_2.dir/test_input_2.gcno");

	a.report(0);
	bool error = f.check();
	return (error == true);
}
