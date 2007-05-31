/* NPath complexity analyser for C++.
   Copyright (C) 2007  Eddy Pronk

Derived from gcov-dump, which is part of GCC
Copyright (C) 2002, 2003, 2004,
              2005, 2006 Free Software Foundation, Inc.
Contributed by Nathan Sidwell <nathan@codesourcery.com>

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

#include <string>
#include <boost/graph/adjacency_list.hpp>

struct params
{
	params()
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
	virtual void on_function(const std::string& fn, const params&) = 0;
};

using namespace boost;

//typedef adjacency_list < vecS, vecS, undirectedS > Graph;
typedef boost::adjacency_list<> Graph;

struct gcov_reader
{
	reporter& reporter_;
	std::string func_name;
	Graph g;
	gcov_reader(reporter& r)
		: reporter_(r)
	{
	}
	~gcov_reader()
	{
		//os_ << "}" << std::endl;
	}
	void open(const char *filename);
	void tag_function (const char *, unsigned, unsigned);
	void tag_blocks (const char *, unsigned, unsigned);
	void tag_arcs (const char *, unsigned, unsigned);
	void tag_lines (const char *, unsigned, unsigned);
	void tag_counters (const char *, unsigned, unsigned);
	void tag_summary (const char *, unsigned, unsigned);
	void process_graph();
};
