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

#ifndef GCOV_READER_HPP_
#define GCOV_READER_HPP_

#include <string>
#include "function_data.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>
#include <map>

class Analyser;

struct gcov_reader
{
	Analyser& analyser;
	FunctionData::ptr data_;
	std::map<int,int> block_map_;
	boost::program_options::variables_map options_;
	gcov_reader(Analyser& a, boost::program_options::variables_map options = boost::program_options::variables_map())
		: analyser(a)
		, options_(options)
	{
	}
	void open(const boost::filesystem::path& filename);
	void tag_function (const char*, unsigned, unsigned);
	void tag_blocks (const char*, unsigned, unsigned);
	void tag_arcs (const char*, unsigned, unsigned);
	void tag_lines (const char*, unsigned, unsigned);
	void tag_counters (const char*, unsigned, unsigned);
	void tag_summary (const char*, unsigned, unsigned);
	void process_graph();
	void print_file(const std::string& filename);
};

#endif
