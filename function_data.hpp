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

#ifndef GCOV_FUNCTION_DATA
#define GCOV_FUNCTION_DATA

#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

struct FunctionData
{
	typedef boost::shared_ptr<FunctionData> ptr;
	FunctionData()
		: line_number(0)
		, cyclomatic_complexity(0)
		, npath_complexity(0)
		, cyclomatic_complexity_e(0)
		, npath_complexity_e(0)
	{}
	std::string name;
	boost::filesystem::path filename;
	int line_number;
	long long cyclomatic_complexity;
	long long npath_complexity;
	long long cyclomatic_complexity_e;
	long long npath_complexity_e;
};

#endif
