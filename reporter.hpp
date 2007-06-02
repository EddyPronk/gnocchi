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

#ifndef REPORTER_HPP_
#define REPORTER_HPP_

struct FunctionData
{
	FunctionData()
		: cyclomatic_complexity(0)
		, npath_complexity(0)
		, npath_complexity_2(0)
	{}
	int cyclomatic_complexity;
	int npath_complexity;
	int npath_complexity_2;
};

class reporter
{
public:
	virtual ~reporter() {}
	virtual void on_function(const std::string& fn, const FunctionData&) = 0;
};

#endif
