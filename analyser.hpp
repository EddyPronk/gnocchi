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

#ifndef ANALYSER_HPP_
#define ANALYSER_HPP_

#include <boost/graph/adjacency_list.hpp>
#include "reporter.hpp"
#include "function_data.hpp"

typedef boost::adjacency_list<> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::vertices_size_type size_type;

class Analyser
{
public:
	Analyser(reporter& r)
		: reporter_(r)
	{
	}
	void process(FunctionData::ptr data);
	void clear();
	void add_edge(int src, int dest);
	void calculate_npath();
	void calculate_npath_2();
	void report();
private:
	reporter& reporter_;	
	Graph graph_;
	FunctionData::ptr data_;
	std::multimap<int, FunctionData::ptr> functions;
};

#endif
