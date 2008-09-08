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

#include <iostream>
#include <string>
#include <map>
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

struct FunctionData
{
	typedef boost::shared_ptr<FunctionData> ptr;

	FunctionData()
		: line_number(0)
	{
	}
	std::string name;
	boost::filesystem::path filename;
	int line_number;
};

inline bool operator<(const FunctionData& lhs, const FunctionData& rhs)
{
	if (lhs.filename < rhs.filename) return true;
	if (lhs.filename > rhs.filename) return false;

	if (lhs.line_number < rhs.line_number) return true;
	if (lhs.line_number > rhs.line_number) return false;
	return false;
}

struct data_model
{
	data_model()
		: cyclomatic_complexity(0)
		, npath_complexity(0)
		, cyclomatic_complexity_e(0)
		, npath_complexity_e(0)
	{
	}
	FunctionData function;
	long cyclomatic_complexity;
	long npath_complexity;
	long cyclomatic_complexity_e;
	long npath_complexity_e;
};

inline bool operator<(const data_model& lhs, const data_model& rhs)
{
	if(lhs.npath_complexity < rhs.npath_complexity)	return true;
	if(lhs.npath_complexity > rhs.npath_complexity)	return false;

	if (lhs.function.filename < rhs.function.filename) return true;
	if (lhs.function.filename > rhs.function.filename) return false;

	if (lhs.function.line_number < rhs.function.line_number) return true;
	if (lhs.function.line_number > rhs.function.line_number) return false;
	return false;
}

#endif
